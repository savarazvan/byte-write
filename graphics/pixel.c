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
word* my_clock = (word*)0x046C;      /* this points to the 18.2hz system
										 clock. */

										 /**************************************************************************
										  *  set_mode                                                              *
										  *     Sets the video mode.                                               *
										  **************************************************************************/

byte* doubleBuffer;
int* collisions;

void* initDoubleBuffer()
{
	doubleBuffer = (byte*)malloc(320 * 200);
}

int speed = 5;

typedef struct tagBITMAP              /* the structure for a bitmap. */
{
	word width;
	word height;
	byte* data;
} BITMAP;

typedef struct tagplayerPos
{
	word x;
	word y;
} playerPos;


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
/**************************************************************************
 *  plot_pixel_fast                                                       *
 *    Plot a pixel by directly writing to video memory.                   *
 **************************************************************************/

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

void load_bmp(char* file, BITMAP* b)
{
	FILE* fp;
	long index;
	word num_colors;
	int x;

	/* open the file */
	if ((fp = fopen(file, "rb")) == NULL)
	{
		printf("Error opening file %s.\n", file);
		exit(1);
	}

	/* check to see if it is a valid bitmap file */
	if (fgetc(fp) != 'B' || fgetc(fp) != 'M')
	{
		fclose(fp);
		printf("%s is not a bitmap file.\n", file);
		exit(1);
	}

	/* read in the width and height of the image, and the
	   number of colors used; ignore the rest */
	fskip(fp, 16);
	fread(&b->width, sizeof(word), 1, fp);
	fskip(fp, 2);
	fread(&b->height, sizeof(word), 1, fp);
	fskip(fp, 22);
	fread(&num_colors, sizeof(word), 1, fp);
	fskip(fp, 6);

	/* assume we are working with an 8-bit file */
	if (num_colors == 0) num_colors = 256;


	/* try to allocate memory */
	if ((b->data = (byte*)malloc((word)(b->width * b->height))) == NULL)
	{
		fclose(fp);
		printf("Error allocating memory for file %s.\n", file);
		exit(1);
	}

	/* Ignore the palette information for now.
	   See palette.c for code to read the palette info. */
	fskip(fp, num_colors * 4);

	/* read the bitmap */
	for (index = (b->height - 1) * b->width; index >= 0; index -= b->width)
		for (x = 0; x < b->width; x++)
			b->data[(word)index + x] = (byte)fgetc(fp);

	fclose(fp);
}

/**************************************************************************
 *  draw_bitmap                                                           *
 *    Draws a bitmap.                                                     *
 **************************************************************************/

void draw_bitmap(BITMAP* bmp, int x, int y)
{
	int j;
	word screen_offset = (y << 8) + (y << 6) + x;
	word bitmap_offset = 0;

	for (j = 0; j < bmp->height; j++)
	{
		memcpy(&VGA[screen_offset], &bmp->data[bitmap_offset], bmp->width);

		bitmap_offset += bmp->width;
		screen_offset += SCREEN_WIDTH;
	}
}



/**************************************************************************
 *  draw_transparent_bitmap                                               *
 *    Draws a transparent bitmap.                                         *
 **************************************************************************/

void draw_transparent_bitmap(BITMAP* bmp, int x, int y)
{
	int i, j;
	word screen_offset = (y << 8) + (y << 6);
	word bitmap_offset = 0;
	byte data;

	for (j = 0; j < bmp->height; j++)
	{
		for (i = 0; i < bmp->width; i++, bitmap_offset++)
		{
			data = bmp->data[bitmap_offset];
			if (data) VGA[screen_offset + x + i] = data;
		}
		screen_offset += SCREEN_WIDTH;
	}
}



void main()
{
	playerPos pos;
	int x, y, color;
	float t1, t2;
	word i, start;
	initDoubleBuffer();
	
	if (doubleBuffer == NULL)
	{
		printf("Not enough physical memory for the double buffer!\n");
		exit(-1);
	}

	
	VGA += __djgpp_conventional_base;

	set_mode(VGA_256_COLOR_MODE);       /* set the video mode. */

	if (__djgpp_nearptr_enable() == 0)
		return;

	plot_pixel(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 2);
	pos.x = SCREEN_WIDTH / 2;
	pos.y = SCREEN_HEIGHT / 2;


	while (1)
	{
		if (getch() == 'q')
			break;

		if (getch() == 'w')
				pos.y-=speed;
		if (getch() == 's')
				pos.y+=speed;
		if (getch() == 'd')
				pos.x+=speed;
		if (getch() == 'a')
				pos.x-=speed;
		
		if (pos.x >= SCREEN_WIDTH)
			pos.x = 1;
		else if (pos.x <= 0)
			pos.x = SCREEN_WIDTH - 1;
		if (pos.y >= SCREEN_HEIGHT)
			pos.y = 1;
		else if (pos.y < 0)
			pos.y = SCREEN_HEIGHT - 1;

		plot_pixel(pos.x, pos.y, 2);

	}
	

	__djgpp_nearptr_disable();

	set_mode(TEXT_MODE);

	return;
}
