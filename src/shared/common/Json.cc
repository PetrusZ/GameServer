/*
 * =====================================================================================
 *
 *       Filename:  Json.cc
 *
 *    Description:  Json
 *
 *        Version:  1.0
 *        Created:  12/02/2015 03:24:58 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "Json.h"

#include "rapidjson/error/en.h"
#include "rapidjson/prettywriter.h"

#define JSON_BUFF_SIZE 65535

Json::Json() {
    document_.SetObject();
}

bool Json::Parse(const std::string& json) {
    return Parse(json.c_str());
}

bool Json::Parse(char* json) {
    if (document_.Parse(json).HasParseError()) {
        LOG_ERROR("Json(%s) parse error at offset %u, due to %s", json,
                (unsigned)document_.GetErrorOffset(), rapidjson::GetParseError_En(document_.GetParseError()));
        return false;
    }
    return true;
}

bool Json::Parse(const char* json) {
    if (document_.Parse(json).HasParseError()) {
        LOG_ERROR("Json(%s) parse error at offset %u, due to %s", json,
                (unsigned)document_.GetErrorOffset(), rapidjson::GetParseError_En(document_.GetParseError()));
        return false;
    }
    return true;
}

bool Json::ParseInsitu(char* json) {
    if (document_.ParseInsitu(json).HasParseError()) {
        LOG_ERROR("Json(%s) parse error at offset %u, due to %s", json,
                (unsigned)document_.GetErrorOffset(), rapidjson::GetParseError_En(document_.GetParseError()));
        return false;
    }
    return true;
}


// XXX: 返回值是否还有效？
std::string Json::PrettyFormat() {
    rapidjson::StringBuffer buffer;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    document_.Accept(writer);
    return buffer.GetString();
}

std::string Json::Format() {
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    document_.Accept(writer);
    return buffer.GetString();
}

bool Json::GetString(const char *key, const char* field, std::string& value) {
    rapidjson::Value val;
    if (GetValue(key, field, val) && val.IsString()) {
        value = val.GetString();
        return true;
    }
    return false;
}

bool Json::GetString(std::string& value, int argc, ...) {
    rapidjson::Value val;

    va_list arg_list;
    va_start(arg_list, argc);

    if (GetValue(val, argc, arg_list)) {
        va_end(arg_list);
        value = val.GetString();
        return true;
    }

    va_end(arg_list);
    return false;
}

bool Json::GetInt(const char *key, const char* field, int& value) {
    rapidjson::Value val;
    if (GetValue(key, field, val) && val.IsInt()) {
        value = val.GetInt();
        return true;
    }
    return false;
}

bool Json::GetInt(int& value, int argc, ...) {
    rapidjson::Value val;

    va_list arg_list;
    va_start(arg_list, argc);

    if (GetValue(val, argc, arg_list)) {
        va_end(arg_list);
        value = val.GetInt();
        return true;
    }

    va_end(arg_list);
    return false;
}

bool Json::GetBool(const char *key, const char* field, bool& value) {
    rapidjson::Value val;
    if (GetValue(key, field, val) && val.IsBool()) {
        value = val.GetBool();
        return true;
    }
    return false;
}

bool Json::GetBool(bool& value, int argc, ...) {
    rapidjson::Value val;

    va_list arg_list;
    va_start(arg_list, argc);

    if (GetValue(val, argc, arg_list)) {
        va_end(arg_list);
        value = val.GetBool();
    }

    va_end(arg_list);
    return false;
}

bool Json::GetFloat(const char *key, const char* field, float& value) {
    rapidjson::Value val;
    if (GetValue(key, field, val) && val.IsDouble()) {
        value = val.GetDouble();
        return true;
    }
    return false;
}

bool Json::GetFloat(float& value, int argc, ...) {
    rapidjson::Value val;

    va_list arg_list;
    va_start(arg_list, argc);

    if (GetValue(val, argc, arg_list)) {
        va_end(arg_list);
        value = val.GetDouble();
        return true;
    }

    va_end(arg_list);
    return false;
}

bool Json::SetString(const char *key, const char* field, const std::string& value) {
    rapidjson::Value val;
    val.SetString(value.c_str(), value.size(), document_.GetAllocator());

    if (SetValue(key, field, val)) {
        return true;
    }
    return false;
}

bool Json::SetString(const std::string& value, int argc, ...) {
    rapidjson::Value val;
    val.SetString(value.c_str(), value.size(), document_.GetAllocator());

    va_list arg_list;
    va_start(arg_list, argc);

    if (SetValue(val, argc, arg_list)) {
        va_end(arg_list);
        return true;
    }

    va_end(arg_list);
    return false;
}

bool Json::SetInt(const char *key, const char* field, int value) {
    rapidjson::Value val;
    val.SetInt(value);

    if (SetValue(key, field, val)) {
        return true;
    }
    return false;
}

bool Json::SetInt(int value, int argc, ...) {
    rapidjson::Value val;
    val.SetInt(value);

    va_list arg_list;
    va_start(arg_list, argc);

    if (SetValue(val, argc, arg_list)) {
        va_end(arg_list);
        return true;
    }

    va_end(arg_list);
    return false;
}

bool Json::SetBool(const char *key, const char* field, bool value) {
    rapidjson::Value val;
    val.SetBool(value);

    if (SetValue(key, field, val)) {
        return true;
    }
    return false;
}

bool Json::SetBool(bool value, int argc, ...) {
    rapidjson::Value val;
    val.SetBool(value);

    va_list arg_list;
    va_start(arg_list, argc);

    if (SetValue(val, argc, arg_list)) {
        va_end(arg_list);
    }

    va_end(arg_list);
    return false;
}

bool Json::SetFloat(const char *key, const char* field, float value) {
    rapidjson::Value val;
    val.SetDouble(value);

    if (SetValue(key, field, val)) {
        return true;
    }
    return false;
}

bool Json::SetFloat(float value, int argc, ...) {
    rapidjson::Value val;
    val.SetDouble(value);

    va_list arg_list;
    va_start(arg_list, argc);

    if (SetValue(val, argc, arg_list)) {
        va_end(arg_list);
        return true;
    }

    va_end(arg_list);
    return false;
}

bool Json::SetObject(const char *key, const char* field) {
    rapidjson::Value val(rapidjson::kObjectType);

    if (SetValue(key, field, val)) {
        return true;
    }
    return false;
}

bool Json::SetObject(int argc, ...) {
    rapidjson::Value val(rapidjson::kObjectType);

    va_list arg_list;
    va_start(arg_list, argc);

    if (SetValue(val, argc, arg_list)) {
        va_end(arg_list);
        return true;
    }

    va_end(arg_list);
    return false;
}

bool Json::AddString(const std::string& name, const std::string& value) {
    rapidjson::Value val;
    rapidjson::Value name_val;
    val.SetString(value.c_str(), value.size(), document_.GetAllocator());
    name_val.SetString(name.c_str(), name.size(), document_.GetAllocator());

    if (AddMember(name_val, val)) {
        return true;
    }
    return false;
}

bool Json::AddString(const char *key, const char* field, const std::string& name, const std::string& value) {
    rapidjson::Value val;
    rapidjson::Value name_val;
    val.SetString(value.c_str(), value.size(), document_.GetAllocator());
    name_val.SetString(name.c_str(), name.size(), document_.GetAllocator());

    if (AddMember(key, field, name_val, val)) {
        return true;
    }
    return false;
}

bool Json::AddString(const std::string& name, const std::string& value, int argc, ...) {
    rapidjson::Value val;
    rapidjson::Value name_val;
    val.SetString(value.c_str(), value.size(), document_.GetAllocator());
    name_val.SetString(name.c_str(), name.size(), document_.GetAllocator());

    va_list arg_list;
    va_start(arg_list, argc);

    if (AddMember(name_val, val, argc, arg_list)) {
        va_end(arg_list);
        return true;
    }

    va_end(arg_list);
    return false;
}

bool Json::AddInt(const std::string& name, int value) {
    rapidjson::Value val;
    rapidjson::Value name_val;
    val.SetInt(value);
    name_val.SetString(name.c_str(), name.size(), document_.GetAllocator());

    if (AddMember(name_val, val)) {
        return true;
    }
    return false;
}


bool Json::AddInt(const char *key, const char* field, const std::string& name, int value) {
    rapidjson::Value val;
    rapidjson::Value name_val;
    val.SetInt(value);
    name_val.SetString(name.c_str(), name.size(), document_.GetAllocator());

    if (AddMember(key, field, name_val, val)) {
        return true;
    }
    return false;
}

bool Json::AddInt(const std::string& name, int value, int argc, ...) {
    rapidjson::Value val;
    rapidjson::Value name_val;
    val.SetInt(value);
    name_val.SetString(name.c_str(), name.size(), document_.GetAllocator());

    va_list arg_list;
    va_start(arg_list, argc);

    if (AddMember(name_val, val, argc, arg_list)) {
        va_end(arg_list);
        return true;
    }

    va_end(arg_list);
    return false;
}

bool Json::AddBool(const std::string& name, bool value) {
    rapidjson::Value val;
    rapidjson::Value name_val;
    val.SetBool(value);
    name_val.SetString(name.c_str(), name.size(), document_.GetAllocator());

    if (AddMember(name_val, val)) {
        return true;
    }
    return false;
}


bool Json::AddBool(const char *key, const char* field, const std::string& name, bool value) {
    rapidjson::Value val;
    rapidjson::Value name_val;
    val.SetBool(value);
    name_val.SetString(name.c_str(), name.size(), document_.GetAllocator());

    if (AddMember(key, field, name_val, val)) {
        return true;
    }
    return false;
}

bool Json::AddBool(const std::string& name, bool value, int argc, ...) {
    rapidjson::Value val;
    rapidjson::Value name_val;
    val.SetBool(value);
    name_val.SetString(name.c_str(), name.size(), document_.GetAllocator());

    va_list arg_list;
    va_start(arg_list, argc);

    if (AddMember(name_val, val, argc, arg_list)) {
        va_end(arg_list);
    }

    va_end(arg_list);
    return false;
}

bool Json::AddFloat(const std::string& name, float value) {
    rapidjson::Value val;
    rapidjson::Value name_val;
    val.SetDouble(value);
    name_val.SetString(name.c_str(), name.size(), document_.GetAllocator());

    if (AddMember(name_val, val)) {
        return true;
    }
    return false;
}

bool Json::AddFloat(const char *key, const char* field, const std::string& name, float value) {
    rapidjson::Value val;
    rapidjson::Value name_val;
    val.SetDouble(value);
    name_val.SetString(name.c_str(), name.size(), document_.GetAllocator());

    if (AddMember(key, field, name_val, val)) {
        return true;
    }
    return false;
}

bool Json::AddFloat(const std::string& name, float value, int argc, ...) {
    rapidjson::Value val;
    rapidjson::Value name_val;
    val.SetDouble(value);
    name_val.SetString(name.c_str(), name.size(), document_.GetAllocator());

    va_list arg_list;
    va_start(arg_list, argc);

    if (AddMember(name_val, val, argc, arg_list)) {
        va_end(arg_list);
        return true;
    }

    va_end(arg_list);
    return false;
}

bool Json::AddObject(const std::string& name) {
    rapidjson::Value val(rapidjson::kObjectType);
    rapidjson::Value name_val;
    name_val.SetString(name.c_str(), name.size(), document_.GetAllocator());

    if (AddMember(name_val, val)) {
        return true;
    }
    return false;
}

bool Json::AddObject(const char *key, const char* field, const std::string& name) {
    rapidjson::Value val(rapidjson::kObjectType);
    rapidjson::Value name_val;
    name_val.SetString(name.c_str(), name.size(), document_.GetAllocator());

    if (AddMember(key, field, name_val, val)) {
        return true;
    }
    return false;
}

bool Json::AddObject(const std::string& name, int argc, ...) {
    rapidjson::Value val(rapidjson::kObjectType);
    rapidjson::Value name_val;
    name_val.SetString(name.c_str(), name.size(), document_.GetAllocator());

    va_list arg_list;
    va_start(arg_list, argc);

    if (AddMember(name_val, val, argc, arg_list)) {
        va_end(arg_list);
        return true;
    }

    va_end(arg_list);
    return false;
}

bool Json::GetValue(const char* key, const char* field, rapidjson::Value& val) {
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

bool Json::GetValue(rapidjson::Value &val, int argc, va_list& arg_list) {
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

bool Json::SetValue(const char* key, const char* field, rapidjson::Value& val) {
    rapidjson::Value::MemberIterator it;
    if (document_.MemberEnd() != (it = document_.FindMember(key))) {
        if (field) {
            if (document_.MemberEnd() != (it = it->value.FindMember(field))) {
                it->value = val;
                return true;
            }
        } else {
            it->value = val;
            return true;
        }
    }

    return false;
}

bool Json::SetValue(rapidjson::Value &val, int argc, va_list& arg_list) {
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

    it->value = val;
    return true;
}

bool Json::AddMember(rapidjson::Value &name, rapidjson::Value &val) {
    document_.AddMember(name, val, document_.GetAllocator());
    return true;
}

bool Json::AddMember(const char *key, const char *field, rapidjson::Value &name, rapidjson::Value &val) {
    rapidjson::Value::MemberIterator it;
    if (document_.MemberEnd() != (it = document_.FindMember(key))) {
        if (field) {
            if (document_.MemberEnd() != (it = it->value.FindMember(field))) {
                it->value.AddMember(name, val, document_.GetAllocator());
                return true;
            }
        } else {
            it->value.AddMember(name, val, document_.GetAllocator());
            return true;
        }
    }

    return false;
}

bool Json::AddMember(rapidjson::Value &name, rapidjson::Value &val, int argc, va_list &arg_list) {
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

    it->value.AddMember(name, val, document_.GetAllocator());
    return true;
}

bool Json::Array2Vector(const rapidjson::Value &array, std::vector<int> &vector) {
    if (array.IsArray()) {
        for (uint32_t i = 0; i < array.Size(); ++i) {
            if (array[i].IsInt()) {
                vector.push_back(array[i].GetInt());
            } else {
                return false;
            }
        }
        
        return true;
    }

    return false;
}

bool Json::Array2Vector(const rapidjson::Value &array, std::vector<std::string> &vector) {
    if (array.IsArray()) {
        for (uint32_t i = 0; i < array.Size(); ++i) {
            if (array[i].IsString()) {
                vector.push_back(array[i].GetString());
            } else {
                return false;
            }
        }
        
        return true;
    }

    return false;
}

bool Json::Array2Vector(const rapidjson::Value &array, std::vector<float> &vector) {
    if (array.IsArray()) {
        for (uint32_t i = 0; i < array.Size(); ++i) {
            if (array[i].IsDouble()) {
                vector.push_back(array[i].GetDouble());
            } else {
                return false;
            }
        }
        
        return true;
    }

    return false;
}

bool Json::Array2Vector(const rapidjson::Value &array, std::vector<bool> &vector) {
    if (array.IsArray()) {
        for (uint32_t i = 0; i < array.Size(); ++i) {
            if (array[i].IsBool()) {
                vector.push_back(array[i].GetBool());
            } else {
                return false;
            }
        }
        
        return true;
    }

    return false;
}

