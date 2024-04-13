﻿#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>

static pthread_rwlock_t rw_lock;
static int n_r = 0;
static int n_w = 0;
pthread_mutex_t state_check_mutex;

static void
cs_status_check() {

	pthread_mutex_lock(&state_check_mutex);
	assert(n_r >= 0 && n_w >= 0); /* cannot be negative */

	if (n_r == 0 && n_w == 0) {
		//valid condition
	}
	else if (n_r > 0 && n_w == 0) {
		//valid condition
	}
	else if (n_r == 0 && n_w == 1) {
		//valid condition
	}
	else
		assert(0);

	printf("n_r = %d, n_w = %d\n", n_r, n_w);
	pthread_mutex_unlock(&state_check_mutex);
}

static void
execute_cs() {

	cs_status_check();
}

static void
reader_enter_cs() {
	pthread_mutex_lock(&state_check_mutex);
	n_r++;
	pthread_mutex_unlock(&state_check_mutex);
}

static void
reader_leaves_cs() {
	pthread_mutex_lock(&state_check_mutex);
	n_r--;
	pthread_mutex_unlock(&state_check_mutex);
}


static void
writer_enter_cs() {
	n_w++;
}

static void
writer_leaves_cs() {
	n_w--;
}

void*
read_thread_fn(void* arg) {
	while (1) {
		pthread_rwlock_rdlock(&rw_lock);

		reader_enter_cs();
		execute_cs();
		reader_leaves_cs();

		pthread_rwlock_unlock(&rw_lock);
	}
}

void*
write_thread_fn(void* arg) {

	while (1) {
		pthread_rwlock_wrlock(&rw_lock);
		writer_enter_cs();
		execute_cs();
		writer_leaves_cs();
		pthread_rwlock_unlock(&rw_lock);
	}
}

int
main(int argc, char* argv[]) {

	static pthread_t th1, th2, th3, th4, th5, th6;
	pthread_rwlock_init(&rw_lock, NULL);
	pthread_mutex_init(&state_check_mutex, NULL);
	pthread_create(&th1, NULL, read_thread_fn, NULL);
	pthread_create(&th2, NULL, read_thread_fn, NULL);
	pthread_create(&th3, NULL, read_thread_fn, NULL);
	pthread_create(&th4, NULL, write_thread_fn, NULL);
	pthread_create(&th5, NULL, write_thread_fn, NULL);
	pthread_create(&th6, NULL, write_thread_fn, NULL);
	pthread_exit(0);
	return 0;
}