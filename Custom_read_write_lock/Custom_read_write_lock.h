#ifndef __RW_LOCK_H
#define __RW_LOCK_H

#include <pthread.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct rwlock_{

	/* A Mutex to manipulate/inspect the state of rwlock
	in a mutually exclusive manner */

	pthread_mutex_t stateMutex;

	/* CV to block the threads if rwlock is not available*/

	pthread_cond_t cv;

	/* count of number of concurrent thread existing inside C.S*/
	uint16_t n_locks;

	/* No of reader thread waiting for the lock grant*/
	uint16_t n_reader_waiting;

	/* No of writer thread waiting for the lock grant*/
	uint16_t n_writer_waiting;

	/* Is locked currently occupied by Reader threads */
	bool is_locked_by_reader;

	/* Is locked currently occupied by Writer threads*/
	bool is_locked_by_writer;

	/* Thread handle of the writer thread currently holding the 
	lock. It is 0 if lock is not being held by any writer thread*/
	pthread_t writer_thread;
}rwlock_t;


void
rw_lock_init(rwlock_t* rw_lock);

void
rw_lock_rd_lock(rwlock_t* rw_lock);

void
rw_lock_wr_lock(rwlock_t* rw_lock);

void
rw_lock_unlock(rwlock_t* rw_lock);

void
rw_lock_destroy(rwlock_t* rw_lock);

#endif