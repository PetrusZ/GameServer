/*
 * =====================================================================================
 *
 *       Filename:  WritableFile.cc
 *
 *    Description:  WritableFile
 *
 *        Version:  1.0
 *        Created:  03/30/2015 02:32:17 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "WritableFile.h"

WritableFile::WritableFile(const std::string file_name, FILE* file) : file_name_(file_name), file_(file) {

}

WritableFile::~WritableFile() {
    if (file_)  {
        flockfile(file_);
        fflush(file_);
        fclose(file_);
        funlockfile(file_);
    }
}

bool WritableFile::Append(const std::string &data) {
    flockfile(file_);
    if (fwrite(data.data(), 1, data.size(), file_) != data.size()) {
        funlockfile(file_);
        return false;
    } else {
        funlockfile(file_);
        return true;
    }
}

bool WritableFile::Close() {
    flockfile(file_);
    fflush(file_);
    if (0 != fclose(file_)) {
        funlockfile(file_);
        return false;
    } else {
        funlockfile(file_);
        return true;
    }
}

bool WritableFile::Flush() {
    flockfile(file_);
    if (0 != fflush(file_)) {
        funlockfile(file_);
        return false;
    } else {
        funlockfile(file_);
        return true;
    }
}
