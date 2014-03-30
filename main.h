#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Windows.h>
#define M_PI (4*atanf(1))
#define square(x) ((x)*(x))
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#define MAX_PROJECTILES 50
#define MAX_ROCKS 50
#define MISSILE 2
#define ROCK 3
#define ACCELSPEED 20 

typedef struct global{
	int windowHeight;
	int windowWidth;
	float difTime;
	float pointX;
	float pointY;
	int Akey,Skey,Dkey,Wkey,Qkey;
	int spaceKey;
	int pType;
	int score;
	int deaths;
	int Inv;
}Global;

Global global;

typedef struct triangle{
	float angleA,angleB,angleC;
	float a,b,c;
}Triangle;

typedef struct triangle * TrianglePtr;

void setView();
void drawPitch();
void drawShips();
void drawTriangle(float,float);
void drawPoint(float x, float y);
void drawLine(float,float,float,float);
void key(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void idle();
void update();
int randomMax(int);
void drawText(char* text,int x,int y);
void drawGUI();
void drawCircle(float r, float xPos, float yPos);