/*
 * =====================================================================================
 *
 *       Filename:  Json.h
 *
 *    Description:  rappidjson封装
 *
 *        Version:  1.0
 *        Created:  12/02/2015 02:57:15 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */

#ifndef JSON_H_N6IE3Y1C
#define JSON_H_N6IE3Y1C

#include <cstdarg>
#include <typeinfo>

#include "Common.h"

#include "rapidjson/document.h"
// XXX
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

class Json {
    public:
        Json();
        virtual ~Json() = default;

        bool Parse(const std::string& json);
        bool Parse(char* json);
        bool Parse(const char* json);
        bool ParseInsitu(char* json);

        std::string PrettyFormat();
        std::string Format();

        // Get Function

        bool GetString(const char* key, const char* field, std::string& value);
        bool GetString(std::string& value, int argc, ...);

        bool GetInt(const char* key, const char* field, int& value);
        bool GetInt(int& value, int argc, ...);

        bool GetBool(const char* key, const char* field, bool& value);
        bool GetBool(bool& value, int argc, ...);

        bool GetFloat(const char* key, const char* field, float& value);
        bool GetFloat(float& value, int argc, ...);

        template <typename type> bool GetArray(const char* key, const char* field, std::vector<type>& value);
        template <typename type> bool GetArray(std::vector<type>& value, int argc, ...);

        // Set Function

        bool SetString(const char* key, const char* field, const std::string& value);
        bool SetString(const std::string& value, int argc, ...);

        bool SetInt(const char* key, const char* field, int value);
        bool SetInt(int value, int argc, ...);

        bool SetBool(const char* key, const char* field, bool value);
        bool SetBool(bool value, int argc, ...);

        bool SetFloat(const char* key, const char* field, float value);
        bool SetFloat(float value, int argc, ...);

        bool SetObject(const char* key, const char* field);
        bool SetObject(int argc, ...);

        template <typename type> bool SetArray(const std::vector<type>& value);
        template <typename type> bool SetArray(const char* key, const char* field, const std::vector<type>& value);
        template <typename type> bool SetArray(const std::vector<type>& value, int argc, ...);

        // Add Function

        bool AddString(const std::string& name, const std::string& value);
        bool AddString(const char* key, const char* field, const std::string& name, const std::string& value);
        bool AddString(const std::string& name, const std::string& value, int argc, ...);

        bool AddInt(const std::string& name, int value);
        bool AddInt(const char* key, const char* field, const std::string& name, int value);
        bool AddInt(const std::string& name, int value, int argc, ...);

        bool AddBool(const std::string& name, bool value);
        bool AddBool(const char* key, const char* field, const std::string& name, bool value);
        bool AddBool(const std::string& name, bool value, int argc, ...);

        bool AddFloat(const std::string& name, float value);
        bool AddFloat(const char* key, const char* field, const std::string& name, float value);
        bool AddFloat(const std::string& name, float value, int argc, ...);

        bool AddObject(const std::string& name);
        bool AddObject(const char* key, const char* field, const std::string& name);
        bool AddObject(const std::string& name, int argc, ...);

        template <typename type> bool AddArray(const std::string& name, const std::vector<type>& value);
        template <typename type> bool AddArray(const char* key, const char* field, const std::string& name, const std::vector<type>& value);
        template <typename type> bool AddArray(const std::string& name, const std::vector<type>& value, int argc, ...);

    private:
        bool GetValue(const char* key, const char* field, rapidjson::Value& val);
        bool GetValue(rapidjson::Value& val, int argc, va_list& arg_list);

        bool SetValue(const char* key, const char* field, rapidjson::Value& val);
        bool SetValue(rapidjson::Value& val, int argc, va_list& arg_list);

        bool AddMember(rapidjson::Value& name, rapidjson::Value& val);
        bool AddMember(const char* key, const char* field, rapidjson::Value& name, rapidjson::Value& val);
        bool AddMember(rapidjson::Value& name, rapidjson::Value& val, int argc, va_list& arg_list);

        bool Array2Vector(const rapidjson::Value& array, std::vector<int>& vector);
        bool Array2Vector(const rapidjson::Value& array, std::vector<std::string>& vector);
        bool Array2Vector(const rapidjson::Value& array, std::vector<float>& vector);
        bool Array2Vector(const rapidjson::Value& array, std::vector<bool>& vector);

        rapidjson::Document document_;
};

template <typename type>
bool Json::GetArray(const char* key, const char* field, std::vector<type>& value) {
    rapidjson::Value val;
    if (GetValue(key, field, val) && val.IsArray()) {
        if (Array2Vector(val, value)) {
            return true;
        }

        return false;
    }

    return false;
}

template <typename type>
bool Json::GetArray(std::vector<type>& value, int argc, ...) {
    rapidjson::Value val;

    va_list arg_list;
    va_start(arg_list, argc);

    if (GetValue(val, argc, arg_list) && val.IsArray()) {
        va_end(arg_list);

        if (Array2Vector(val, value)) {
            return true;
        }

        return false;
    }

    va_end(arg_list);

    return false;
}

template <typename type>
bool Json::SetArray(const std::vector<type>& value) {
    document_.SetArray();
    for (uint32_t i = 0; i < value.size(); ++i) {
        document_.PushBack(value[i], document_.GetAllocator());
    }
    return true;
}


template <typename type>
bool Json::SetArray(const char* key, const char* field, const std::vector<type>& value) {
    rapidjson::Value val;
    if (SetValue(key, field, val)) {
        val.SetArray();
        for (uint32_t i = 0; i < value.size(); ++i) {
            val.PushBack(value[i], document_.GetAllocator());
        }
        return true;
    }
    return false;
}

template <typename type>
bool Json::SetArray(const std::vector<type>& value, int argc, ...) {
    rapidjson::Value val;

    va_list arg_list;
    va_start(arg_list, argc);

    if (SetValue(val, argc, arg_list)) {
        va_end(arg_list);

        val.SetArray();
        for (uint32_t i = 0; i < value.size(); ++i) {
            val.PushBack(value[i], document_.GetAllocator());
        }
        return true;
    }

    va_end(arg_list);

    return false;
}

template <typename type>
bool Json::AddArray(const std::string& name, const std::vector<type>& value) {
    rapidjson::Value val;
    rapidjson::Value name_val;
    name_val.SetString(name.c_str(), name.size(), document_.GetAllocator());

    val.SetArray();
    for (uint32_t i = 0; i < value.size(); ++i) {
        val.PushBack(value[i], document_.GetAllocator());
    }

    AddMember(name_val, val);
    return false;
}

template <typename type>
bool Json::AddArray(const char* key, const char* field, const std::string& name, const std::vector<type>& value) {
    rapidjson::Value name_val;
    name_val.SetString(name.c_str(), name.size(), document_.GetAllocator());

    rapidjson::Value val;
    val.SetArray();
    for (uint32_t i = 0; i < value.size(); ++i) {
        val.PushBack(value[i], document_.GetAllocator());
    }

    if (AddMember(key, field, name_val, val)) {
        return true;
    }
    return false;
}

template <typename type>
bool Json::AddArray(const std::string& name, const std::vector<type>& value, int argc, ...) {
    rapidjson::Value name_val;
    name_val.SetString(name.c_str(), name.size(), document_.GetAllocator());

    rapidjson::Value val;
    val.SetArray();
    for (uint32_t i = 0; i < value.size(); ++i) {
        val.PushBack(value[i], document_.GetAllocator());
    }

    va_list arg_list;
    va_start(arg_list, argc);
    if (AddMember(name_val, val, argc, arg_list)) {
        va_end(arg_list);

        return true;
    }

    va_end(arg_list);

    return false;
}

#endif /* end of include guard: JSON_H_N6IE3Y1C */
