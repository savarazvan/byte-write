#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include "input.h"

unsigned short int keyPressed[128];

void getInput()
{
    keyPressed={0};
    unsigned short int currentKey;

    while((currentKey=getch())!= -1)
    {
        if(currentKey>=65 && currentKey<=90) /*Daca e o litera mare*/
            currentKey+=32;

        keyPressed[currentKey]=1;
    }
}

int isPressed(int key)
{
    if(keyPressed[key])
        return 1;
    return 0;
}

