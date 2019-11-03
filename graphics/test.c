#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <sys/nearptr.h>
#include "graphics.h"
#include "input.h"

typedef struct playerPos{
int x;
int y;
}

void main()
{
	playerPos pos;
	int x, y, color;
	float t1, t2;
	word i, start;
	initDoubleBuffer();
	int speed=5;

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
		if (isPressed(Q))
			break;

		if (isPressed(W))
				pos.y-=speed;
		if (isPressed(S))
				pos.y+=speed;
		if (isPressed(D))
				pos.x+=speed;
		if (isPressed(A))
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
