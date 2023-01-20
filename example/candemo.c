
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/socket.h>

#include <linux/can.h>
#include <linux/can/raw.h>

#define TX_CYCLE   500  // ms  

time_t time_in_ms(void)
{
	struct timeval tv = {0};

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000UL + tv.tv_usec / 1000UL);
}

int main(int argc, char **argv)
{
	int s, i; 
	int nbytes;
	struct sockaddr_can addr;
	struct ifreq ifr;
	struct can_frame rxframe;
	struct can_frame txframe;

        if (argc != 2) {
		printf("Syntax: candemo <ix_can>\r\n");
		return 1;
	}

	printf("CAN Sockets Transmit/Receive Demo\r\n");

	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		perror("Socket");
		return 1;
	}

	strcpy(ifr.ifr_name, argv[1]);
	ioctl(s, SIOCGIFINDEX, &ifr);

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;

	if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("Bind");
		return 1;
	}

	txframe.can_id = 0x123;
	txframe.can_dlc = 8;
	memcpy(txframe.data, "\001\002\003\004\005\006\007\010", 8);
	time_t txcycle = time_in_ms();
	
	while(1) {
		// non blocking call
		nbytes = recv(s, &rxframe, sizeof(struct can_frame), MSG_DONTWAIT);

		if (nbytes > 0) {
			printf("0x%03X [%d] ",rxframe.can_id, rxframe.can_dlc);

			for (i = 0; i < rxframe.can_dlc; i++)
				printf("%02X ",rxframe.data[i]);

			printf("\r\n");
		}

		if(txcycle < time_in_ms()) {
			txcycle = time_in_ms() + TX_CYCLE;
			
			if (write(s, &txframe, sizeof(struct can_frame)) != sizeof(struct can_frame)) {
				perror("Write");
				return 1;
			}
			txframe.data[0]++;
		}

		usleep(10000);
	}

	if (close(s) < 0) {
		perror("Close");
		return 1;
	}

	return 0;
}
