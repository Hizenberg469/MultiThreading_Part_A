#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


#define MAX_THREADS 5
#define PCOUNT 2

sem_t sem;
pthread_t thread[MAX_THREADS];

static void*
fn_callback(void* arg) {
	
	char* th_name = (char*)arg;

	/*Critical Section*/

	printf("%s waiting to enter C.S\n", th_name);

	sem_wait(&sem);

	printf("%s entered the C.S\n", th_name);

	for (int i = 0; i < 5; i++) {
		printf("%s execute the C.S\n", th_name);
		sleep(1);
	}

	printf("%s leaving the C.S\n", th_name);

	sem_post(&sem);

	printf("%s out of the C.S\n", th_name);


}

void
create_thread(pthread_t* p, void* arg) {

	int rc = pthread_create(p, NULL, fn_callback, arg);


	if (rc != 0) {
		printf("Thread creation failed, error code = %d\n", rc);
		exit(EXIT_FAILURE);
	}
}

int
main(int argv, char* argc[]) {

	sem_init(&sem, 0, PCOUNT);
	create_thread(&thread[0], "Thread1");
	create_thread(&thread[1], "Thread2");
	create_thread(&thread[2], "Thread3");
	create_thread(&thread[3], "Thread4");
	create_thread(&thread[4], "Thread5");


	for (int i = 0; i < 5; i++) {
		pthread_join(thread[i], NULL);

	}
	sem_destroy(&sem);

	return 0;
}