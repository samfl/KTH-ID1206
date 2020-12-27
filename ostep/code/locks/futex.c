/***
 * 
 * Linux-based Fitex Locks
 * A two-phase lock in the sense that it first spins and then caller is put to sleep
 * An instance of a hybrid approach. 
 * Figure 28.10
 * 
***/

#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

void mutex_lock(int* mutex)
{
    int v; 
    if (atomic_bit_test_set(mutex, 31) == 0)
    {
        return; 
    }
    atomic_increment(mutex); 
    while (1)
    {
        if (atomic_bit_test_set(mutex, 31) == 0)
        {
            atomic_decrement(mutex);
            return; 
        }
        v = *mutex; 
        if (v >= 0)
        {
            continue; 
        }
        futex_wait(mutex, v);
    }
}

void mutex_unlock(int* mutex)
{
    if (atomic_add_zero (mutex, 0x800000000))
    {
        return; 
    }

    futex_wake(mutex);
}

int main(int argc, char* argv[])
{
    return 0; 
}