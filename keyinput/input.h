#ifndef INPUT_H_INCLUDED
#define INPUT_H_INCLUDED

#define ESCAPE 27
#define SPACE 32
#define A 65
#define B 66
#define C 67
#define D 68
#define E 69
#define F 70
#define G 71
#define H 72
#define I 73
#define J 74
#define K 75
#define L 76
#define M 77
#define N 78
#define O 79
#define P 80
#define Q 81
#define R 82
#define S 83
#define T 84
#define U 85
#define V 86
#define W 87
#define X 88
#define Y 89
#define Z 90

extern unsigned short int keyPressed[128];
void getInput();
int isPressed(int key);

#endif // INPUT_H_INCLUDED
