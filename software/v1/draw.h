#ifndef _DRAW_H
#define _DRAW_H

#include "cube.h"

	led redRGB;
	led greenRGB;
	led blueRGB;

	led yellowRGB;
	led magentaRGB;
	led cyanRGB;

	led whiteRGB;
	led blackRGB;

	hsvColor red;
	hsvColor yellow;
	hsvColor green;
	hsvColor cyan;
	hsvColor blue;
	hsvColor purple;
	hsvColor magenta;

	hsvColor black;
	hsvColor white;

	hsvColor colorPool[7];

void drawInitColorPalette();
void drawLine(uint8_t x0, uint8_t y0, uint8_t z0, uint8_t x1, uint8_t y1, uint8_t z1, led color);
void drawLineHSV(uint8_t x0, uint8_t y0, uint8_t z0, uint8_t x1, uint8_t y1, uint8_t z1, hsvColor hsv);
void drawCuboid(uint8_t x0, uint8_t y0, uint8_t z0, uint8_t x1, uint8_t y1, uint8_t z1, led color);
void drawCuboidHSV(uint8_t x0, uint8_t y0, uint8_t z0, uint8_t x1, uint8_t y1, uint8_t z1, hsvColor color);
led hsvToRGB(hsvColor hsv);

#endif
