#ifndef TTY_H
#define TTY_H
 
#include <stdint.h>
 
enum vgaColor {
	vgaColorBlack = 0,
	vgaColorBlue = 1,
	vgaColorGreen = 2,
	vgaColorCyan = 3,
	vgaColorRed = 4,
	vgaColorMagenta = 5,
	vgaColorBrown = 6,
	vgaColorLightGrey = 7,
	vgaColorDarkGrey = 8,
	vgaColorLightBlue = 9,
	vgaColorLightGreen = 10,
	vgaColorLightCyan = 11,
	vgaColorLightRed = 12,
	vgaColorLightMagenta = 13,
	vgaColorLightBrown = 14,
	vgaColorWhite = 15,
};
 
#define ttyColor(fg, bg) ((bg << 4) | fg )
#define ttyEntry(c, color) ((uint16_t) (c | (color << 8)))

void ttyClear ();
void ttyPutString(const char *data);
bool initTty();

#endif