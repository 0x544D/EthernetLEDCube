#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cube.h"
#include "draw.h"

#include "pattern_flash.h"


hsvColor color;

led colorRGB;



uint16_t ledCounter;



void flash_init() {

	color.h = 48;
	color.s = 0;
	color.v = 1;

	colorRGB = hsvToRGB(color);

	ledCounter = 0;
}

void flash_loop(uint8_t* modeSwitchIndicator) {

	drawCuboidHSV(0, 0, 0, CUBE_DIM, CUBE_DIM, CUBE_DIM, black);

	while (ledCounter < CUBE_DIM*CUBE_DIM*CUBE_DIM) {
		startTimeMeasurement();
		uint8_t x, y, z;
		do {
			x = rand()%CUBE_DIM;
			y = rand()%CUBE_DIM;
			z = rand()%CUBE_DIM;

		} while (ledCube[x][y][z].r == colorRGB.r && ledCube[x][y][z].g == colorRGB.g && ledCube[x][y][z].b == colorRGB.b);
		ledCube[x][y][z] = colorRGB;
		ledCounter++;
		delayExecutionTimeCompensated(3000);


		if (*modeSwitchIndicator != 5) {
			return;
		}
	}


	if (*modeSwitchIndicator != 5) {
		return;
	}

	usleep(50000);
	ledCounter = 0;
	startTimeMeasurement();
	drawCuboidHSV(0, 0, 0, CUBE_DIM, CUBE_DIM, CUBE_DIM, black);
	delayExecutionTimeCompensated(100000);
	startTimeMeasurement();
	drawCuboidHSV(0, 0, 0, CUBE_DIM, CUBE_DIM, CUBE_DIM, color);
	delayExecutionTimeCompensated(100000);
	startTimeMeasurement();
	drawCuboidHSV(0, 0, 0, CUBE_DIM, CUBE_DIM, CUBE_DIM, black);
	delayExecutionTimeCompensated(100000);
	startTimeMeasurement();
	if (*modeSwitchIndicator != 5) {
		return;
	}
	drawCuboidHSV(0, 0, 0, CUBE_DIM, CUBE_DIM, CUBE_DIM, color);
	delayExecutionTimeCompensated(100000);
	startTimeMeasurement();
	drawCuboidHSV(0, 0, 0, CUBE_DIM, CUBE_DIM, CUBE_DIM, black);
	delayExecutionTimeCompensated(100000);

	hsvColor colorTemp = color;

	for (int i = 200; i > 0; i--) {

		colorTemp.v -= 0.0005;  
		colorTemp.s += 0.005;

		startTimeMeasurement();
		drawCuboidHSV(0, 0, 0, CUBE_DIM, CUBE_DIM, CUBE_DIM, colorTemp);
		delayExecutionTimeCompensated(500);
		if (*modeSwitchIndicator != 5) {
			return;
		}
	}
	for (int i = 20; i > 0; i--) {

		colorTemp.v -= 0.0005;
		colorTemp.h--;

		startTimeMeasurement();
		drawCuboidHSV(0, 0, 0, CUBE_DIM, CUBE_DIM, CUBE_DIM, colorTemp);
		delayExecutionTimeCompensated(10000);
		if (*modeSwitchIndicator != 5) {
			return;
		}
	}
	while (colorTemp.v > 0) {

		colorTemp.v -= 0.001;  

		startTimeMeasurement();
		drawCuboidHSV(0, 0, 0, CUBE_DIM, CUBE_DIM, CUBE_DIM, colorTemp);
		delayExecutionTimeCompensated(1000);
		if (*modeSwitchIndicator != 5) {
			return;
		}
	}


}

