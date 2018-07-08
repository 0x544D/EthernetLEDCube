#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cube.h"
#include "draw.h"

#include "pattern_snake.h"

#define DIRX 0
#define DIRNX 1
#define DIRY 2
#define DIRNY 3
#define DIRZ 4
#define DIRNZ 5


hsvColor color;
uint8_t currentMoveDir;
uint16_t posX;
uint16_t posY;
uint16_t posZ;


void snake_init() {

	color = green;

	currentMoveDir = (1 << DIRX);

	posX = rand() % CUBE_DIM;
	posY = rand() % CUBE_DIM;
	posZ = rand() % CUBE_DIM;

	drawCuboidHSV(0, 0, 0, CUBE_DIM, CUBE_DIM, CUBE_DIM, black);
	swapLedCubeBuffers();

}

void snake_loop() {
	startTimeMeasurement();
	(*drawLedCube)[posX][posY][posZ] = hsvToRGB(color);

	uint8_t possibleMoveDirs = checkPossibleMoveDirs(posX, posY, posZ);

	if (possibleMoveDirs) {

		if (!(possibleMoveDirs & currentMoveDir)/* || !(rand()%20)*/) {

			uint8_t nextDir;

			do {
				nextDir = rand() % 6;
			}
			while (!(possibleMoveDirs & (1 << nextDir)));

			currentMoveDir = (1 << nextDir);
		}

		if (currentMoveDir == (1 << DIRX)) {
			posX++;
		}
		else if (currentMoveDir == (1 << DIRNX)) {
			posX--;
		}
		else if (currentMoveDir == (1 << DIRY)) {
			posY++;
		}
		else if (currentMoveDir == (1 << DIRNY)) {
			posY--;
		}
		else if (currentMoveDir == (1 << DIRZ)) {
			posZ++;
		}
		else if (currentMoveDir == (1 << DIRNZ)) {
			posZ--;
		}
		else {
			printf("invalid moveDir!\n");
		}

		color.h = (color.h+1)%360;
	}
	else {

		for (uint8_t x = 0; x < CUBE_DIM; x++) {
			for (uint8_t y = 0; y < CUBE_DIM; y++) {
				for (uint8_t z = 0; z < CUBE_DIM; z++) {

					if ((*drawLedCube)[x][y][z].r > 3)
						(*drawLedCube)[x][y][z].r -= 4;
					else if ((*drawLedCube)[x][y][z].r > 0)
						(*drawLedCube)[x][y][z].r -= 1;

					if ((*drawLedCube)[x][y][z].g > 3)
						(*drawLedCube)[x][y][z].g -= 4;
					else if ((*drawLedCube)[x][y][z].g > 0)
						(*drawLedCube)[x][y][z].g -= 1;

					if ((*drawLedCube)[x][y][z].b > 3)
						(*drawLedCube)[x][y][z].b -= 4;
					else if ((*drawLedCube)[x][y][z].b > 0)
						(*drawLedCube)[x][y][z].b -= 1;
				}
			}
		}

		

	}

	swapLedCubeBuffers();
	delayExecutionTimeCompensated(50000);
}

uint8_t checkPossibleMoveDirs(uint16_t posX, uint16_t posY, uint16_t posZ){

	uint8_t moveDirs = 0;

	if (posX+1 < CUBE_DIM) {
		if ((*drawLedCube)[posX+1][posY][posZ].r == 0 && (*drawLedCube)[posX+1][posY][posZ].g == 0 && (*drawLedCube)[posX+1][posY][posZ].b == 0) {
			moveDirs |= (1 << DIRX);
		}
		
	}
	if (posX > 0) {
		if ((*drawLedCube)[posX-1][posY][posZ].r == 0 && (*drawLedCube)[posX-1][posY][posZ].g == 0 && (*drawLedCube)[posX-1][posY][posZ].b == 0) {
			moveDirs |= (1 << DIRNX);
		}
		
	}
	if (posY+1 < CUBE_DIM) {
		if ((*drawLedCube)[posX][posY+1][posZ].r == 0 && (*drawLedCube)[posX][posY+1][posZ].g == 0 && (*drawLedCube)[posX][posY+1][posZ].b == 0) {
			moveDirs |= (1 << DIRY);
		}
		
	}
	if (posY > 0) {
		if ((*drawLedCube)[posX][posY-1][posZ].r == 0 && (*drawLedCube)[posX][posY-1][posZ].g == 0 && (*drawLedCube)[posX][posY-1][posZ].b == 0) {
			moveDirs |= (1 << DIRNY);
		}
		
	}
	if (posZ+1 < CUBE_DIM) {
		if ((*drawLedCube)[posX][posY][posZ+1].r == 0 && (*drawLedCube)[posX][posY][posZ+1].g == 0 && (*drawLedCube)[posX][posY][posZ+1].b == 0) {
			moveDirs |= (1 << DIRZ);
		}
		
	}
	if (posZ > 0) {
		if ((*drawLedCube)[posX][posY][posZ-1].r == 0 && (*drawLedCube)[posX][posY][posZ-1].g == 0 && (*drawLedCube)[posX][posY][posZ-1].b == 0) {
			moveDirs |= (1 << DIRNZ);
		}
		
	}

	return moveDirs;
}