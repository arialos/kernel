#ifndef VGA_H
#define VGA_H
 
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
 
static inline uint8_t vgaEntryColor(enum vgaColor fg, enum vgaColor bg) {
	return fg | bg << 4;
}
 
static inline uint16_t vgaEntry(unsigned char uc, uint8_t color) {
	return (uint16_t) uc | (uint16_t) color << 8;
}
 
#endif