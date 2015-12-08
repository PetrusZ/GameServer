/*
 * =====================================================================================
 *
 *       Filename:  RedisDatabase.cc
 *
 *    Description:  RedisDatabase
 *
 *        Version:  1.0
 *        Created:  11/30/2015 03:26:36 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef REDISDATABASE_CC_THW3Y1JZ
#define REDISDATABASE_CC_THW3Y1JZ

#include "RedisDatabase.h"

RedisDatabase::~RedisDatabase() {
    for (uint32_t i = 0; i < connection_count_; ++i) {
        redisFree(static_cast<RedisDatabaseConnection*>(connections_[i])->Redis_);
        if (NULL != static_cast<RedisDatabaseConnection*>(connections_[i])->Reply_) {
            freeReplyObject(static_cast<RedisDatabaseConnection*>(connections_[i])->Reply_);
        }

        delete connections_[i];
    }
    delete[] connections_;

}

bool RedisDatabase::Initialize(const char *hostname, uint16_t port, const char *username, const char *password, const char *database_name, uint32_t connection_count) {
    hostname_ = std::string(hostname);
    port_ = port;
    username_ = std::string(username);
    password_ = std::string(password_);
    database_name_ = std::string(database_name);
    connection_count_ = connection_count;

    LOG_KTRACE("RedisDatabase", "Connecting to `%s`, database `%s` ...", hostname, database_name);

    RedisDatabaseConnection** redis_connections = new RedisDatabaseConnection*[connection_count_];
    connections_ = reinterpret_cast<DatabaseConnection**>(redis_connections);

    for (uint32_t i = 0; i < connection_count_; ++i) {
        redisContext* redis = redisConnect(hostname_.c_str(), port_);

        if (NULL == redis) {
            LOG_KERROR("RedisDatabase", "Connection failed due to unknown, returned NULL");
            return false;
        } else if (redis->err) {
            LOG_KERROR("RedisDatabase", "Connection failed due to [%s] %s", redis->err, redis->errstr);
            redisFree(redis);
            return false;
        }

        redis_connections[i] = new RedisDatabaseConnection();
        redis_connections[i]->Redis_ = redis;
    }

    Database::Initialize_();
    return true;
}

void RedisDatabase::Shutdown() {
    EndThreads();
}

bool RedisDatabase::SendQuery(DatabaseConnection *conn, const char *sql) {
    redisReply* reply;
    int max_retry = 2;

    for (int i = 1; i <= max_retry; ++i) {
        reply = (redisReply*)redisCommand(static_cast<RedisDatabaseConnection*>(conn)->Redis_, sql);
        if (NULL == reply || reply->type == REDIS_REPLY_ERROR) {
            if (!HandleError(static_cast<RedisDatabaseConnection*>(conn), static_cast<RedisDatabaseConnection*>(conn)->Redis_->err)
                    && i == max_retry) {
                if (reply) {
                    LOG_ERROR("redis query failed due to [%s], Query: [%s]", reply->str, sql);
                } else {
                    LOG_ERROR("redis query failed due to unknown and returned NULL, Query: [%s]", sql);
                }
            }
        } else {
            static_cast<RedisDatabaseConnection*>(conn)->Reply_ =  reply;
            break;
        }
    }

    return (!reply || reply->type == REDIS_REPLY_ERROR ? false : true);
}

bool RedisDatabase::HandleError(RedisDatabaseConnection *conn, uint32_t error_number) {
    switch (error_number) {
        case REDIS_ERR_IO:
        case REDIS_ERR_EOF:
            return Reconnect(conn);
            break;
    }
    return false;
}

bool RedisDatabase::Reconnect(RedisDatabaseConnection *conn) {
    redisContext* redis = redisConnect(hostname_.c_str(), port_);

    if (NULL == redis) {
        LOG_KERROR("RedisDatabase", "Reconnection failed due to unknown, returned NULL");
        return false;
    } else if (redis->err) {
        LOG_KERROR("RedisDatabase", "Reconnection failed due to [%s] %s", redis->err, redis->errstr);
        redisFree(redis);
        return false;
    }

    if (conn->Redis_) {
        redisFree(conn->Redis_);
    }

    conn->Redis_ = redis;

    return true;
}

QueryResult* RedisDatabase::StoreQueryResult(DatabaseConnection *conn) {
    RedisDatabaseConnection* redis_conn = static_cast<RedisDatabaseConnection*>(conn);
    if (NULL == redis_conn->Reply_) {
        return NULL;
    }

    if (REDIS_REPLY_NIL == redis_conn->Reply_->type) {
        return NULL;
    }

    int row_count = 1;
    int field_count = 1;
    if (REDIS_REPLY_ARRAY == redis_conn->Reply_->type) {
        row_count = redis_conn->Reply_->elements;
    }


    RedisQueryResult* result = new RedisQueryResult(redis_conn->Reply_, row_count, field_count);
    result->NextRow();

    redis_conn->Reply_ = NULL;

    return result;
}

void RedisDatabase::BeginTransaction(DatabaseConnection *conn) {
    SendQuery(conn, "MULTI");
}

void RedisDatabase::EndTransaction(DatabaseConnection *conn) {
    SendQuery(conn, "EXEC");
}

std::string RedisDatabase::EscapeString(std::string escape) {
    return escape;
}

void RedisDatabase::EscapeLongString(const char* str, uint32_t len, std::stringstream& out) {

}

std::string RedisDatabase::EscapeString(const char* escape, DatabaseConnection* conn) {
    return std::string(escape);
}

RedisQueryResult::RedisQueryResult(redisReply* reply, uint32_t field_count, uint32_t row_count) : QueryResult(field_count, row_count), result_(reply) {
    current_row_ = new Field[field_count];
}

RedisQueryResult::~RedisQueryResult() {
    freeReplyObject(result_);
    delete[] current_row_;
}

bool RedisQueryResult::NextRow() {
    if (current_row_count_ == row_count_) {
        return false;
    }

    // TODO: field count
    switch (result_->type) {
        case REDIS_REPLY_STATUS:
            current_row_[0].SetValue(result_->str);
            break;
        case REDIS_REPLY_STRING:
            current_row_[0].SetValue(result_->str);
            break;
        case REDIS_REPLY_INTEGER:
            current_row_[0].SetValue((char*)result_->integer);
            break;
        case REDIS_REPLY_ARRAY:
            {
                redisReply* res = result_->element[current_row_count_++];
                current_row_[0].SetValue(res->str);
            }
            break;
        default:
            break;
    }

    return true;
}

#endif /* end of include guard: REDISDATABASE_CC_THW3Y1JZ */
