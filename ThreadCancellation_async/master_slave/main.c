#include "master_slave.h"


#define N_SLAVES 5

pthread_t slaves[N_SLAVES];

int main() {

	int i;
	int* thread_id = 0;

	for (i = 0; i < N_SLAVES; i++) {
		thread_id = calloc(1, sizeof(*thread_id));
		*thread_id = i;
		pthread_create(&slaves[i], 0, write_into_file, thread_id);
	}

	/* main menu */

	int choice;
	int thread_num;

	while (1) {

		printf("1. Cancel the thread\n");
		scanf("%d", &choice);
		getchar();
		printf("Enter slave thread id [0-%d] :", N_SLAVES - 1);
		scanf("%d", &thread_num);
		getchar();

		if (thread_num < 0 || thread_num >= N_SLAVES) {
			printf("Invalid Thread id\n");
			exit(0);
		}
		printf("\n");

		switch (choice) {

		case 1:
			pthread_cancel(slaves[thread_num]);
			break;
		default:
			continue;

		}
	}
	return 0;
}