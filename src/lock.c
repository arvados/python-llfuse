/*
lock.c

This file provides the plain C components for the global lock.

Copyright (C) Nikolaus Rath <Nikolaus@rath.org>

This file is part of python-llfuse (http://python-llfuse.googlecode.com).
python-llfuse can be distributed under the terms of the GNU LGPL.
*/

#define TRUE  (1==1)
#define FALSE (0==1)

#include <pthread.h>

int acquire(void);
int release(void);
int c_yield(int count);

// Who was the last to acquire the lock
static pthread_t lock_owner;

// Is the lock currently taken
static int lock_taken = FALSE;

/* This variable indicates how many threads are currently waiting for
 * the lock. */
static int lock_wanted = 0;

/* Mutex for protecting access to lock_wanted, lock_owner and
 * lock_taken */
static pthread_mutex_t mutex;

/* Condition even to notify when the lock becomes available */
static pthread_cond_t cond;

void init_lock(void) {
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex, NULL);
}

int acquire(void) {
    int ret;
    pthread_t me = pthread_self();

    ret = pthread_mutex_lock(&mutex);
    if(ret != 0) return ret;
    if(lock_taken) {
        if(pthread_equal(lock_owner, me)) {
            pthread_mutex_unlock(&mutex);
            return EDEADLK;
        }
        lock_wanted++;

        /* We need while here even though pthread_cond_signal wakes
         * only one thread:
         * http://stackoverflow.com/questions/8378789/forcing-a-thread-context-switch
         * http://en.wikipedia.org/wiki/Spurious_wakeup */
        while(lock_taken) pthread_cond_wait(&cond, &mutex);

        lock_wanted--;
    }
    lock_taken = TRUE;
    lock_owner = me;
    return pthread_mutex_unlock(&mutex);
}

int release(void) {
    int ret;
    if(!lock_taken)
        return EPERM;
    if(!pthread_equal(lock_owner, pthread_self())) 
        return EPERM;
    ret = pthread_mutex_lock(&mutex);
    if(ret != 0) return ret;
    lock_taken = FALSE;
    if(lock_wanted > 0) {
        pthread_cond_signal(&cond);
    }
    return pthread_mutex_unlock(&mutex);
}

int c_yield(int count) {
    int ret;
    int i;
    pthread_t me = pthread_self();
    
    if(!lock_taken || !pthread_equal(lock_owner, me)) 
        return EPERM;
    ret = pthread_mutex_lock(&mutex);
    if(ret != 0) return ret;

    for(i=0; i < count; i++) {
        if(lock_wanted == 0)
            break;
        
        lock_taken = FALSE;
        lock_wanted++;
        pthread_cond_signal(&cond);
        // See acquire() for why 'while' is required
        while(lock_taken) pthread_cond_wait(&cond, &mutex); 
        lock_wanted--;
        if(lock_taken) {
            pthread_mutex_unlock(&mutex);
            return EPROTO;
        }
        if(pthread_equal(lock_owner, me)) 
            return ENOMSG;
        lock_taken = TRUE;
        lock_owner = me;
    }
    return pthread_mutex_unlock(&mutex);
}


