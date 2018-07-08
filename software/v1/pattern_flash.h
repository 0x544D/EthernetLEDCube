#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cube.h"
#include "draw.h"



#ifndef _FLASH_H
#define _FLASH_H

void flash_init();
void flash_loop(uint8_t* modeSwitchIndicator);

#endif
