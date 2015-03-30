/*
 * =====================================================================================
 *
 *       Filename:  Status.cc
 *
 *    Description:  Status
 *
 *        Version:  1.0
 *        Created:  03/30/2015 05:21:41 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "Status.h"
#include <cstring>
#include <cassert>

Status::Status(Code code, const std::string& msg, const std::string& msg2) {
    assert(code != kOk);

    const uint32_t len1 = msg.size();
    const uint32_t len2 = msg2.size();
    const uint32_t size = len1 + (len2 ? (2 + len2) : 0);

    char *status = new char[size + 5];
    memcpy(status, &size, sizeof(size));
    status[4] = static_cast<char>(code);
    memcpy(status + 5, msg.data(), len1);

    if (len2) {
        status[5 + len1] = ':';
        status[6 + len1] = ' ';
        memcpy(status + len1 + 7, msg2.data(), len2);
    }

    status_ = status;
}

const char* Status::CopyStatus(const char *status) {
    uint32_t size;
    memcpy(&size, status, sizeof(size));
    char* result = new char[size + 5];
    memcpy(result, status, size + 5);
    return result;
}

std::string Status::ToString() const {
    if (status_ == NULL) {
        return "OK";
    } else {
        const char* type;
        switch (Code()) {
            case kIOError:
                type = "IO Error: ";
                break;
            default:
                char tmp[30];
                snprintf(tmp, sizeof(tmp), "unknown code(%d): ", static_cast<int>(Code()));
                type = tmp;
        }

        std::string result(type);
        uint32_t len;
        memcpy(&len, status_, sizeof(len));
        result.append(status_ + 5, len);
        return result;
    }
}
