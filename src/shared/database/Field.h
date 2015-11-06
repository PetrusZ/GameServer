/*
 * =====================================================================================
 *
 *       Filename:  Field.h
 *
 *    Description:  Field
 *
 *        Version:  1.0
 *        Created:  11/03/2015 05:51:44 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef FIELD_H_YQJ5TCAL
#define FIELD_H_YQJ5TCAL

#include "common/Common.h"

class Field {
    public:
        void SetValue(char* value) { value_ = value; }

        const char* GetString() { return value_; }
        float GetFloat() { return value_ ? static_cast<float>(atof(value_)) : 0; }
        bool  GetBool() { return value_ ? atoi(value_) > 0 : false; }
        uint8_t GetUInt8() { return value_ ? static_cast<uint8_t>(atol(value_)) : 0; }
        int8_t GetInt8() { return value_ ? static_cast<int8_t>(atol(value_)) : 0; }
        uint16_t GetUInt16() { return value_ ? static_cast<uint16_t>(atol(value_)) : 0; }
        int16_t GetInt16() { return value_ ? static_cast<int16_t>(atol(value_)) : 0; }
        uint32_t GetUInt32() { return value_ ? static_cast<uint32_t>(atol(value_)) : 0; }
        int32_t GetInt32() { return value_ ? static_cast<int32_t>(atol(value_)) : 0; }
        uint64_t GetUInt64() { return value_ ? static_cast<uint64_t>(atoll(value_)) : 0; }
        int64_t GetInt64() { return value_ ? static_cast<int64_t>(atoll(value_)) : 0; }

    private:
        char* value_;
};

#endif /* end of include guard: FIELD_H_YQJ5TCAL */

