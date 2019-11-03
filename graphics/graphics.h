#include <stdio.h>
#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED


#define VIDEO_INT           0x10      /* the BIOS video interrupt. */
#define WRITE_DOT           0x0C      /* BIOS func to plot a pixel. */
#define SET_MODE            0x00      /* BIOS func to set the video mode. */
#define VGA_256_COLOR_MODE  0x13      /* use to set 256-color mode. */
#define TEXT_MODE           0x03      /* use to set 80x25 text mode. */

#define SCREEN_WIDTH        320       /* width in pixels of mode 0x13 */
#define SCREEN_HEIGHT       200       /* height in pixels of mode 0x13 */
#define NUM_COLORS          256       /* number of colors in mode 0x13 */


typedef unsigned char  byte;
typedef unsigned short word;
extern byte *VGA;
extern byte *doubleBuffer;

int sgn(int n);
void initDoubleBuffer();
void fskip(FILE* fp, int num_bytes);
void set_mode(byte);
void plot_pixel(int x, int y, byte color);
void plot_line(int x1, int y1, int x2, int y2, byte color);


#endif // GRAPHICS_H_INCLUDED
