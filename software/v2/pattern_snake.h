#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cube.h"
#include "draw.h"



#ifndef _SNAKE
#define _SNAKE

void snake_init();
void snake_loop();
uint8_t checkPossibleMoveDirs(uint16_t posX, uint16_t posY, uint16_t posZ);

#endif
