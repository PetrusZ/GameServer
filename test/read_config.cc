/*
 * =====================================================================================
 *
 *       Filename:  read_confg.cc
 *
 *    Description:  read_confg
 *
 *        Version:  1.0
 *        Created:  06/02/2015 02:43:06 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */

#include <cstdio>
#include "../config/Config.h"

int main(int argc, char *argv[])
{
    ConfigFile config;
    bool result = config.LoadFile("config.json");
    printf("load result: %d\n", result);

    int intval;
    std::string strval;
    bool boolval;
    float fltval;

    if (config.GetString("server_name", NULL, strval)) {
        printf("server_name: %s\n", strval.c_str());
    }
    /*
    printf("ip: %s\n", config.GetString("ip").c_str());
    printf("port: %d\n", config.GetInt("port"));
    printf("author: %s\n", config.GetString("detail", "author").c_str());
    printf("email: %s\n", config.GetString("detail", "email").c_str());
    printf("is_released: %d\n", config.GetBool("detail", "is_released"));
    printf("version: %f\n", config.GetFloat("detail", "version"));
    */

    printf("default test: %s\n", config.GetStringDefault("test").c_str());

    printf("VA test: %s\n", config.GetStringVA("failed", 4, "test1","test2","test3","test4").c_str());

    return 0;
}
