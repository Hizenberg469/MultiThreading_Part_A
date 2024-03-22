#include "queue/queue.h"
#include <stdio.h>
#include <stdlib.h>

struct Queue_t* reso;

static const char* prod1 = "TP1";
static const char* prod2 = "TP2";
static const char* cons1 = "TC1";
static const char* cons2 = "TC2";

void*
producer_fn_callback(void* arg) {
	
	char* th_name = (char*)arg;

	printf("Thread %s waiting to lock the Queue\n", th_name);

	// C.S start
	pthread_mutex_lock(&(reso->q_mutex));

	printf("Thread %s locks the Queue\n", th_name);

	while (is_queue_full(reso)) {
		printf("Currently Queue is full, Thread %s cannot insert element\n", th_name);
		pthread_cond_wait(&(reso->q_cv), &(reso->q_mutex));
		printf("Thread %s wakes up, checking the Queue status again\n", th_name);
	}

	if (is_queue_full(reso)) {
		printf("Queue is full, but accessd by thread. Error Occured.\n");
		exit(EXIT_FAILURE);
	}

	int* value = NULL;
	int i = 1;
	while( !is_queue_full(reso) ) {
		printf("Thread %s produces new integer %d\n", th_name, i);

		value = (int*)malloc(sizeof(int));
		*value = i;
		enqueue(reso, (void*)value);
		printf("Thread %s pushed integer %d in Queue, Queue size = %d\n", th_name, i, Q_COUNT(reso));
		i++;
	}

	printf("Thread %s filled up the Queue, signalling and releasing the lock\n",th_name);

	pthread_cond_broadcast(&(reso->q_cv));
	

	pthread_mutex_unlock(&(reso->q_mutex));
	printf("Thread %s finished and exit\n", th_name);

	return NULL;
}

void*
consumer_fn_callback(void* arg) {

	char* th_name = (char*)arg;

	printf("Thread %s waiting to lock the Queue\n", th_name);

	// C.S start
	pthread_mutex_lock(&(reso->q_mutex));

	printf("Thread %s locks the Queue\n", th_name);

	while (is_queue_empty(reso)) {
		printf("Currently Queue is empty, Thread %s cannot drain Queue\n", th_name);
		pthread_cond_wait(&(reso->q_cv), &(reso->q_mutex));
		printf("Thread %s wakes up, checking the Queue status again\n", th_name);
	}

	if (is_queue_empty(reso)) {
		printf("Queue is empty, but accessd by thread. Error Occured.\n");
		exit(EXIT_FAILURE);
	}

	int value = 0;
	while(!is_queue_empty(reso)){
		value = *((int*)deque(reso));

		printf("Thread %s consumes ans integer %d, Queue size = %d\n", th_name, value, Q_COUNT(reso));
	}

	printf("Thread %s Drains the entire Queue, sending signal to Blocking Therads\n", th_name);

	pthread_cond_broadcast(&(reso->q_cv));

	printf("Thread %s releasing lock\n", th_name);
	pthread_mutex_unlock(&(reso->q_mutex));

	printf("Thread %s finished and exit\n", th_name);
	return NULL;
}


void
create_thread(pthread_t* p, void* (*fn_callback)(void *arg),const char* arg) {

	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr,
		PTHREAD_CREATE_JOINABLE);

	pthread_create(p,
		&attr,
		fn_callback,
		(void*)arg);


}

int
main(int argv, char* argc[]) {

	pthread_t pt1;
	pthread_t pt2;
	pthread_t ct1;
	pthread_t ct2;

	reso = initQ();

	create_thread(&pt1,producer_fn_callback,prod1);
	create_thread(&pt2,producer_fn_callback,prod2);
	create_thread(&ct1,consumer_fn_callback,cons1);
	create_thread(&ct2,consumer_fn_callback,cons2);

	pthread_join(pt1, NULL);
	pthread_join(pt2, NULL);
	pthread_join(ct1, NULL);
	pthread_join(ct2, NULL);

	printf("Program finished\n");
	pthread_exit(0);
	return 0;
}