/*
 * =====================================================================================
 *
 *       Filename:  MySQLDatabase.cc
 *
 *    Description:  MySQLDatabase
 *
 *        Version:  1.0
 *        Created:  11/09/2015 03:18:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "MySQLDatabase.h"

MySQLDatabase::~MySQLDatabase() {
    for (uint32_t i = 0; i < connection_count_; ++i) {
        mysql_close(static_cast<MySQLDatabaseConnection*>(connections_[i])->Mysql_);
        delete connections_[i];
    }
    delete[] connections_;
}

void MySQLDatabase::BeginTransaction(DatabaseConnection *conn) {
    SendQuery(conn, "START TRANSACTION");
}

void MySQLDatabase::EndTransaction(DatabaseConnection *conn) {
    SendQuery(conn, "COMMIT");
}

bool MySQLDatabase::Initialize(const char *hostname, uint16_t port, const char *username, const char *password, const char *database_name, uint32_t connection_count) {
    hostname_ = std::string(hostname);
    port_ = port;
    username_ = std::string(username);
    password_ = std::string(password_);
    database_name_ = std::string(database_name);
    connection_count_ = connection_count;

    LOG_KTRACE("MySQLDatabase", "Connecting to `%s`, database `%s` ...", hostname, database_name);

    if (mysql_library_init(0, NULL, NULL)) {
        LOG_KERROR("MySQLDatabase", "Could not initialize MySQL library");
        return false;
    }

    MySQLDatabaseConnection** mysql_connections;
    mysql_connections = new MySQLDatabaseConnection*[connection_count];
    connections_ = reinterpret_cast<DatabaseConnection**>(mysql_connections);

    MYSQL* mysql = NULL;
    MYSQL* mysql2 = NULL;
    my_bool my_true = true;

    for (uint32_t i = 0; i < connection_count; ++i) {
        mysql = mysql_init(NULL);
        if (NULL == mysql) {
            continue;
        }

        if (mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "utf8")) {
            LOG_KERROR("MySQLDatabase", "Cound not set utf8 character set.");
        }
        if (mysql_options(mysql, MYSQL_OPT_RECONNECT, &my_true)) {
            LOG_KERROR("MySQLDatabase", "MYSQL_OPT_RECONNECT could not be set, connection drops may occur but will be counteracted.");
        }
        
        mysql2 = mysql_real_connect(mysql, hostname, username, password, database_name, port, NULL, 0);
        if (NULL == mysql2) {
            LOG_KERROR("MySQLDatabase", "Connection failed due to: `%s`", mysql_error(mysql));
            mysql_close(mysql);
            return false;
        }

        mysql_connections[i] = new MySQLDatabaseConnection;
        mysql_connections[i]->Mysql_ = mysql2;
    }

    Database::Initialize_();
    return true;
}

std::string MySQLDatabase::EscapeString(std::string escape) {
    char buffer[QUERY_STRING_SIZE] = { 0 };

    std::string ret;
    DatabaseConnection* conn = GetFreeConnection();
    if (mysql_real_escape_string(static_cast<MySQLDatabaseConnection*>(conn)->Mysql_, buffer, escape.c_str(), (unsigned long)escape.size()) == 0) {
        ret = escape.c_str();
    } else {
        ret = buffer;
    }

    conn->Busy_.UnLock();

    return std::string(ret);
}


std::string MySQLDatabase::EscapeString(const char* escape, DatabaseConnection* conn) {
    char buffer[QUERY_STRING_SIZE] = { 0 };
    const char* ret;

    if (mysql_real_escape_string(static_cast<MySQLDatabaseConnection*>(conn)->Mysql_, buffer, escape, (unsigned long)strlen(escape)) == 0) {
        ret = escape;
    } else {
        ret = buffer;
    }

    return std::string(ret);
}

void MySQLDatabase::EscapeLongString(const char *str, uint32_t len, std::stringstream &out) {
    char buffer[655356 * 3] = { 0 };

    const char* ret;
    DatabaseConnection* conn = GetFreeConnection();
    if (mysql_real_escape_string(static_cast<MySQLDatabaseConnection*>(conn)->Mysql_, buffer, str, (unsigned long)len) == 0) {
        ret = str;
    } else {
        ret = buffer;
    }

    out.write(buffer, (std::streamsize)strlen(ret));
    conn->Busy_.UnLock();
}

void MySQLDatabase::Shutdown() {
    EndThreads();
    mysql_library_end();
}

bool MySQLDatabase::SendQuery(DatabaseConnection *conn, const char *sql) {
    int result = 0;
    int max_retry = 2;

    for (int i = 1; i <= max_retry; ++i) {
        result = mysql_query(static_cast<MySQLDatabaseConnection*>(conn)->Mysql_, sql);
        if (result > 0) {
            if (!HandleError(static_cast<MySQLDatabaseConnection*>(conn), mysql_errno(static_cast<MySQLDatabaseConnection*>(conn)->Mysql_))
                    && i == max_retry) {
                LOG_ERROR("sql query failed due to [%s], Query: [%s]", mysql_error(static_cast<MySQLDatabaseConnection*>(conn)->Mysql_), sql);
            }
        } else {
            break;
        }
    }

    return (result == 0 ? true : false);
}

QueryResult* MySQLDatabase::StoreQueryResult(DatabaseConnection *conn) {
    MySQLQueryResult* res;
    MySQLDatabaseConnection* db = static_cast<MySQLDatabaseConnection*>(conn);
    MYSQL_RES* my_res = mysql_store_result(db->Mysql_);
    uint32_t rows = (uint32_t)mysql_affected_rows(db->Mysql_);
    uint32_t fields = (uint32_t)mysql_field_count(db->Mysql_);

    if (rows == 0 || fields == 0 || my_res == 0) {
        if (my_res != NULL) {
            mysql_free_result(my_res);
        }
        return NULL;
    }

    res = new MySQLQueryResult(my_res, fields, rows);
    res->NextRow();

    return res;
}

bool MySQLDatabase::HandleError(MySQLDatabaseConnection *conn, uint32_t error_number) {
    // Handle errors that should cause a reconnect to the Database
    switch(error_number) {
        case 2006:  // Mysql server has gone away
        case 2008:  // Client ran out of memory
        case 2013:  // Lost connection to sql server during query
        case 2055:  // Lost connection to sql server - system error
            {
                return Reconnect(conn);
                break;
            }
    }
    return false;
}

bool MySQLDatabase::Reconnect(MySQLDatabaseConnection *conn) {
    MYSQL* mysql;
    MYSQL* mysql2;

    mysql = mysql_init(NULL);
    mysql2 = mysql_real_connect(mysql, hostname_.c_str(), username_.c_str(), password_.c_str(), database_name_.c_str(), port_, NULL, 0);

    if (NULL == mysql2) {
        LOG_KERROR("MySQLDatabase", "Cound not reconnect to database because of `%s`", mysql_error(mysql));
        mysql_close(mysql);
        return false;
    }

    if (NULL != conn->Mysql_) {
        mysql_close(conn->Mysql_);
    }

    conn->Mysql_ = mysql;
    return true;
}

MySQLQueryResult::MySQLQueryResult(MYSQL_RES* res, uint32_t field_count, uint32_t row_count) : QueryResult(field_count, row_count), result_(res) {
    current_row_ = new Field[field_count];
}

MySQLQueryResult::~MySQLQueryResult() {
    mysql_free_result(result_);
    delete[] current_row_;
}

bool MySQLQueryResult::NextRow() {
    MYSQL_ROW row = mysql_fetch_row(result_);
    if (NULL == row) {
        return false;
    }

    for (uint32_t i = 0; i < field_count_; ++i) {
        current_row_[i].SetValue(row[i]);
    }

    return true;
}
