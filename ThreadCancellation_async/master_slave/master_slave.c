#include "master_slave.h"

void
memory_cleanup_handler(void* arg) {
	
	printf("%s invoked ...\n", __FUNCTION__);
	free(arg);
}

void
file_cleanup_handler(void* arg) {
	
	printf("%s invoked ...\n", __FUNCTION__);
	fclose((FILE*)arg);

}

void *
write_into_file(void* arg) {
	
	char file_name[64];
	char string_to_write[64];
	int len;
	int count = 0;

	/* Mark the thread eligible for cancellation */
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE /*PTHREAD_CANCEL_DISABLE*/, 0);

	/* Mode of cancellation */
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
	int* thread_id = (int*)arg;

	/* When pthread_cancel is invoked and this thread get the
	signal to get cancelled, this API is invoked to free up the 
	resources used by this thread.*/
	pthread_cleanup_push(memory_cleanup_handler, arg);
	
	sprintf(file_name, "thread_%d.txt", *thread_id);

	FILE* fptr = fopen(file_name, "w");

	pthread_cleanup_push(file_cleanup_handler, fptr);

	if (!fptr) {
		printf("Error : Could not open log file %s, errno = %d\n",
			file_name, errno);
		pthread_exit(0);
	}

	while (1) {
		len = sprintf(string_to_write, "%d : I'm thread %d\n", count++, *thread_id);
		fwrite(string_to_write, sizeof(char), len, fptr);
		fflush(fptr);
		sleep(1);
	}

	/* If this thread is successfully executed and the cancellation request
	is not invoked till now then, the stack where resource free API are
	need to be cleaned.*/
	pthread_cleanup_pop(0);
	pthread_cleanup_pop(0);

	/*This is not executed as this thread is in infinite loop
	and when the pthread_cancel is executed this thread is cancelled
	while it was executing in the loop. Hence, this causes resource
	leakage.*/
	// fclose(fptr);
	return 0;
}