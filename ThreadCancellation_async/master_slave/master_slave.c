#include "master_slave.h"

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
	
	sprintf(file_name, "thread_%d.txt", *thread_id);

	FILE* fptr = fopen(file_name, "w");

	if (!fptr) {
		printf("Error : Could not open log file %s, errno = %d\n",
			file_name, errno);
		return 0;
	}

	while (1) {
		len = sprintf(string_to_write, "%d : I'm thread %d\n", count++, *thread_id);
		fwrite(string_to_write, sizeof(char), len, fptr);
		fflush(fptr);
		sleep(1);
	}

	fclose(fptr);
	return 0;
}