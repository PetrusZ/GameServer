/*
 * =====================================================================================
 *
 *       Filename:  QueryWorkerThread.cc
 *
 *    Description:  QueryWorkerThread
 *
 *        Version:  1.0
 *        Created:  11/26/2015 05:51:33 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "QueryWorkerThread.h"

bool QueryWorkerThread::Run() {
    QueryResult* temp_result = sMySQLDatabase.QueryNA((char*)packet_);
    MySQLQueryResult* result = dynamic_cast<MySQLQueryResult*>(temp_result);

    for (uint32_t i = 0; i < result->row_count(); ++i) {
        Field* field = result->Fetch();
        LOG_INFO("field %d: %s", i, field->GetString());
        result->NextRow();
    }

    result->Delete();

    return true;
}
