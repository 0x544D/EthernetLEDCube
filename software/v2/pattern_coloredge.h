#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cube.h"
#include "draw.h"



#ifndef _COLOREDGE
#define _COLOREDGE

void coloredge_init();
void coloredge_loop(uint8_t* modeSwitchIndicator);

#endif
