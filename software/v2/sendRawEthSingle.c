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

#define FPGA_MAC_ADDR0	0xaa
#define FPGA_MAC_ADDR1	0xaa
#define FPGA_MAC_ADDR2	0xaa
#define FPGA_MAC_ADDR3	0xaa
#define FPGA_MAC_ADDR4	0xaa
#define FPGA_MAC_ADDR5	0xaa

#define INTERFACE	"enp3s0"
#define BUF_SIZ		122

#define BUF_PAYLOAD_BEGIN	14



int sockfd;
struct ifreq if_idx;
struct ifreq if_mac;
int tx_len = 0;
char sendbuf[BUF_SIZ];
struct ether_header *eh = (struct ether_header *) sendbuf;
struct sockaddr_ll socket_address;
char ifName[] = INTERFACE;

void initEthernet();



int main(int argc, char *argv[])
{
	
	srand(time(NULL));
    initEthernet();

	for (int i=0; i<108; i++) {
		sendbuf[BUF_PAYLOAD_BEGIN + i] = 0x00;
	}
	sendbuf[BUF_PAYLOAD_BEGIN + 0] = 0x21;
	sendbuf[BUF_PAYLOAD_BEGIN + 1] = 0x04;

	sendto(sockfd, sendbuf, tx_len, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll));

	return 0;
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


