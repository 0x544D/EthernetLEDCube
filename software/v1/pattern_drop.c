#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cube.h"
#include "draw.h"

#include "pattern_drop.h"


uint8_t zPos[CUBE_DIM][CUBE_DIM];
int8_t zLevel[CUBE_DIM][CUBE_DIM];

uint8_t colorNr;
led color;

uint8_t emptying;



void drop_init() {

	zPos[CUBE_DIM][CUBE_DIM];
	zLevel[CUBE_DIM][CUBE_DIM];


	colorNr = rand()%(sizeof(colorPool)/sizeof(colorPool[0]));
	color = hsvToRGB(colorPool[colorNr]);



	emptying = 0;


	drawCuboidHSV(0, 0, 0, CUBE_DIM, CUBE_DIM, CUBE_DIM, black);

	for (uint8_t x = 0; x < CUBE_DIM; x++) {
		for (uint8_t y = 0; y < CUBE_DIM; y++) {
			zPos[x][y] = rand()%80;
			zLevel[x][y] = 0;
		}
	}

}

void drop_loop() {


	if (!emptying) {

		startTimeMeasurement();

		uint8_t allLevelsFilled = 1;

		for (uint8_t x = 0; x < CUBE_DIM; x++) {
			for (uint8_t y = 0; y < CUBE_DIM; y++) {

				if (zLevel[x][y] < CUBE_DIM) {

					if (zPos[x][y] < CUBE_DIM) {
						for (uint8_t z = zLevel[x][y]; z < CUBE_DIM; z++) {
							if (z == zPos[x][y]) {
								ledCube[x][y][z] = color;
								if (zLevel[x][y] == z) {
									zLevel[x][y]++;
								}
							}
							else {
								ledCube[x][y][z] = blackRGB;
							}
						}
					}


					if (zPos[x][y] > 0) {
						zPos[x][y]--;
					}
					else {
						zPos[x][y] = rand()%80;
					}

					allLevelsFilled = 0;
				}
			}
		}

		if (allLevelsFilled) {
			emptying = 1;

			delayExecutionTimeCompensated(500000);
			startTimeMeasurement();
			for (uint8_t x = 0; x < CUBE_DIM; x++) {
				for (uint8_t y = 0; y < CUBE_DIM; y++) {
					zLevel[x][y] = -1;
					zPos[x][y] = rand()%80;
				}
			}
		}
		delayExecutionTimeCompensated(30000);
	}
	else {

		startTimeMeasurement();

		uint8_t allLevelsFilled = 1;

		for (uint8_t x = 0; x < CUBE_DIM; x++) {
			for (uint8_t y = 0; y < CUBE_DIM; y++) {

				if (zLevel[x][y] < CUBE_DIM-1) {

					for (uint16_t z = 0; z <= zLevel[x][y]; z++) {


						if (z == zPos[x][y]) {
							ledCube[x][y][z] = color;
						}
						else {
							ledCube[x][y][z] = blackRGB;
						}
					}

					if (zPos[x][y] > 0) {
						zPos[x][y]--;
					}
					else {
						zPos[x][y] = rand()%80 + zLevel[x][y];			
						zLevel[x][y]++;
					}

					allLevelsFilled = 0;
				}
				else if (zLevel[x][y] >= CUBE_DIM-1) {
					ledCube[x][y][5] = blackRGB;
					ledCube[x][y][0] = blackRGB;
				}
				else ledCube[x][y][0] = blackRGB;
			}
		}

		if (allLevelsFilled) {
			emptying = 0;

			for (uint8_t x = 0; x < CUBE_DIM; x++) {
				for (uint8_t y = 0; y < CUBE_DIM; y++) {
					zLevel[x][y] = 0;
					zPos[x][y] = rand()%80;
					colorNr = rand()%(sizeof(colorPool)/sizeof(colorPool[0]));
					color = hsvToRGB(colorPool[colorNr]);
				}
			}
			delayExecutionTimeCompensated(500000);
			startTimeMeasurement();

		}
		delayExecutionTimeCompensated(20000);
	}	

}

