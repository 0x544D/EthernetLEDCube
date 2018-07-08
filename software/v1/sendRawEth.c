#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>

#define FPGA_MAC_ADDR0	0x4c
#define FPGA_MAC_ADDR1	0xcc
#define FPGA_MAC_ADDR2	0x6a
#define FPGA_MAC_ADDR3	0xf9
#define FPGA_MAC_ADDR4	0x6e
#define FPGA_MAC_ADDR5	0xcf

#define INTERFACE	"enp0s3"
#define BUF_SIZ		122

#define BUF_PAYLOAD_BEGIN	14
#define CUBE_DIM	6

#define PWM_STEPS	256


int sockfd;
struct ifreq if_idx;
struct ifreq if_mac;
int tx_len = 0;
char sendbuf[BUF_SIZ];
struct ether_header *eh = (struct ether_header *) sendbuf;
struct sockaddr_ll socket_address;
char ifName[] = INTERFACE;

void initEthernet();
void *ethSend(void *);

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} led;

led red = {255,0,0};
led green = {0,255,0};
led blue = {0,0,255};

led yellow = {255,255,0};
led magenta = {255,0,255};
led cyan = {0,255,255};

led white = {255,255,255};
led black = {0,0,0};

led ledCube[CUBE_DIM][CUBE_DIM][CUBE_DIM];


void drawLine(uint8_t x0, uint8_t y0, uint8_t z0, uint8_t x1, uint8_t y1, uint8_t z1, led color);
void drawCuboid(uint8_t x0, uint8_t y0, uint8_t z0, uint8_t x1, uint8_t y1, uint8_t z1, led color);
void send_single_test_frame();
void *ethSend_single(void* void_ptr);


void drawLine(uint8_t x0, uint8_t y0, uint8_t z0, uint8_t x1, uint8_t y1, uint8_t z1, led color) {
	
	if (x0 > x1) {
		uint8_t c = x0;
		x0 = x1;
		x1 = c;
	}
	if (y0 > y1) {
		uint8_t c = y0;
		y0 = y1;
		y1 = c;
	}
	if (z0 > z1) {
		uint8_t c = z0;
		z0 = z1;
		z1 = c;
	}

	uint8_t diffX = x1-x0;
	uint8_t diffY = y1-y0;
	uint8_t diffZ = z1-z0;

	for (uint8_t x = x0; x < x1; x++) {
		uint8_t y = round(((double)diffY / (double)diffX) * x + x0);
		uint8_t z = round(((double)diffZ / (double)diffX) * x + x0);

		ledCube[x][y][z] = color;
	}
}

void drawCuboid(uint8_t x0, uint8_t y0, uint8_t z0, uint8_t x1, uint8_t y1, uint8_t z1, led color) {

	for (uint8_t x = x0; x < x1; x++) {
		for (int y = y0; y < y1; y++) {
			for (int z = z0; z < z1; z++) {
				ledCube[x][y][z] = color;
			}
		}
	}
	
}

int main(int argc, char *argv[])
{
	
	srand(time(NULL)); 

    /*
    for (int c = 14; c < 95; c++) {
        sendbuf[c] = rand();
    }*/

    initEthernet();

    pthread_t senderThread;

	/* Original    
	if(pthread_create(&senderThread, NULL, ethSend, NULL)) {
		perror("creating sender thread failed!");
		return 1;
	}
	*/

	send_single_test_frame();


	// Send single Frame
	if(pthread_create(&senderThread, NULL, ethSend_single, NULL)) {
		perror("creating sender thread failed!");
		return 1;
	}

	// trap program here
	while(1){};


	led darkred = {128,0,0};

	//drawLine(0,0,0,1,CUBE_DIM,1, white);
	drawCuboid(0, 0, 0, CUBE_DIM, CUBE_DIM, CUBE_DIM, darkred);

	
	while(1) {

		for (uint8_t t = 0; t < (PWM_STEPS-1); t++) {

			for (int z = 0; z < CUBE_DIM; z++) {

				uint8_t zByteOffset = z * 18;	
					
				for (int xy = 0; xy < (CUBE_DIM*CUBE_DIM/2); xy++) {

					uint8_t x = (xy % 3)*2;
					uint8_t y = xy / 3;

					uint8_t msgByte = ledCube[x][y][z].r > t;
					msgByte |= (ledCube[x][y][z].g > t) << 1;
					msgByte |= (ledCube[x][y][z].b > t) << 2;

					msgByte |= (ledCube[x+1][y][z].r > t) << 4;
					msgByte |= (ledCube[x+1][y][z].g > t) << 5;
					msgByte |= (ledCube[x+1][y][z].b > t) << 6;

					sendbuf[BUF_PAYLOAD_BEGIN + xy + zByteOffset] = msgByte;
					
				}
			}

			usleep(19);
		}

		/*
		for (int c = BUF_PAYLOAD_BEGIN; c < BUF_SIZ; c++) {
			sendbuf[c] = 'A';
			usleep(1);
		}

		for (int c = BUF_PAYLOAD_BEGIN; c < BUF_SIZ; c++) {
			sendbuf[c] = 'B';
			usleep(1);
		}*/
	}


	pthread_join(senderThread, NULL);

	return 0;
}


void *ethSend(void* void_ptr) {

    clock_t lastsend;
    clock_t current;
    clock_t diff;

    lastsend = clock();

 
    while (1){
    for (int i=0; i<1000; i++) {

        /*
        for (int c = 14; c < 122; c++) {
            sendbuf[c] = rand();
        }*/

        //while((clock()-lastsend) < 960){};

        //while(clock()%100){};

	    sendto(sockfd, sendbuf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll));
        //pack++;
 
        //usleep(1000);
        /*current = clock();
        diff = current-lastsend;
        if (diff < 50)
            usleep(50 - diff);
        lastsend = current;*/

    }

    current = clock();
    diff = current-lastsend;
    lastsend = current;

    if (diff > 45000)
        printf("\033[31m %u \n", diff);
    else if (diff < 15000)
        printf("\033[33m %u \n", diff);
    else 
        printf("\033[m %u \n", diff);
    //printf("%u %u \n", diff, pack);

    
    }
}



void initEthernet() {

	/* Open RAW socket to send on */
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
	    perror("socket");
	}

	/* Get the index of the interface to send on */
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
	    perror("SIOCGIFINDEX");
	/* Get the MAC address of the interface to send on */
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
	    perror("SIOCGIFHWADDR");

    // Construct the Ethernet header 
	memset(sendbuf, 0, BUF_SIZ);

	/* Ethernet header */
	eh->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
	eh->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
	eh->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
	eh->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
	eh->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
	eh->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
	eh->ether_dhost[0] = FPGA_MAC_ADDR0;
	eh->ether_dhost[1] = FPGA_MAC_ADDR1;
	eh->ether_dhost[2] = FPGA_MAC_ADDR2;
	eh->ether_dhost[3] = FPGA_MAC_ADDR3;
	eh->ether_dhost[4] = FPGA_MAC_ADDR4;
	eh->ether_dhost[5] = FPGA_MAC_ADDR5;
	/* Ethertype field */
	eh->ether_type = htons(0x006c);//ETH_P_IP
	tx_len += sizeof(struct ether_header);

    
	/* Index of the network device */
	socket_address.sll_ifindex = if_idx.ifr_ifindex;
	/* Address length*/
	socket_address.sll_halen = ETH_ALEN;
	/* Destination MAC */
	socket_address.sll_addr[0] = FPGA_MAC_ADDR0;
	socket_address.sll_addr[1] = FPGA_MAC_ADDR1;
	socket_address.sll_addr[2] = FPGA_MAC_ADDR2;
	socket_address.sll_addr[3] = FPGA_MAC_ADDR3;
	socket_address.sll_addr[4] = FPGA_MAC_ADDR4;
	socket_address.sll_addr[5] = FPGA_MAC_ADDR5;

    //tx_len += 81;
    tx_len += 108;
}

/***** All functions for single test frame *****/
void *ethSend_single(void* void_ptr) {

    clock_t lastsend;
    clock_t current;
    clock_t diff;

    lastsend = clock();

 
    while (1){
    for (int i=0; i<1; i++) {

        /*
        for (int c = 14; c < 122; c++) {
            sendbuf[c] = rand();
        }*/

        //while((clock()-lastsend) < 960){};

        //while(clock()%100){};

	    sendto(sockfd, sendbuf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll));
        //pack++;
 
        //usleep(1000);
        /*current = clock();
        diff = current-lastsend;
        if (diff < 50)
            usleep(50 - diff);
        lastsend = current;*/

    }

    current = clock();
    diff = current-lastsend;
    lastsend = current;

    if (diff > 45000)
        printf("\033[31m %u \n", diff);
    else if (diff < 15000)
        printf("\033[33m %u \n", diff);
    else 
        printf("\033[m %u \n", diff);
    //printf("%u %u \n", diff, pack);

    
    }
}

void send_single_test_frame()
{
	int offset = 0;

	for (int i=0; i<50; i++) {
		uint8_t msgByte = i;
		sendbuf[BUF_PAYLOAD_BEGIN + offset] = msgByte;
		offset++;
	}
}

