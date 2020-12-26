    #include <stdlib.h>
    #include <stdio.h>
    #include <pthread.h>
    #include <unistd.h>
    #include <linux/futex.h>
    #include <sys/syscall.h>

    typedef struct args {
        int inc; 
        int id;
        volatile int* mutex;  
    } args; 

    volatile int global = 0; 
    volatile int count = 0; 

    int futex_wait(volatile int* futexp)
    {
        return syscall(SYS_futex, futexp, FUTEX_WAIT, 1, NULL, NULL, 0);
    }

    int futex_wake(volatile int* futexp)
    {
        return syscall(SYS_futex, futexp, FUTEX_WAKE, 1, NULL, NULL, 0);
    }

    int try(volatile int* mutex)
    {
        return __sync_val_compare_and_swap(mutex, 0, 1);
    }

    int lock(volatile int* lock)
    {   
        int susp = 0; 
        while (try(lock) != 0)
        {
            //printf(".");
            susp++;
            futex_wait(lock);
        }
        return susp;
    }

    void unlock(volatile int* lock)
    {
        *lock = 0;
        futex_wake(lock);  
    }

    void* increment(void* arg)
    {
        int inc = ((args*) arg)->inc;
        int id = ((args*) arg)->id;
        volatile int* mutex = ((args*) arg)->mutex;

        for (int i = 0; i < inc; i++)
        {
            int spin = lock(mutex);
            //printf("%d\n", spin);
            count++; 
            unlock(mutex); 
        }
    }

    /* Our task is to have 2 threads incrementing a shared counter "count". */
    int main(int argc, char* argv[])
    {
        if (argc != 2)
        {
            printf("usage swap <inc>\n");
            exit(0); 
        }

        int inc = atoi(argv[1]);

        pthread_t one_p, two_p;
        args one_args, two_args; 

        one_args.inc = inc; 
        two_args.inc = inc; 

        one_args.id = 0; 
        two_args.id = 1; 

        one_args.mutex = &global;
        two_args.mutex = &global;

        pthread_create(&one_p, NULL, increment, &one_args);
        pthread_create(&two_p, NULL, increment, &two_args);
        pthread_join(one_p, NULL);
        pthread_join(two_p, NULL);

        printf("result is %d\n", count);

        return 0; 
    }