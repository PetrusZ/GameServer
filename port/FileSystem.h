/*
 * =====================================================================================
 *
 *       Filename:  FileSystem.h
 *
 *    Description:  FileSystem
 *
 *        Version:  1.0
 *        Created:  03/30/2015 11:16:25 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */

#ifndef FILESYSTEM_H_6HRNOJVP
#define FILESYSTEM_H_6HRNOJVP

#include <string>
#include "base/Singleton.hpp"
#include "WritableFile.h"

class FileSystem : public Singleton<FileSystem> {
    public:
        FileSystem() = default;
        ~FileSystem() = default;

        FileSystem(const FileSystem&) = delete;
        FileSystem& operator=(const FileSystem&) = delete;

        bool NewWritableFile(const std::string& file_name, WritableFile** result);

        bool FileExists(const std::string file_name);

        /*
         * similar with mkdir -p, but mkdir based on dirname.
         *
         * eg: dir_path = /foo/bar/some  equal mkdir -p /foo/bar
         *     dir_path = /foo/bar/some/ equal mkdir -p /foo/bar/some
         */
        bool MakeDirRecursive(const std::string dir_path);
};

#define sFileSystem FileSystem::getSingleton()

#endif /* end of include guard: FILESYSTEM_H_6HRNOJVP */
