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

        bool Append(const std::string &data);
        bool Close();
        bool Flush();

    private:
        std::string file_name_;
        FILE* file_;
};


#endif /* end of include guard: WRITABLEFILE_H_EIVNP1X7 */
