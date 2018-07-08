#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cube.h"
#include "draw.h"

#include "pattern_coloredge.h"


uint8_t cubeColorNr;
hsvColor cubeColor;
hsvColor lineColor;


void coloredge_init() {

	cubeColorNr = rand()%(sizeof(colorPool)/sizeof(colorPool[0]));

	cubeColor = colorPool[cubeColorNr];
	drawCuboidHSV(0, 0, 0, CUBE_DIM, CUBE_DIM, CUBE_DIM, cubeColor);

	lineColor = cubeColor;
	lineColor.h = (lineColor.h + 60 + rand()%270)%360;

}

void coloredge_loop(uint8_t* modeSwitchIndicator) {

	for (int i=0; i < CUBE_DIM; i++) {
		startTimeMeasurement();

		drawLineHSV(0, 0, 0, i, 0, 0, lineColor);
		drawLineHSV(0, 0, 0, 0, i, 0, lineColor);
		drawLineHSV(0, 0, 0, 0, 0, i, lineColor);

		delayExecutionTimeCompensated(200000);
		if (*modeSwitchIndicator != 3) {
			return;
		}
	}


	for (int i=0; i < CUBE_DIM; i++) {
		startTimeMeasurement();

		drawLineHSV(CUBE_DIM-1, 0, CUBE_DIM-1, CUBE_DIM-1-i, 0, CUBE_DIM-1, lineColor);
		drawLineHSV(CUBE_DIM-1, 0, CUBE_DIM-1, CUBE_DIM-1, i, CUBE_DIM-1, lineColor);
		drawLineHSV(CUBE_DIM-1, 0, CUBE_DIM-1, CUBE_DIM-1, 0, CUBE_DIM-1-i, lineColor);

		delayExecutionTimeCompensated(200000);
		if (*modeSwitchIndicator != 3) {
			return;
		}
	}


	for (int i=0; i < CUBE_DIM; i++) {
		startTimeMeasurement();

		drawLineHSV(0, CUBE_DIM-1, CUBE_DIM-1, i, CUBE_DIM-1, CUBE_DIM-1, lineColor);
		drawLineHSV(0, CUBE_DIM-1, CUBE_DIM-1, 0, CUBE_DIM-1-i, CUBE_DIM-1, lineColor);
		drawLineHSV(0, CUBE_DIM-1, CUBE_DIM-1, 0, CUBE_DIM-1, CUBE_DIM-1-i, lineColor);

		delayExecutionTimeCompensated(200000);
		if (*modeSwitchIndicator != 3) {
			return;
		}
	}


	for (int i=0; i < CUBE_DIM; i++) {
		startTimeMeasurement();

		drawLineHSV(CUBE_DIM-1, CUBE_DIM-1, 0, CUBE_DIM-1-i, CUBE_DIM-1, 0, lineColor);
		drawLineHSV(CUBE_DIM-1, CUBE_DIM-1, 0, CUBE_DIM-1, CUBE_DIM-1-i, 0, lineColor);
		drawLineHSV(CUBE_DIM-1, CUBE_DIM-1, 0, CUBE_DIM-1, CUBE_DIM-1, i, lineColor);

		delayExecutionTimeCompensated(200000);
		if (*modeSwitchIndicator != 3) {
			return;
		}
	}


	hsvColor cubeColorTemp = cubeColor;

	for (int i=0; i < 200; i++) {

		cubeColorTemp.h = cubeColor.h + ((lineColor.h - cubeColor.h) / (double)200) * i;

		startTimeMeasurement();
		drawCuboidHSV(1, 1, 0, CUBE_DIM-2, CUBE_DIM-2, CUBE_DIM, cubeColorTemp);
		drawCuboidHSV(1, 0, 1, CUBE_DIM-2, 1, CUBE_DIM-2, cubeColorTemp);
		drawCuboidHSV(1, CUBE_DIM-1, 1, CUBE_DIM-2, 1, CUBE_DIM-2, cubeColorTemp);
		drawCuboidHSV(0, 1, 1, 1, CUBE_DIM-2, CUBE_DIM-2, cubeColorTemp);
		drawCuboidHSV(CUBE_DIM-1, 1, 1, 1, CUBE_DIM-2, CUBE_DIM-2, cubeColorTemp);

		delayExecutionTimeCompensated(10000);
		if (*modeSwitchIndicator != 3) {
			return;
		}
	}

	cubeColor = lineColor;
	lineColor.h = (lineColor.h + 60 + rand()%270)%360;

}

