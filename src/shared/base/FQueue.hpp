/*
 * =====================================================================================
 *
 *       Filename:  FQueue.hpp
 *
 *    Description:  自带锁Queue
 *
 *        Version:  1.0
 *        Created:  11/04/2015 11:32:12 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Petrus (), silencly07@gmail.com
 *   Organization:  http://codeplayer.org
 *
 * =====================================================================================
 */
#ifndef FQUEUE_HPP_SO4FYVJU
#define FQUEUE_HPP_SO4FYVJU

#include <cstdint>

#include "thread/Mutex.h"

template <typename T>
class FQueue {
    public:
        FQueue() { first_ = last_ = NULL; size_ = 0; }

        uint32_t get_size() {
            mutex_.Lock();
            uint32_t ret_val = size_;
            mutex_.UnLock();
            return ret_val;
        }

        void push(T& item) {
            Node* p = new Node;
            p->value_ = item;
            p->next_  = NULL;

            mutex_.Lock();
            if (NULL != last_) {
                // have some tiems
                last_->next_ = (Node*)p;
                last_ = p;
                ++size_;
                
            } else {
                // first item
                last_ = first_ = p;
                size_ = 1;
            }
            mutex_.UnLock();
        }

        T pop() {
            mutex_.Lock();
            if (0 == size_) {
                mutex_.UnLock();
                return NULL;
            }

            Node* temp = first_;
            if (temp == NULL) {
                mutex_.UnLock();
                return NULL;
            }

            if (--size_) {
                // more than 1 item
                first_ = static_cast<Node*>(first_->next_);
            } else {
                // last item
                first_ = last_ = NULL;
            }
            mutex_.UnLock();

            T return_val = temp->value_;
            delete temp;
            return return_val;
        }

    private:
        struct Node {
            T value_;
            void *next_;
        };

        volatile unsigned int size_;

        Node* first_;
        Node* last_;

        Mutex mutex_;

};

#endif /* end of include guard: FQUEUE_HPP_SO4FYVJU */
