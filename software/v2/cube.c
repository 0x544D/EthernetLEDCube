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


#include "cube.h"
#include "draw.h"

#include "pattern_snake.h"
#include "pattern_fade3d.h"
#include "pattern_coloredge.h"
#include "pattern_drop.h"
#include "pattern_flash.h"


uint8_t mode;
clock_t timeMeasurementStart;

void cubeSetAnimationMode(uint8_t nextMode) {
	mode = nextMode;
}

void startTimeMeasurement(){
	timeMeasurementStart = clock();
}

void delayExecutionTimeCompensated(uint64_t uSec){
	clock_t diff = clock()-timeMeasurementStart;

	if (diff < uSec) {
		usleep(uSec - diff);
		printf("\033[m %u sleep %lu \n", (unsigned)diff, uSec - diff);	
	} else {
		printf("\033[31m timing failed!\n");
	}
}


void swapLedCubeBuffers() {
	
	led (*ledCubeTemp)[CUBE_DIM][CUBE_DIM][CUBE_DIM];

	pthread_mutex_lock(&ledCubeMutex);
	ledCubeTemp = showLedCube;
	showLedCube = drawLedCube;
	drawLedCube = ledCubeTemp;
	pthread_mutex_unlock(&ledCubeMutex);

	for (uint8_t x = 0; x < CUBE_DIM; x++) {
		for (int y = 0; y < CUBE_DIM; y++) {
			for (int z = 0; z < CUBE_DIM; z++) {
				(*drawLedCube)[x][y][z] = (*showLedCube)[x][y][z];
			}
		}
	}

}




void *cubeAnimator(void* void_ptr) {

	srand(time(NULL));
	drawInitColorPalette();

	while(1) {

		if (mode == 1) {

			fade3d_init();

			while (mode == 1) {
				fade3d_loop();
			}
		}
		else if (mode == 2) {
			
			snake_init();

			while (mode == 2) {
				snake_loop();
			}
		}
		else if (mode == 3) {

			coloredge_init();

			while (mode == 3) {
				coloredge_loop(&mode);		
			}
		}
		else if (mode == 4) {

			drop_init();

			while (mode == 4) {		
				drop_loop();	
			}
		}
		else if (mode == 5) {

			flash_init();

			while (mode == 5) {
				flash_loop(&mode);
			}
		}
		else {
			drawCuboidHSV(0, 0, 0, CUBE_DIM, CUBE_DIM, CUBE_DIM, black);
			swapLedCubeBuffers();
			usleep(50000);
		}
	}
}

void *cubePWMController(void* void_ptr) {
	   
	while(1) {

		for (uint8_t t = 0; t < (PWM_STEPS-1); t++) {

			for (int z = 0; z < CUBE_DIM; z++) {

				uint8_t zByteOffset = z * 18;	
				int8_t msgByte = 0;
				uint8_t byteNum = 0;

				pthread_mutex_lock(&ledCubeMutex);
				
					
				msgByte = ((*showLedCube)[0][0][z].g > t) << 4;
				msgByte |= ((*showLedCube)[0][0][z].b > t) << 5;
				msgByte |= ((*showLedCube)[0][0][z].r > t) << 6;

				msgByte |= ((*showLedCube)[1][0][z].g > t) << 0;
				msgByte |= ((*showLedCube)[1][0][z].b > t) << 1;
				msgByte |= ((*showLedCube)[1][0][z].r > t) << 2;

				sendMsg[BUF_PAYLOAD_BEGIN + byteNum + zByteOffset] = msgByte;
				byteNum++;

				msgByte = ((*showLedCube)[2][0][z].g > t) << 4;
				msgByte |= ((*showLedCube)[2][0][z].b > t) << 5;
				msgByte |= ((*showLedCube)[2][0][z].r > t) << 6;

				msgByte |= ((*showLedCube)[3][0][z].g > t) << 0;
				msgByte |= ((*showLedCube)[3][0][z].b > t) << 1;
				msgByte |= ((*showLedCube)[3][0][z].r > t) << 2;

				sendMsg[BUF_PAYLOAD_BEGIN + byteNum + zByteOffset] = msgByte;
				byteNum++;

				msgByte = ((*showLedCube)[4][0][z].g > t) << 4;
				msgByte |= ((*showLedCube)[4][0][z].b > t) << 5;
				msgByte |= ((*showLedCube)[4][0][z].r > t) << 6;

				msgByte |= ((*showLedCube)[5][0][z].g > t) << 0;
				msgByte |= ((*showLedCube)[5][0][z].b > t) << 1;
				msgByte |= ((*showLedCube)[5][0][z].r > t) << 2;

				sendMsg[BUF_PAYLOAD_BEGIN + byteNum + zByteOffset] = msgByte;
				byteNum++;

				msgByte = ((*showLedCube)[5][1][z].g > t) << 4;
				msgByte |= ((*showLedCube)[5][1][z].b > t) << 5;
				msgByte |= ((*showLedCube)[5][1][z].r > t) << 6;

				msgByte |= ((*showLedCube)[4][1][z].g > t) << 0;
				msgByte |= ((*showLedCube)[4][1][z].b > t) << 1;
				msgByte |= ((*showLedCube)[4][1][z].r > t) << 2;

				sendMsg[BUF_PAYLOAD_BEGIN + byteNum + zByteOffset] = msgByte;
				byteNum++;

				msgByte = ((*showLedCube)[3][1][z].g > t) << 4;
				msgByte |= ((*showLedCube)[3][1][z].b > t) << 5;
				msgByte |= ((*showLedCube)[3][1][z].r > t) << 6;

				msgByte |= ((*showLedCube)[2][1][z].g > t) << 0;
				msgByte |= ((*showLedCube)[2][1][z].b > t) << 1;
				msgByte |= ((*showLedCube)[2][1][z].r > t) << 2;

				sendMsg[BUF_PAYLOAD_BEGIN + byteNum + zByteOffset] = msgByte;
				byteNum++;

				msgByte = ((*showLedCube)[1][1][z].g > t) << 4;
				msgByte |= ((*showLedCube)[1][1][z].b > t) << 5;
				msgByte |= ((*showLedCube)[1][1][z].r > t) << 6;

				msgByte |= ((*showLedCube)[0][1][z].g > t) << 0;
				msgByte |= ((*showLedCube)[0][1][z].b > t) << 1;
				msgByte |= ((*showLedCube)[0][1][z].r > t) << 2;

				sendMsg[BUF_PAYLOAD_BEGIN + byteNum + zByteOffset] = msgByte;
				byteNum++;

				//y=2-------------------------

				msgByte = ((*showLedCube)[0][2][z].g > t) << 4;
				msgByte |= ((*showLedCube)[0][2][z].b > t) << 5;
				msgByte |= ((*showLedCube)[0][2][z].r > t) << 6;

				msgByte |= ((*showLedCube)[1][2][z].g > t) << 0;
				msgByte |= ((*showLedCube)[1][2][z].b > t) << 1;
				msgByte |= ((*showLedCube)[1][2][z].r > t) << 2;

				sendMsg[BUF_PAYLOAD_BEGIN + byteNum + zByteOffset] = msgByte;
				byteNum++;

				msgByte = ((*showLedCube)[2][2][z].g > t) << 4;
				msgByte |= ((*showLedCube)[2][2][z].b > t) << 5;
				msgByte |= ((*showLedCube)[2][2][z].r > t) << 6;

				msgByte |= ((*showLedCube)[3][2][z].g > t) << 0;
				msgByte |= ((*showLedCube)[3][2][z].b > t) << 1;
				msgByte |= ((*showLedCube)[3][2][z].r > t) << 2;

				sendMsg[BUF_PAYLOAD_BEGIN + byteNum + zByteOffset] = msgByte;
				byteNum++;

				msgByte = ((*showLedCube)[4][2][z].g > t) << 4;
				msgByte |= ((*showLedCube)[4][2][z].b > t) << 5;
				msgByte |= ((*showLedCube)[4][2][z].r > t) << 6;

				msgByte |= ((*showLedCube)[5][2][z].g > t) << 0;
				msgByte |= ((*showLedCube)[5][2][z].b > t) << 1;
				msgByte |= ((*showLedCube)[5][2][z].r > t) << 2;

				sendMsg[BUF_PAYLOAD_BEGIN + byteNum + zByteOffset] = msgByte;
				byteNum++;


				//y=3----------------------------------------

				msgByte = ((*showLedCube)[5][3][z].g > t) << 4;
				msgByte |= ((*showLedCube)[5][3][z].b > t) << 5;
				msgByte |= ((*showLedCube)[5][3][z].r > t) << 6;

				msgByte |= ((*showLedCube)[4][3][z].g > t) << 0;
				msgByte |= ((*showLedCube)[4][3][z].b > t) << 1;
				msgByte |= ((*showLedCube)[4][3][z].r > t) << 2;

				sendMsg[BUF_PAYLOAD_BEGIN + byteNum + zByteOffset] = msgByte;
				byteNum++;

				msgByte = ((*showLedCube)[3][3][z].g > t) << 4;
				msgByte |= ((*showLedCube)[3][3][z].b > t) << 5;
				msgByte |= ((*showLedCube)[3][3][z].r > t) << 6;

				msgByte |= ((*showLedCube)[2][3][z].g > t) << 0;
				msgByte |= ((*showLedCube)[2][3][z].b > t) << 1;
				msgByte |= ((*showLedCube)[2][3][z].r > t) << 2;

				sendMsg[BUF_PAYLOAD_BEGIN + byteNum + zByteOffset] = msgByte;
				byteNum++;

				msgByte = ((*showLedCube)[1][3][z].g > t) << 4;
				msgByte |= ((*showLedCube)[1][3][z].b > t) << 5;
				msgByte |= ((*showLedCube)[1][3][z].r > t) << 6;

				msgByte |= ((*showLedCube)[0][3][z].g > t) << 0;
				msgByte |= ((*showLedCube)[0][3][z].b > t) << 1;
				msgByte |= ((*showLedCube)[0][3][z].r > t) << 2;

				sendMsg[BUF_PAYLOAD_BEGIN + byteNum + zByteOffset] = msgByte;
				byteNum++;

				//y=4-------------------------

				msgByte = ((*showLedCube)[0][4][z].g > t) << 4;
				msgByte |= ((*showLedCube)[0][4][z].b > t) << 5;
				msgByte |= ((*showLedCube)[0][4][z].r > t) << 6;

				msgByte |= ((*showLedCube)[1][4][z].g > t) << 0;
				msgByte |= ((*showLedCube)[1][4][z].b > t) << 1;
				msgByte |= ((*showLedCube)[1][4][z].r > t) << 2;

				sendMsg[BUF_PAYLOAD_BEGIN + byteNum + zByteOffset] = msgByte;
				byteNum++;

				msgByte = ((*showLedCube)[2][4][z].g > t) << 4;
				msgByte |= ((*showLedCube)[2][4][z].b > t) << 5;
				msgByte |= ((*showLedCube)[2][4][z].r > t) << 6;

				msgByte |= ((*showLedCube)[3][4][z].g > t) << 0;
				msgByte |= ((*showLedCube)[3][4][z].b > t) << 1;
				msgByte |= ((*showLedCube)[3][4][z].r > t) << 2;

				sendMsg[BUF_PAYLOAD_BEGIN + byteNum + zByteOffset] = msgByte;
				byteNum++;

				msgByte = ((*showLedCube)[4][4][z].g > t) << 4;
				msgByte |= ((*showLedCube)[4][4][z].b > t) << 5;
				msgByte |= ((*showLedCube)[4][4][z].r > t) << 6;

				msgByte |= ((*showLedCube)[5][4][z].g > t) << 0;
				msgByte |= ((*showLedCube)[5][4][z].b > t) << 1;
				msgByte |= ((*showLedCube)[5][4][z].r > t) << 2;

				sendMsg[BUF_PAYLOAD_BEGIN + byteNum + zByteOffset] = msgByte;
				byteNum++;

				//y=5----------------------------------------

				msgByte = ((*showLedCube)[5][5][z].g > t) << 4;
				msgByte |= ((*showLedCube)[5][5][z].b > t) << 5;
				msgByte |= ((*showLedCube)[5][5][z].r > t) << 6;

				msgByte |= ((*showLedCube)[4][5][z].g > t) << 0;
				msgByte |= ((*showLedCube)[4][5][z].b > t) << 1;
				msgByte |= ((*showLedCube)[4][5][z].r > t) << 2;

				sendMsg[BUF_PAYLOAD_BEGIN + byteNum + zByteOffset] = msgByte;
				byteNum++;

				msgByte = ((*showLedCube)[3][5][z].g > t) << 4;
				msgByte |= ((*showLedCube)[3][5][z].b > t) << 5;
				msgByte |= ((*showLedCube)[3][5][z].r > t) << 6;

				msgByte |= ((*showLedCube)[2][5][z].g > t) << 0;
				msgByte |= ((*showLedCube)[2][5][z].b > t) << 1;
				msgByte |= ((*showLedCube)[2][5][z].r > t) << 2;

				sendMsg[BUF_PAYLOAD_BEGIN + byteNum + zByteOffset] = msgByte;
				byteNum++;

				msgByte = ((*showLedCube)[1][5][z].g > t) << 4;
				msgByte |= ((*showLedCube)[1][5][z].b > t) << 5;
				msgByte |= ((*showLedCube)[1][5][z].r > t) << 6;

				msgByte |= ((*showLedCube)[0][5][z].g > t) << 0;
				msgByte |= ((*showLedCube)[0][5][z].b > t) << 1;
				msgByte |= ((*showLedCube)[0][5][z].r > t) << 2;

				sendMsg[BUF_PAYLOAD_BEGIN + byteNum + zByteOffset] = msgByte;
				byteNum++;

				pthread_mutex_unlock(&ledCubeMutex);


			}

			pthread_mutex_lock(&etherFrameMutex);
			char* msgTemp = sendBuf;
			sendBuf = sendMsg;
			sendMsg = msgTemp;
			pthread_mutex_unlock(&etherFrameMutex);

			usleep(2);
		}

	}
}

void *ethSend(void* void_ptr) {

	initEthernet(INTERFACE);

    clock_t lastsend;
    clock_t current;
    clock_t diff;

    //lastsend = clock();
 
    while (1){
    //for (int i=0; i<1000; i++) {

    	pthread_mutex_lock(&etherFrameMutex);
	    sendto(sockfd, sendBuf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll));
	    pthread_mutex_unlock(&etherFrameMutex);
	    
	    usleep(5);

    //}

    /*
    current = clock();
    diff = current-lastsend;
    lastsend = current;

    
    if (diff > 45000)
        printf("\033[31m %u \n", (unsigned)diff);
    else if (diff < 15000)
        printf("\033[33m %u \n", (unsigned)diff);
    else 
        printf("\033[m %u \n", (unsigned)diff);
     */
    

    
    }
}



void initEthernet(char* ifName) {

	tx_len = 0;
	eh0 = (struct ether_header *) sendBuf;
	eh1 = (struct ether_header *) sendMsg;

	//open socket in raw mode
	if ((sockfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW)) == -1) {
	    perror("socket");
	}

	//get interface index
	memset(&if_idx, 0, sizeof(struct ifreq));
	strncpy(if_idx.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFINDEX, &if_idx) < 0)
	    perror("SIOCGIFINDEX");

	//get MAC of interface
	memset(&if_mac, 0, sizeof(struct ifreq));
	strncpy(if_mac.ifr_name, ifName, IFNAMSIZ-1);
	if (ioctl(sockfd, SIOCGIFHWADDR, &if_mac) < 0)
	    perror("SIOCGIFHWADDR");

    //init Ethernet header 
	memset(sendBuf, 0, BUF_SIZ);
	memset(sendMsg, 0, BUF_SIZ);

	//Source and Destination MAC
	eh0->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
	eh0->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
	eh0->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
	eh0->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
	eh0->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
	eh0->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
	eh0->ether_dhost[0] = FPGA_MAC_ADDR0;
	eh0->ether_dhost[1] = FPGA_MAC_ADDR1;
	eh0->ether_dhost[2] = FPGA_MAC_ADDR2;
	eh0->ether_dhost[3] = FPGA_MAC_ADDR3;
	eh0->ether_dhost[4] = FPGA_MAC_ADDR4;
	eh0->ether_dhost[5] = FPGA_MAC_ADDR5;

	eh0->ether_type = htons(0x006c);//Ethertype

	eh1->ether_shost[0] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[0];
	eh1->ether_shost[1] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[1];
	eh1->ether_shost[2] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[2];
	eh1->ether_shost[3] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[3];
	eh1->ether_shost[4] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[4];
	eh1->ether_shost[5] = ((uint8_t *)&if_mac.ifr_hwaddr.sa_data)[5];
	eh1->ether_dhost[0] = FPGA_MAC_ADDR0;
	eh1->ether_dhost[1] = FPGA_MAC_ADDR1;
	eh1->ether_dhost[2] = FPGA_MAC_ADDR2;
	eh1->ether_dhost[3] = FPGA_MAC_ADDR3;
	eh1->ether_dhost[4] = FPGA_MAC_ADDR4;
	eh1->ether_dhost[5] = FPGA_MAC_ADDR5;

	eh1->ether_type = htons(0x006c);//Ethertype

	tx_len += sizeof(struct ether_header);

	socket_address.sll_ifindex = if_idx.ifr_ifindex;
	socket_address.sll_halen = ETH_ALEN;

	//Destination MAC
	socket_address.sll_addr[0] = FPGA_MAC_ADDR0;
	socket_address.sll_addr[1] = FPGA_MAC_ADDR1;
	socket_address.sll_addr[2] = FPGA_MAC_ADDR2;
	socket_address.sll_addr[3] = FPGA_MAC_ADDR3;
	socket_address.sll_addr[4] = FPGA_MAC_ADDR4;
	socket_address.sll_addr[5] = FPGA_MAC_ADDR5;

    tx_len += 108;
}
