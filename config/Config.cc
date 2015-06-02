/*
 * =====================================================================================
 *
 *       Filename:  Config.cc
 *
 *    Description:  Config
 *
 *        Version:  1.0
 *        Created:  06/02/2015 11:22:21 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */

#include "Config.h"
#include "common/Common.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/encodedstream.h"
#include <cstdio>
#include <cstdarg>

#define CONFIG_BUFF_SIZE 65535

bool ConfigFile::LoadFile(const char* file_name) {
    FILE* fp = fopen(file_name, "r");
    if (!fp) {
        return false;
    }

    char readBuffer[CONFIG_BUFF_SIZE];
    rapidjson::FileReadStream fstream(fp, readBuffer, CONFIG_BUFF_SIZE);
    rapidjson::AutoUTFInputStream<unsigned, rapidjson::FileReadStream> auto_fstream(fstream);
    if (!document_.ParseStream<0, rapidjson::AutoUTF<unsigned>>(auto_fstream).HasParseError()) {
        LOG_TRACE("Config file(%s) load success!", file_name);
        fclose(fp);

        return true;
    }

    LOG_ERROR("Config file(%s) load failed!", file_name);
    fclose(fp);

    return false;
}

std::string ConfigFile::GetString(const char *key, const char* field) {
    rapidjson::Value val;
    if (GetValue(key, field, val) && val.IsString()) {
        return val.GetString();
    }
    return "";
}

std::string ConfigFile::GetStringDefault(const char *key, const char* field, const char* default_value) {
    std::string value = GetString(key, field);
    if ("" != value) {
        return value;
    }
    return default_value;
}

std::string ConfigFile::GetStringVA(const char *default_value, int argc, ...) {
    rapidjson::Value val;

    va_list arg_list;
    va_start(arg_list, argc);

    if (GetValueNA(val, argc, arg_list)) {
        va_end(arg_list);
        return val.GetString();
    }

    va_end(arg_list);
    return default_value;
}

int ConfigFile::GetInt(const char *key, const char* field) {
    rapidjson::Value val;
    if (GetValue(key, field, val) && val.IsInt()) {
        return val.GetInt();
    }
    return 0;
}

int ConfigFile::GetIntDefault(const char *key, const char* field, int default_value) {
    int value = GetInt(key, field);
    if (0 != value) {
        return value;
    }
    return default_value;
}

int ConfigFile::GetIntVA(int default_value, int argc, ...) {
    rapidjson::Value val;

    va_list arg_list;
    va_start(arg_list, argc);

    if (GetValueNA(val, argc, arg_list)) {
        va_end(arg_list);
        return val.GetInt();
    }

    va_end(arg_list);
    return default_value;
}

bool ConfigFile::GetBool(const char *key, const char* field) {
    rapidjson::Value val;
    if (GetValue(key, field, val) && val.IsBool()) {
        return val.GetBool();
    }
    return false;
}

bool ConfigFile::GetBoolDefault(const char *key, const char* field, bool default_value) {
    bool value = GetBool(key, field);
    if (false != value) {
        return value;
    }
    return default_value;
}

bool ConfigFile::GetBoolVA(bool default_value, int argc, ...) {
    rapidjson::Value val;

    va_list arg_list;
    va_start(arg_list, argc);

    if (GetValueNA(val, argc, arg_list)) {
        va_end(arg_list);
        return val.GetBool();
    }

    va_end(arg_list);
    return default_value;
}

float ConfigFile::GetFloat(const char *key, const char* field) {
    rapidjson::Value val;
    if (GetValue(key, field, val) && val.IsDouble()) {
        return val.GetDouble();
    }
    return 0.0f;
}

float ConfigFile::GetFloatDefault(const char *key, const char* field, float default_value) {
    float value = GetFloat(key, field);
    if (0.0f != value) {
        return value;
    }
    return default_value;
}

float ConfigFile::GetFloatVA(float default_value, int argc, ...) {
    rapidjson::Value val;

    va_list arg_list;
    va_start(arg_list, argc);

    if (GetValueNA(val, argc, arg_list)) {
        va_end(arg_list);
        return val.GetDouble();
    }

    va_end(arg_list);
    return default_value;
}

bool ConfigFile::GetValue(const char* key, const char* field, rapidjson::Value& val) {
    rapidjson::Value::MemberIterator it;
    if (document_.MemberEnd() != (it = document_.FindMember(key))) {
        if (field) {
            if (document_.MemberEnd() != (it = it->value.FindMember(field))) {
                val = it->value;
                return true;
            }
        } else {
            val = it->value;
            return true;
        }
    }

    return false;
}

bool ConfigFile::GetValueNA(rapidjson::Value &val, int argc, va_list& arg_list) {
    rapidjson::Value::MemberIterator it = document_.MemberEnd();
    while(argc--) {
        char* key = va_arg(arg_list, char*);
        // 检查是否未初始化it
        if (it == document_.MemberEnd()) {
            // 真正初始化it
            if (document_.MemberEnd() == (it = document_.FindMember(key))) {
                return false;
            } else {
                continue;
            }
        }

        if (document_.MemberEnd() == (it = it->value.FindMember(key))) {
            return false;
        }
    }

    val = it->value;
    return true;
}
