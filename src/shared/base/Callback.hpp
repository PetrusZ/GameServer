/*
 * =====================================================================================
 *
 *       Filename:  Callback.hpp
 *
 *    Description:  Callback Series Template
 *
 *        Version:  1.0
 *        Created:  11/03/2015 03:58:14 PM
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef CALLBACK_HPP_SMIPZOAB
#define CALLBACK_HPP_SMIPZOAB

#include <vector>

class AsyncQueryResult;
typedef std::vector<AsyncQueryResult> QueryResultVector;

class CallbackBase {
    public:
        virtual ~CallbackBase() {}
        virtual void execute() = 0;
};

class CallbackFunctionP0 : public CallbackBase {
    typedef void (*callback_function)();

    public:
        CallbackFunctionP0(callback_function function) : callback_(function) {}
        void operator()() { return (callback_)(); }
        void execute() { return operator()(); }

    private:
        callback_function callback_;

};

template <typename P1>
class CallbackFunctionP1 : public CallbackBase {
    typedef void (*callback_function)(P1);

    public:
        CallbackFunctionP1(callback_function function, P1 p1) : callback_(function), p1_(p1) {}
        void operator()() { return (callback_)(p1_); }
        void execute() { return operator()(); }

    private:
        callback_function callback_;
        P1 p1_;
};

template <typename P1, typename P2>
class CallbackFunctionP2 : public CallbackBase {
    typedef void (*callback_function)(P1, P2);

    public:
        CallbackFunctionP2(callback_function function, P1 p1, P2 p2) : callback_(function),
            p1_(p1), p2_(p2) {}
        void operator()() { return (callback_)(p1_, p2_); }
        void execute() { return operator()(); }

    private:
        callback_function callback_;
        P1 p1_;
        P2 p2_;
};

template <typename P1, typename P2, typename P3>
class CallbackFunctionP3 : public CallbackBase {
    typedef void (*callback_function)(P1, P2, P3);

    public:
        CallbackFunctionP3(callback_function function, P1 p1, P2 p2, P3 p3) : callback_(function),
            p1_(p1), p2_(p2), p3_(p3) {}
        void operator()() { return (callback_)(p1_, p2_, p3_); }
        void execute() { return operator()(); }

    private:
        callback_function callback_;
        P1 p1_;
        P2 p2_;
        P3 p3_;
};

template <typename P1, typename P2, typename P3, typename P4>
class CallbackFunctionP4 : public CallbackBase {
    typedef void (*callback_function)(P1, P2, P3, P4);

    public:
        CallbackFunctionP4(callback_function function, P1 p1, P2 p2, P3 p3, P4 p4) : callback_(function),
            p1_(p1), p2_(p2), p3_(p3), p4_(p4) {}
        void operator()() { return (callback_)(p1_, p2_, p3_, p4_); }
        void execute() { return operator()(); }

    private:
        callback_function callback_;
        P1 p1_;
        P2 p2_;
        P3 p3_;
        P4 p4_;
};

template <typename Class>
class CallbackP0 : public CallbackBase{
    typedef void (Class::*Method)();

    public:
        CallbackP0(Class* class_instance, Method method) : obj_(class_instance), func_(method) {}
        void operator()() { return (obj_->*func_()); }
        void execute() { return operator()(); }

    private:

        Class* obj_;
        Method func_;
};

template <typename Class, typename P1>
class CallbackP1 : public CallbackBase{
    typedef void (Class::*Method)(P1);

    public:
        CallbackP1(Class* class_instance, Method method, P1 p1) : obj_(class_instance), func_(method),
            p1_(p1) {}
        void operator()() { return (obj_->*func_(p1_)); }
        void execute() { return operator()(); }

    private:
        Class* obj_;
        Method func_;
        P1 p1_;
};

template <typename Class, typename P1, typename P2>
class CallbackP2 : public CallbackBase{
    typedef void (Class::*Method)(P1, P2);

    public:
        CallbackP2(Class* class_instance, Method method, P1 p1, P2 p2) : obj_(class_instance), func_(method),
            p1_(p1), p2_(p2) {}
        void operator()() { return (obj_->*func_(p1_, p2_)); }
        void execute() { return operator()(); }

    private:
        Class* obj_;
        Method func_;
        P1 p1_;
        P2 p2_;
};

template <typename Class, typename P1, typename P2, typename P3>
class CallbackP3 : public CallbackBase{
    typedef void (Class::*Method)(P1, P2, P3);

    public:
        CallbackP3(Class* class_instance, Method method, P1 p1, P2 p2, P3 p3) : obj_(class_instance), func_(method),
            p1_(p1), p2_(p2), p3_(p3) {}
        void operator()() { return (obj_->*func_(p1_, p2_, p3_)); }
        void execute() { return operator()(); }

    private:
        Class* obj_;
        Method func_;
        P1 p1_;
        P2 p2_;
        P3 p3_;
};

template <typename Class, typename P1, typename P2, typename P3, typename P4>
class CallbackP4 : public CallbackBase{
    typedef void (Class::*Method)(P1, P2, P3, P4);

    public:
        CallbackP4(Class* class_instance, Method method, P1 p1, P2 p2, P3 p3, P4 p4) : obj_(class_instance), func_(method),
            p1_(p1), p2_(p2), p3_(p3), p4_(p4) {}
        void operator()() { return (obj_->*func_(p1_, p2_, p3_, p4_)); }
        void execute() { return operator()(); }

    private:
        Class* obj_;
        Method func_;
        P1 p1_;
        P2 p2_;
        P3 p3_;
        P4 p4_;
};

class QueryCallbackBase {
    public:
        virtual ~QueryCallbackBase() {}
        virtual void Run(QueryResultVector& result) = 0;
};

template <typename Class>
class QueryCallbackP0 : public QueryCallbackBase {
    typedef void (Class::*Method)(QueryResultVector& result);

    public:
        QueryCallbackP0(Class* class_instance, Method method) : QueryCallbackBase(), obj_(class_instance), func_(method) {}
        virtual ~QueryCallbackP0() {} 
        void Run(QueryResultVector& result) { (obj_->*func_)(result); }

    private:
        Class* obj_;
        Method func_;
};

template <typename Class, typename P1>
class QueryCallbackP1 : public QueryCallbackBase {
    typedef void (Class::*Method)(QueryResultVector& result);

    public:
        QueryCallbackP1(Class* class_instance, Method method, P1 p1) : QueryCallbackBase(), obj_(class_instance), func_(method),
            p1_(p1) {}
        virtual ~QueryCallbackP1() {} 
        void Run(QueryResultVector& result) { (obj_->*func_)(result, p1_); }

    private:
        Class* obj_;
        Method func_;
        P1 p1_;
};

template <typename Class, typename P1, typename P2>
class QueryCallbackP2 : public QueryCallbackBase {
    typedef void (Class::*Method)(QueryResultVector& result);

    public:
        QueryCallbackP2(Class* class_instance, Method method, P1 p1, P2 p2) : QueryCallbackBase(), obj_(class_instance), func_(method),
            p1_(p1), p2_(p2) {}
        virtual ~QueryCallbackP2() {} 
        void Run(QueryResultVector& result) { (obj_->*func_)(result, p1_, p2_); }

    private:
        Class* obj_;
        Method func_;
        P1 p1_;
        P2 p2_;
};

template <typename Class, typename P1, typename P2, typename P3>
class QueryCallbackP3 : public QueryCallbackBase {
    typedef void (Class::*Method)(QueryResultVector& result);

    public:
        QueryCallbackP3(Class* class_instance, Method method, P1 p1, P2 p2, P3 p3) : QueryCallbackBase(), obj_(class_instance), func_(method),
            p1_(p1), p2_(p2), p3_(p3) {}
        virtual ~QueryCallbackP3() {} 
        void Run(QueryResultVector& result) { (obj_->*func_)(result, p1_, p2_, p3_); }

    private:
        Class* obj_;
        Method func_;
        P1 p1_;
        P2 p2_;
        P3 p3_;
};

template <typename Class, typename P1, typename P2, typename P3, typename P4>
class QueryCallbackP4 : public QueryCallbackBase {
    typedef void (Class::*Method)(QueryResultVector& result);

    public:
        QueryCallbackP4(Class* class_instance, Method method, P1 p1, P2 p2, P3 p3, P4 p4) : QueryCallbackBase(), obj_(class_instance), func_(method),
            p1_(p1), p2_(p2), p3_(p3), p4_(p4) {}
        virtual ~QueryCallbackP4() {} 
        void Run(QueryResultVector& result) { (obj_->*func_)(result, p1_, p2_, p3_, p4_); }

    private:
        Class* obj_;
        Method func_;
        P1 p1_;
        P2 p2_;
        P3 p3_;
        P4 p4_;
};

#endif /* end of include guard: CALLBACK_HPP_SMIPZOAB */
