/*
 * =====================================================================================
 *
 *       Filename:  Config.h
 *
 *    Description:  Config
 *
 *        Version:  1.0
 *        Created:  06/02/2015 11:13:31 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef CONFIG_H_KLOREHKC
#define CONFIG_H_KLOREHKC

#include <string>
#include "rapidjson/document.h"

class ConfigFile {
    public:
        ConfigFile() = default;
        virtual ~ConfigFile() = default;

        bool LoadFile(const char* file_name);

        bool GetString(const char* key, const char* field, std::string& value);
        std::string GetStringDefault(const char* key, const char* field = NULL, const char* default_value = "");
        std::string GetStringVA(const char* default_value, int argc, ...);

        bool GetInt(const char* key, const char* field, int& value);
        int GetIntDefault(const char* key, const char* field = NULL, int default_value = 0);
        int GetIntVA(int default_value, int argc, ...);

        bool GetBool(const char* key, const char* field, bool& value);
        bool GetBoolDefault(const char* key, const char* field = NULL, bool default_value = false);
        bool GetBoolVA(bool default_value, int argc, ...);

        bool GetFloat(const char* key, const char* field, float& value);
        float GetFloatDefault(const char* key, const char* field = NULL, float default_value = 0.0f);
        float GetFloatVA(float default_value, int argc, ...);

    private:
        bool GetValue(const char* key, const char* field, rapidjson::Value& val);
        bool GetValueNA(rapidjson::Value& val, int argc, va_list& arg_list);

        rapidjson::Document document_;
};

#endif /* end of include guard: CONFIG_H_KLOREHKC */
