#include "RecursiveMutex.h"
#include <stdlib.h>
#include <assert.h>

void
rec_mutex_init(rec_mutex_t* mutex) {

	/* No lock till by same thread*/
	mutex->nLocks = 0;

	/* No lock by any thread*/
	mutex->lockingThread = 0;

	/* initialising cv */
	pthread_cond_init(&mutex->cv, NULL);

	/* initialising state mutex*/
	pthread_mutex_init(&mutex->stateMutex, NULL);

	/* No blocked threads till now*/
	mutex->nWaited = 0;
}

void
rec_mutex_lock(rec_mutex_t* mutex) {

	pthread_mutex_lock(&mutex->stateMutex);
	/* Case 1 : No lock is applied till now */
	if (mutex->nLocks == 0) {
		mutex->lockingThread = pthread_self();
		mutex->nLocks=1;
		pthread_mutex_unlock(&mutex->stateMutex);
		return;
	}
	/* Case 2 : Locked is applied by same thread */
	if (mutex->lockingThread == pthread_self()) {

		if (mutex->nLocks == 0) {
			printf("No self locking. Thread is applying lock for first time.\n");
			exit(EXIT_FAILURE);
		}

		mutex->nLocks++;
		pthread_mutex_unlock(&mutex->stateMutex);
		return;
	}
	/* Case 3 : Different thread is trying to apply
	the lock */

	while (mutex->lockingThread && mutex->lockingThread != pthread_self()) {

		mutex->nWaited++;
		pthread_cond_wait(&mutex->cv, &mutex->stateMutex);
		mutex->nWaited--;
	}

	/* Another thread trying to applying lock after 
	the owner thread unlocked all its locks on mutexes.*/
	if (mutex->nLocks != 0 || 
		mutex->lockingThread != 0 ) {
		printf("Locks is still on rec mutex and another thread is trying to apply a lock.\n");
		exit(EXIT_FAILURE);
	}

	mutex->nLocks = 1;
	mutex->lockingThread = pthread_self();
	pthread_mutex_unlock(&mutex->stateMutex);

}

void
rec_mutex_unlock(rec_mutex_t* mutex) {

	pthread_mutex_lock(&mutex->stateMutex);
	/* Case 1 : No lock is currently on recursive mutex.*/
	if (mutex->nLocks == 0 || mutex->lockingThread == 0) {
		printf("No lock is present on the rec mutex.\n");
		exit(EXIT_FAILURE);
	}

	/* Case 2 : Unlocking by same thread. */
	if (mutex->lockingThread == pthread_self()) {

		assert(mutex->nLocks > 0);

		mutex->nLocks--;

		if (mutex->nLocks > 0) {
			pthread_mutex_unlock(&mutex->stateMutex);
			return;
		}

		mutex->lockingThread = 0;
		if (mutex->nWaited > 0) {
			pthread_cond_signal(&mutex->cv);
		}

		pthread_mutex_unlock(&mutex->stateMutex);
		return;
	}

	/* Case 3 : Unlocking attempt by different thread. */

	while (mutex->lockingThread && mutex->lockingThread != pthread_self()) {
		assert(0);
	}

	pthread_mutex_unlock(&mutex->stateMutex);
}

void
rec_mutex_destroy(rec_mutex_t* mutex) {
	if (mutex->nLocks > 0) {
		printf("Mutex is still locked by live thread.\n");
		exit(EXIT_FAILURE);
	}

	if (mutex->lockingThread != 0) {
		printf("Mutex is still locked by live thread.\n");
		exit(EXIT_FAILURE);
	}

	if (mutex->nWaited > 0) {
		printf("There is some blocked state.\n");
		exit(EXIT_FAILURE);
	}

	pthread_mutex_destroy(&mutex->stateMutex);

	pthread_cond_destroy(&mutex->cv);
}