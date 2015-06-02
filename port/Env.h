/*
 * =====================================================================================
 *
 *       Filename:  Env.h
 *
 *    Description:  Env
 *
 *        Version:  1.0
 *        Created:  03/31/2015 11:45:39 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */

#ifndef ENV_H_CIE5HSSN
#define ENV_H_CIE5HSSN

#include "base/Singleton.hpp"
#include <sys/time.h>
#include <string>
#include <cstdint>

class Env : public Singleton<Env> {
    public:
        Env() {
            UpdateTime();
        }

        virtual ~Env() = default;

        uint64_t GetTid();

        void UpdateTime();
        uint64_t GetRealMSTime();
        uint64_t GetMSTime();
        uint32_t GetNowStamp();

        std::string GetDate();
        std::string GetTime();

        void Sleep(unsigned long time_ms);

    private:
        Env(const Env&) = delete;
        Env& operator=(const Env&) = delete;

        struct timeval tv_;
};

#define sEnv Env::getSingleton()

#endif /* end of include guard: ENV_H_CIE5HSSN */
