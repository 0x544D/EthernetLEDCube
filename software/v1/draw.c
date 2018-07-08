#include "cube.h"
#include "draw.h"

#include <math.h>


void drawInitColorPalette() {

	redRGB.r = 255;
	redRGB.g = 0;
	redRGB.b = 0;

	red.h = 0;
	red.s = 1;
	red.v = 1;

	yellow.h = 60;
	yellow.s = 1;
	yellow.v = 1;

	green.h = 120;
	green.s = 1;
	green.v = 1;

	cyan.h = 180;
	cyan.s = 1;
	cyan.v = 1;

	blue.h = 240;
	blue.s = 1;
	blue.v = 1;

	purple.h = 270;
	purple.s = 1;
	purple.v = 1;

	magenta.h = 300;
	magenta.s = 1;
	magenta.v = 1;

	black.h = 0;
	black.s = 0;
	black.v = 0;

	white.h = 0;
	white.s = 0;
	white.v = 1;


	colorPool[0] = red;
	colorPool[1] = yellow;
	colorPool[2] = green;
	colorPool[3] = cyan;
	colorPool[4] = blue;
	colorPool[5] = purple;
	colorPool[6] = cyan;

}

led hsvToRGB(hsvColor hsv){

	led rgbValue;
	uint16_t hi = hsv.h / 60;

	float f = ((float)hsv.h / (float)60)-hi;
	float p = hsv.v * (1-hsv.s);
	float q = hsv.v * (1-hsv.s*f);
	float t = hsv.v * (1-hsv.s*(1-f));

	if (hi == 0 || hi == 6) {
		rgbValue.r = hsv.v*255;
		rgbValue.g = t*255;
		rgbValue.b = p*255;
	}
	else if (hi == 1) {
		rgbValue.r = q*255;
		rgbValue.g = hsv.v*255;
		rgbValue.b = p*255;
	}
	else if (hi == 2) {
		rgbValue.r = p*255;
		rgbValue.g = hsv.v*255;
		rgbValue.b = t*255;
	}
	else if (hi == 3) {
		rgbValue.r = p*255;
		rgbValue.g = q*255;
		rgbValue.b = hsv.v*255;
	}
	else if (hi == 4) {
		rgbValue.r = t*255;
		rgbValue.g = p*255;
		rgbValue.b = hsv.v*255;
	}
	else if (hi == 5) {
		rgbValue.r = hsv.v*255;
		rgbValue.g = p*255;
		rgbValue.b = q*255;
	}

	return rgbValue;
}


void drawLine(uint8_t x0, uint8_t y0, uint8_t z0, uint8_t x1, uint8_t y1, uint8_t z1, led color) {
	
	if (x0 > x1) {
		uint8_t c = x0;
		x0 = x1;
		x1 = c;
	}
	if (y0 > y1) {
		uint8_t c = y0;
		y0 = y1;
		y1 = c;
	}
	if (z0 > z1) {
		uint8_t c = z0;
		z0 = z1;
		z1 = c;
	}

	double diffX = x1-x0;
	double diffY = y1-y0;
	double diffZ = z1-z0;

	double mXY = 0;
	double mXZ = 0;
	double mYZ = 0;


	if (diffX >= diffY && diffX >= diffZ) {

		if (diffX > 0) {
			mXY = diffY / diffX;
			mXZ = diffZ / diffX;
		}

		for (uint8_t x = x0; x <= x1; x++) {

			uint8_t y = round(mXY * x) + y0;
			uint8_t z = round(mXZ * x) + z0;

			ledCube[x][y][z] = color;
		}
	}
	else if (diffY >= diffZ && diffY >= diffX) {

		if (diffY > 0) {
			mXY = diffX / diffY;
			mYZ = diffZ / diffY;
		}

		for (uint8_t y = y0; y <= y1; y++) {
			uint8_t x = round(mXY * y) + x0;
			uint8_t z = round(mYZ * y) + z0;

			ledCube[x][y][z] = color;
		}
	}
	else {

		if (diffZ > 0) {
			mXZ = diffX / diffZ;
			mYZ = diffY / diffZ;
		}

		for (uint8_t z = z0; z <= z1; z++) {

			uint8_t y = round(mXZ * z) + y0;
			uint8_t x = round(mYZ * z) + x0;

			ledCube[x][y][z] = color;
		}
	}
	
}

void drawLineHSV(uint8_t x0, uint8_t y0, uint8_t z0, uint8_t x1, uint8_t y1, uint8_t z1, hsvColor hsv) {

	led color = hsvToRGB(hsv);
	
	drawLine(x0, y0, z0, x1, y1, z1, color);
}

void drawCuboid(uint8_t x0, uint8_t y0, uint8_t z0, uint8_t xSize, uint8_t ySize, uint8_t zSize, led color) {

	for (uint8_t x = x0; x < x0+xSize; x++) {
		for (int y = y0; y < y0+ySize; y++) {
			for (int z = z0; z < z0+zSize; z++) {
				ledCube[x][y][z] = color;
			}
		}
	}
	
}

void drawCuboidHSV(uint8_t x0, uint8_t y0, uint8_t z0, uint8_t xSize, uint8_t ySize, uint8_t zSize, hsvColor hsv) {

	led color = hsvToRGB(hsv);

	for (uint8_t x = x0; x < x0+xSize; x++) {
		for (int y = y0; y < y0+ySize; y++) {
			for (int z = z0; z < z0+zSize; z++) {
				ledCube[x][y][z] = color;
			}
		}
	}
	
}