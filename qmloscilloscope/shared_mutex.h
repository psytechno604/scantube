#ifndef SHARED_MUTEX_H
#define SHARED_MUTEX_H

#include <mutex>

class shared_mutex
{
    std::mutex    mut_;
    std::condition_variable gate1_;
    std::condition_variable gate2_;
    unsigned state_;

    static const unsigned write_entered_ = 1U << (sizeof(unsigned)*CHAR_BIT - 1);
    static const unsigned n_readers_ = ~write_entered_;

public:

    shared_mutex() : state_(0) {}

// Exclusive ownership

    void lock();
    bool try_lock();
    void unlock();

// Shared ownership

    void lock_shared();
    bool try_lock_shared();
    void unlock_shared();
};

#endif // SHARED_MUTEX_H
