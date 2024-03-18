#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>	/* for working with posix threads */
#include <unistd.h>		/* for pause() and sleep() */
#include <errno.h>		/* for using global variable errno */
#include <assert.h>

int arr[] = { 1,2,3,4,5 };

void
print_array() {

	int i = 0;
	int arr_size = sizeof(arr) / sizeof(arr[0]);

	for (; i < arr_size - 1; i++) {

		printf("%d ", arr[i]);
	}

	printf("%d\n", arr[i]);
}

/* a thread callback fn must have following prototypes
	void *(*thread_fn)(void *) */

static void *
thread_fn_callback_sum(void* arg) {

	int i;
	int sum;

	int arr_size = sizeof(arr) / sizeof(arr[0]);

	do {
		sum = 0;
		i = 0;
		while (i < arr_size) {
			sum += arr[i];
			i++;
		}

		printf("sum = %d\n", sum);

	} while (1);
}

static void *
thread_fn_callback_swap(void* arg) {

	int temp;
	int arr_size = sizeof(arr) / sizeof(arr[0]);

	do {
		temp = arr[0];
		arr[0] = arr[arr_size - 1];
		arr[arr_size - 1] = temp;
	} while (1);
}

void
sum_thread_create() {

	pthread_t pthread1;

	int rc = pthread_create(&pthread1,
		NULL,
		thread_fn_callback_sum,
		NULL);

	if (rc != 0) {
		printf("error occured, thread could not be created, errno = %d\n", rc);
		exit(EXIT_FAILURE);
	}
}

void
swap_thread_create() {

	pthread_t pthread2;

	int rc = pthread_create(&pthread2,
		NULL,
		thread_fn_callback_swap,
		NULL);

	if (rc != 0) {
		printf("error occured, thread could not be created, errno = %d\n", rc);
		exit(EXIT_FAILURE);
	}
}

int
main(int argc, char* argv[]) {

	sum_thread_create();
	swap_thread_create();

	pthread_exit(0);
	return 0;
}