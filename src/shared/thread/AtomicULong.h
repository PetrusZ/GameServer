/*
 * =====================================================================================
 *
 *       Filename:  AtomicULong.h
 *
 *    Description:  AtomicULong
 *
 *        Version:  1.0
 *        Created:  11/05/2015 05:30:56 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */

#ifndef ATOMICULONG_H_ZQ324AVN
#define ATOMICULONG_H_ZQ324AVN

class AtomicULong {
    public:
        AtomicULong() { value_ = 0; }

        AtomicULong(unsigned long init_value) : value_(init_value) { }

        /*
         * brief: lockless threadsafe set operation on the contained value
         *
         * Paramters
         *   unsigned long val  -  value to set
         *
         * Return values
         *   Returns the initial value contained
         */
        unsigned long SetVal(unsigned long new_value);

        /*
         * brief: non-threadsafe get operation on the contained value
         *
         *
         * Paramters
         *   None
         *
         * Return values
         *  Returns the value contained
         */
        unsigned long GetVal() { return value_; }

    protected:
        volatile unsigned long value_;

    private:
        AtomicULong(const AtomicULong&) = delete;
        AtomicULong& operator=(const AtomicULong&) = delete;
};

#endif /* end of include guard: ATOMICULONG_H_ZQ324AVN */
