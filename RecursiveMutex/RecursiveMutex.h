#ifndef __REC_MUTEX__
#define __REC_MUTEX__

#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct rec_mutex_ {
	
	/* number of locks by same threads*/
	uint16_t nLocks;

	/* Thread ID to lock the thread */
	pthread_t lockingThread;

	/* Condition variable to block all the other thread
	for accessing the C.S */
	pthread_cond_t cv;

	/* mutex to provide mutual exclusiveness for 
	changing the state of the recursive mutex */
	pthread_mutex_t stateMutex;

	/* Number of thread waiting for this mutex lock grant*/
	uint16_t nWaited;
}rec_mutex_t;

void
rec_mutex_init(rec_mutex_t* mutex);

void
rec_mutex_lock(rec_mutex_t* mutex);

void
rec_mutex_unlock(rec_mutex_t* mutex);

void
rec_mutex_destroy(rec_mutex_t* mutex);

#endif