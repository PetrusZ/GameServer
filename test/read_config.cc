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

#include <vector>
#include <cstdio>
#include "../src/shared/config/Config.h"
#include "../src/shared/common/Common.h"
#include "../src/shared/common/Json.h"

int main(int argc, char *argv[])
{
    sLogger.Init("read_config", false);

    ConfigFile config;
    bool result = config.LoadFile("config.json");
    printf("load result: %d\n", result);

    if (!result) {
        exit(-1);
    }

    int intval;
    std::string strval;
    bool boolval;
    float fltval;

    if (config.GetString("server_name", NULL, strval)) {
        printf("server_name: %s\n", strval.c_str());
    }

    /*
    printf("default test: %s\n", config.GetStringDefault("test").c_str());

    printf("VA test: %s\n", config.GetStringVA("failed", 4, "test1","test2","test3","test4").c_str());

    printf("detail array 2: %s\n", config.GetStringVA("failed", 3, "detail" "array", 2).c_str());

    printf("key array b: %d\n", config.GetIntVA(0, 2, "key_array", "b"));

    // TODO
    printf("num array 2: %d\n", config.GetIntVA(0, 2, "num_array", "2"));
    */

    Json* json = new Json();

    std::vector<int> vec = { 1, 2, 3, 4};
    assert(json->AddObject("Test1"));
    assert(json->AddObject("Test1", NULL, "a"));
    assert(json->AddObject("Test1", "a", "b"));
    assert(json->AddArray<int>("c", vec, 3, "Test1", "a", "b"));

    assert(json->AddFloat("float", 3.14f));
    assert(json->SetInt("Test1", "a", 1));
    puts(json->PrettyFormat().c_str());
    float value;
    assert(json->GetFloat("float", NULL, value));
    LOG_STDOUT("float %f\n", value);

    delete json;

    return 0;
}
