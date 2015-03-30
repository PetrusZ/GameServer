/*
 * =====================================================================================
 *
 *       Filename:  FileSystem.cc
 *
 *    Description:  FileSystem
 *
 *        Version:  1.0
 *        Created:  03/30/2015 11:20:00 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */

#include "FileSystem.h"
#include <unistd.h>
#include <sys/stat.h>

bool FileSystem::NewWritableFile(const std::string &file_name, WritableFile **result) {
    FILE* fp = fopen(file_name.c_str(), "a");
    if (fp) {
        *result = new WritableFile(file_name, fp);
        return true;
    } else {
        return false;
    }
}

bool FileSystem::FileExists(const std::string file_name) {
    return 0 == access(file_name.c_str(), F_OK);
}

bool FileSystem::MakeDirRecursive(const std::string dir_path) {
    size_t separate_pos = 0;
    while ((separate_pos = dir_path.find("/", separate_pos)) != std::string::npos) {
        auto sub_path = dir_path.substr(0, separate_pos);
        if (0 != mkdir(sub_path.c_str(), 0755)) {
            return false;
        }
    }
    return true;
}
