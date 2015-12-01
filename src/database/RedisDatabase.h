/*
 * =====================================================================================
 *
 *       Filename:  RedisDatabase.h
 *
 *    Description:  RedisDatabase
 *
 *        Version:  1.0
 *        Created:  11/30/2015 10:16:19 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */

#ifndef REDISDATABASE_H_A2DO91UB
#define REDISDATABASE_H_A2DO91UB

#include "Database.h"

#include <hiredis/hiredis.h>

struct RedisDatabaseConnection;

class RedisQueryResult;

class RedisDatabase : public Database, public Singleton<RedisDatabase> {
    public:
        RedisDatabase() = default;
        virtual ~RedisDatabase();

        bool Initialize(const char* hostname, uint16_t port,
                const char* username, const char* password,
                const char* database_name, uint32_t connection_count);

        void Shutdown();

        std::string EscapeString(std::string escape);
        void EscapeLongString(const char* str, uint32_t len, std::stringstream& out);
        std::string EscapeString(const char* escape, DatabaseConnection* conn);

    protected:
        bool HandleError(RedisDatabaseConnection* conn, uint32_t error_number);
        bool Reconnect(RedisDatabaseConnection* conn);

        void BeginTransaction(DatabaseConnection* conn);
        void EndTransaction(DatabaseConnection* conn);

        bool SendQuery(DatabaseConnection* conn, const char* sql);
        QueryResult* StoreQueryResult(DatabaseConnection* conn);
};

struct RedisDatabaseConnection : public DatabaseConnection {
    redisContext* Redis_;
    redisReply* Reply_;
};

class RedisQueryResult : public QueryResult {
    public:
        RedisQueryResult(redisReply* reply, uint32_t field_count, uint32_t row_count);
        virtual ~RedisQueryResult();

        bool NextRow();

    protected:
        uint32_t current_row_count_ = 0;
        redisReply* result_;
};

#define sRedisDatabase RedisDatabase::getSingleton()

#endif /* end of include guard: REDISDATABASE_H_A2DO91UB */
