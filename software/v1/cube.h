#ifndef _CUBE_H
#define _CUBE_H

#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/ether.h>

#define FPGA_MAC_ADDR0	0xaa
#define FPGA_MAC_ADDR1	0xaa
#define FPGA_MAC_ADDR2	0xaa
#define FPGA_MAC_ADDR3	0xaa
#define FPGA_MAC_ADDR4	0xaa
#define FPGA_MAC_ADDR5	0xaa

#define INTERFACE	"enp0s3"
#define BUF_SIZ		122

#define BUF_PAYLOAD_BEGIN	14
#define CUBE_DIM	6
#define PWM_STEPS	256

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} led;

typedef struct {
	uint16_t h;
	float s;
	float v;
} hsvColor;

led ledCube[CUBE_DIM][CUBE_DIM][CUBE_DIM];


int sockfd;
struct ifreq if_idx;
struct ifreq if_mac;
int tx_len;
char sendbuf[BUF_SIZ];
struct ether_header *eh;
struct sockaddr_ll socket_address;

void initEthernet();
void *ethSend(void *);
void *cubePWMController(void *);
void *cubeAnimator(void *);

void cubeSetAnimationMode(uint8_t nextMode);

void startTimeMeasurement();
void delayExecutionTimeCompensated(uint64_t uSec);




#endif
