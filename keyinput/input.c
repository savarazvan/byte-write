#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <dos.h>
#include "input.h"

int isPressed(int key)
{
    if(getKey()==key)
        return 1;
    return 0;
}

char getKey()
{
    union REGS in, out;

    in.h.ah=0x8;
    int86(0x21,&in,&out);

    return out.h.al;
}

