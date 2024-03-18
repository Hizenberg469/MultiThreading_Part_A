#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> /*For working of POSIX thread*/
#include <unistd.h> /* For pause() and sleep() */
#include <errno.h> /*For using Global variable errno*/

void*
thread_fn_callback(void* arg) {
	
	return arg;

}



void
create_thread(pthread_t* p, int arg) {

	int* value = (int*)malloc(sizeof(int));

	*value = arg;

	pthread_attr_t attr;

	pthread_attr_init(&attr);

	pthread_attr_setdetachstate(&attr,
		PTHREAD_CREATE_JOINABLE);

	pthread_create(p,
		&attr,
		thread_fn_callback,
		(void*)value);
}

int
main(int argv, char* argc[]) {
	
	int n;
	printf("Enter the number of threads:");
	scanf("%d", &n);

	pthread_t* threads;
	threads = (pthread_t *)calloc(n, sizeof(pthread_t));

	int val;

	for (int i = 0; i < n; i++) {

		val = i+1;
		create_thread(&threads[i], val);
	}

	void* thread_result = NULL;
	for (int i = n - 1; i >= 0; i--) {
		pthread_join(threads[i], &thread_result);

		printf("%d\n", *(int*)thread_result);

		if( thread_result != NULL)
		free(thread_result);
		thread_result = NULL;
	}

	free(threads);
	return 0;
}