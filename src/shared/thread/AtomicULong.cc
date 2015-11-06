/*
 * =====================================================================================
 *
 *       Filename:  AtomicULong.cc
 *
 *    Description:  AtomicULong
 *
 *        Version:  1.0
 *        Created:  11/05/2015 05:44:46 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#include "AtomicULong.h"

unsigned long AtomicULong::SetVal(unsigned long new_value) {
    unsigned long ret = 0;

#ifdef __GNUC__
    ret = __sync_val_compare_and_swap(&value_, value_, new_value);
#else
#error Your platform (architecture and compiler) is NOT supported. Requires little endian architecture, and at least GCC 4.1
#endif

    return ret;
}
