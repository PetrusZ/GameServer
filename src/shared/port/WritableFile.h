/*
 * =====================================================================================
 *
 *       Filename:  WritableFile.h
 *
 *    Description:  WritableFile
 *
 *        Version:  1.0
 *        Created:  03/30/2015 02:25:28 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */

#ifndef WRITABLEFILE_H_EIVNP1X7
#define WRITABLEFILE_H_EIVNP1X7

#include <string>
#include <cstdio>

class WritableFile {
    public:
        WritableFile(const std::string filename, FILE* file);
        virtual ~WritableFile();

        std::string GetName() { return file_name_; }

        bool Append(const std::string &data);
        bool Close();
        bool Flush();

    private:
        WritableFile(const WritableFile&) = delete;
        WritableFile& operator=(const WritableFile&) = delete;

        std::string file_name_;
        FILE* file_;
};


#endif /* end of include guard: WRITABLEFILE_H_EIVNP1X7 */
