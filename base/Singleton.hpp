/*
 * =====================================================================================
 *
 *       Filename:  Singleton.hpp
 *
 *    Description:  singleton
 *
 *        Version:  1.0
 *        Created:  2015年03月25日 22时24分47秒
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://www.codeplayer.org
 *
 * =====================================================================================
 */

#ifndef SINGLETON_HPP_JWRCISA4
#define SINGLETON_HPP_JWRCISA4

#include <assert.h>

template < class type > class Singleton {
    public:
		// Destructor
        ~Singleton() {
            this->singleton_ = 0;
        }

        static type& getSingleton() {if(!singleton_) new type(); return *singleton_};
        static type* getSingletonPtr() {if(!singleton_) new type(); return singleton_};

    protected:
		// Constructor
        Singleton() {
            assert(singleton_ == 0);
            this->singleton_ = static_cast<type*>(this);
        }

        static type* singleton_;
}

template < class type > type * Singleton < type > :: singleton_ = 0;

#endif /* end of include guard: SINGLETON_HPP_JWRCISA4 */
