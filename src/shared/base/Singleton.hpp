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

#if 1
/*
 * Meyers Singleton
 * Thread safe(C++11 only)
 * Can't call getSingleton before main function
 */
template <typename Type> class Singleton {
    public:
        static Type& getSingleton() {
            static Type singleton;
            return singleton;
        }

};
#endif

#if 0
/*
 * local static version
 * Thread safe
 * Can call getSingleton before main function
 */
template <typename Type> class Singleton {
    private:
        class Creator {
            public:
                Creator() : value_(new Type()) { }
                ~Creator() { delete value_; }

                Type& get_value() { return *value_; }

            private:
                Type* value_;
        };

    public:
        static Type& getSingleton() {
            static Creator creator;
            return creator.get_value();
        }

    private:
        class Dummy {
            public:
                Dummy() { Singleton<Type>::getSingleton(); }
        };

        static Dummy dummy_;
};

template <typename Type> typename Singleton<Type>::Dummy Singleton<Type>::dummy_;
#endif


#if 0
/*
 * Single thread version
 */
template < typename Type > class Singleton {
    public:
		// Destructor
        ~Singleton() {
            this->singleton_ = 0;
        }

        static Type& getSingleton() {if(!singleton_) new Type(); return *singleton_;}
        // static Type* getSingletonPtr() {if(!singleton_) new Type(); return singleton_;}

    protected:
		// Constructor
        Singleton() {
            assert(singleton_ == 0);
            this->singleton_ = static_cast<Type*>(this);
        }

        static Type* singleton_;
};

template < typename Type > Type * Singleton < Type > :: singleton_ = 0;
#endif

#endif /* end of include guard: SINGLETON_HPP_JWRCISA4 */
