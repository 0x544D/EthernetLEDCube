#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cube.h"
#include "draw.h"

#include "pattern_fade3d.h"

int8_t fadedir;
int32_t stepcounter;

hsvColor color;


void fade3d_init() {

	fadedir = 1;
	stepcounter = 0;

	color = red;


}

void fade3d_loop() {

	startTimeMeasurement();

	hsvColor colorTemp, yColor, xColor;

	for (int z = 0; z < CUBE_DIM; z++) {

		colorTemp = color;
		colorTemp.h = (colorTemp.h + z*10) % 360;

		for (int y = 0; y < CUBE_DIM; y++) {
			yColor = colorTemp;
			yColor.h = (yColor.h + y*10) % 360;

			for (int x = 0; x < CUBE_DIM; x++) {
				xColor = yColor;
				xColor.h = (xColor.h + x*10) % 360;
				(*drawLedCube)[x][y][z] = hsvToRGB(xColor);

			}
		}
	}

	if (fadedir < 0 && color.h == 0) {
		color.h = 359;
	}
	else {
		color.h = (color.h + fadedir)%360;
	}
	stepcounter++;

	if (stepcounter > 100 && !(rand()%500)){
		fadedir = -fadedir;
		stepcounter = 0;
	}

	swapLedCubeBuffers();
	delayExecutionTimeCompensated(2000);	

}

