#include <stdio.h>
#include <stdint.h>
#include "network_utils.h"

void
pkt_recv_fn(
	char* pkt,
	uint32_t pkt_size,
	char* sender_ip,
	uint32_t port_no
) {

	printf("\n%s() : pkt recvd = %s, pkt size = %u\n",
		__FUNCTION__, pkt, pkt_size);
}

pthread_t* listener1 = NULL;
pthread_t* listener2 = NULL;

int
main(int argc, char** argv) {

	printf("Listening on UDP port no 3000\n");
	listener1 = udp_server_create_and_start(
		"127.0.0.1",
		3000,
		pkt_recv_fn
	);

	printf("Listening on UDP port no 3001\n");
	listener2 = udp_server_create_and_start(
		"127.0.0.1",
		3001,
		pkt_recv_fn
	);

	int th_no,cnt = 0;
	while (1) {
		printf("Listener thread you can cancel [ 1 - 2 ]:");
		scanf("%d", &th_no);
		getchar();

		switch (th_no) {
			
		case 1:
			pthread_cancel((*listener1));
			break;
		case 2:
			pthread_cancel((*listener2));
			break;
		case 1000:
			exit(EXIT_SUCCESS);
			break;
		default:
			break;

		}
		sleep(1);
	}

	pthread_exit(0);
	return 0;
}