/*
 * =====================================================================================
 *
 *       Filename:  Status.h
 *
 *    Description:  统一status封装
 *
 *        Version:  1.0
 *        Created:  2015年03月26日 00时07分56秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://www.codeplayer.org
 *
 * =====================================================================================
 */
#ifndef STATUS_H_SGWFZ9YU
#define STATUS_H_SGWFZ9YU

#include <string>

class Status {
    public:
        Status() : status_(NULL) { }

        Status(const Status& s) {
            status_ = (s.status_ == NULL) ? NULL : CopyStatus(s.status_);
        }

        void operator=(const Status &s) {
            if (status_ != s.status_) {
                delete[] status_;
                status_ = (s.status_ == NULL) ? NULL : CopyStatus(s.status_);
            }
        };

        virtual ~Status() { delete[] status_; }

        static Status OK() { return Status(); }

        static Status IOError(const std::string& msg, const std::string& msg2 = "") {
            return Status(kIOError, msg, msg2);
        };

        bool ok() { return (status_ == NULL); }

        std::string ToString() const;

    private:
        // OK status has a NULL state_.  Otherwise, state_ is a new[] array
        // of the following form:
        //    state_[0..3] == length of message
        //    state_[4]    == code
        //    state_[5..]  == message
        const char* status_;

        enum Code {
            kOk = 0,
            kIOError = 1
        };

        Code code() const { return (status_ == NULL) ? kOk : static_cast<Code>(status_[4]); }

        const char* CopyStatus(const char* status);
        Status(Code code, const std::string& prefix, const std::string& msg);
};


#endif /* end of include guard: STATUS_H_SGWFZ9YU */
