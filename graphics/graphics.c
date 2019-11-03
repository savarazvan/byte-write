#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <sys/nearptr.h>
#include <unistd.h>
#include <string.h>
#include <conio.h>

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


byte* VGA = (byte*)0xA0000;          /* this points to video memory. */

byte* doubleBuffer;

void initDoubleBuffer()
{
	doubleBuffer = (byte*)malloc(320 * 200);
}

void fskip(FILE* fp, int num_bytes)
{
	int i;
	for (i = 0; i < num_bytes; i++)
		fgetc(fp);
}


void set_mode(byte mode)
{
	union REGS regs;

	regs.h.ah = SET_MODE;
	regs.h.al = mode;
	int86(VIDEO_INT, &regs, &regs);
}

void plot_pixel(int x, int y, byte color)
{
	memset(doubleBuffer, 1, 320*200);
	doubleBuffer[y * SCREEN_WIDTH + x] = color;
	for (int i = 50; i <= 100; i++)
		doubleBuffer[20 * SCREEN_WIDTH + i] = 3;
	memcpy(VGA, doubleBuffer, 320 * 200);
}

int sgn(int n)
{
	if (n > 0)
		return 1;
	else if (n < 0)
		return -1;
	return 0;
}

void plot_line(int x1, int y1, int x2, int y2, byte color)
{
	int dx = x2 - x1, dy = y2 - y1; /* Diferentele dintre axele punctele x2, x1 si y2,y1*/
	float slope;

	/*Daca axa x este cea majora*/
	if (abs(dx) > abs(dy))
	{
		int sign = sgn(dx); /* Semnul lui dx */
		slope = (float)dy / (float)dx; /*Panta liniei*/

		for (int i = 0; i != dx; i+=sign)
		{
			int px = i + x1;
			int py = slope * i + y1;
			plot_pixel(px, py, color);
		}
	}

	/*Daca axa y este cea majora*/
	else
	{
		int sign = sgn(dy);
		slope = (float)dx / (float)dy;

		for (int i = 0; i != dy; i+=sign)
		{
			int px = slope * i + x1;
			int py = i + y1;
			plot_pixel(px, py, color);
		}
	}
}
