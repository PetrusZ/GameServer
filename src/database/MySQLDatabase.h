/*
 * =====================================================================================
 *
 *       Filename:  MySQLDatabase.h
 *
 *    Description:  MySQLDatabase
 *
 *        Version:  1.0
 *        Created:  11/06/2015 05:31:27 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */

#ifndef MYSQLDATABASE_H_C0F5KYYR
#define MYSQLDATABASE_H_C0F5KYYR

#include "Database.h"

#include <mysql/mysql.h>

struct MySQLDatabaseConnection;

class MySQLQueryResult;

class MySQLDatabase : public Database, public Singleton<MySQLDatabase> {
    public:
        MySQLDatabase();
        virtual ~MySQLDatabase();

        bool Initialize(const char* hostname, uint16_t port,
                const char* username, const char* password,
                const char* database_name, uint32_t connection_count);

        void Shutdown();

        std::string EscapeString(std::string escape);
        void EscapeLongString(const char* str, uint32_t len, std::stringstream& out);
        std::string EscapeString(const char* escape, DatabaseConnection* conn);

    protected:
        bool HandleError(MySQLDatabaseConnection* conn, uint32_t error_number);
        bool Reconnect(MySQLDatabaseConnection* conn);

        void BeginTransaction(DatabaseConnection* conn);
        void EndTransaction(DatabaseConnection* conn);

        bool SendQuery(DatabaseConnection* conn, const char* sql);
        QueryResult* StoreQueryResult(DatabaseConnection* conn);
};

struct MySQLDatabaseConnection : public DatabaseConnection {
    MYSQL* Mysql_;
};

class MySQLQueryResult : public QueryResult {
    public:
        MySQLQueryResult(MYSQL_RES* res, uint32_t field_count, uint32_t row_count);
        virtual ~MySQLQueryResult();

        bool NextRow();

    protected:
        MYSQL_RES* result_;
};

#define sMySQLDatabase MySQLDatabase::getSingleton()

#endif /* end of include guard: MYSQLDATABASE_H_C0F5KYYR */
