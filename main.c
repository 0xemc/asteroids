#include "main.h"
#include "ship.h"
#include "projectile.h"
//#define DEBUG

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawPitch();
	update();
	updateProjectiles();
	drawShips();
	drawAllProjectiles();
	drawGUI();
	glutSwapBuffers();
}

void main(int argc, char** argv){
	glutInit(&argc, argv);
	glutInitWindowSize(600,600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Asteroids");
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	global.windowHeight=600;
	global.windowWidth=600;
	global.pType=ROCK;
	global.score=0;
	global.deaths=0;\
	global.Inv=FALSE;
	global.spaceKey=FALSE;
	ship1=(ShipPtr)malloc(sizeof(struct ship));
	ship1->xVec=0;
	ship1->yVec=0;
	ship1->xPos=0.5;
	ship1->yPos=0.5;
	ship1->angle=0;
	setView();
#ifndef DEBUG
	initialiseProjectileArray();
#endif
	glutDisplayFunc(display);
	glutKeyboardFunc(key);
	glutKeyboardUpFunc(keyUp);
	glutIdleFunc(idle);
	glutMainLoop();
}

void drawPitch(){
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glBegin(GL_QUADS);
	glVertex2f(0,0);
	glVertex2f(0,1);
	glVertex2f(1,1);
	glVertex2f(1,0);
	glEnd();
	glPopMatrix();
}

void setView(){
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glScalef(2,2,2);
	glTranslatef(-0.5,-0.5,0);
	glViewport(0, 0, global.windowWidth, global.windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	/*gluPerspective(90,global.windowWidth/global.windowHeight,0.1,1000);*/
	glOrtho(0,0,1,1,0,1000);
}

void idle(){
	float time;
	static float lastTime=0.0;
	static float asteroidTimer=0.0;
	static float bulletTimer=0.0;
	static float invTimer=0.0;
	projectilePtr p;
	time=glutGet(GLUT_ELAPSED_TIME);
	time/=1000.0;
	global.difTime=time-lastTime;
	asteroidTimer+=global.difTime;
	bulletTimer+=global.difTime;
	if(global.Inv==TRUE)
		invTimer+=global.difTime;
	if(invTimer>2){
		invTimer=0;
		global.Inv=FALSE;
	}
	if(asteroidTimer>3){
		p=createProjectile();
		asteroidTimer=0;
	}
	if(global.difTime<1.0/60){
		Sleep((((float)1.0/60)-global.difTime)*1000);
	}
	if(global.spaceKey==TRUE&&bulletTimer>0.1){
	global.pType=MISSILE;
	createProjectile();
	global.pType=ROCK;
	bulletTimer=0;
	}
	lastTime=time;
	glutPostRedisplay();
}

void drawShips(){
	static int count=0;
	if(global.Inv==TRUE){
	count++;
		if(count>5){
		count=0;
		drawShip(ship1);
		}
	}
	else{
		drawShip(ship1);
	}
}

void drawTriangle(float x,float y){
	glPushMatrix();
	glBegin(GL_TRIANGLES);
	glVertex2f(x-0.1,y-0.1);
	glVertex2f(x+0.1,y-0.1);
	glVertex2f(x,y+0.1);
	glEnd();
	glPopMatrix();
}

void drawPoint(float x,float y){
	glPushMatrix();
	glBegin(GL_POINTS);
	glVertex2f(x,y);
	glEnd();
	glPopMatrix();
}

void drawLine(float x, float y,float x2, float y2){
	glPushMatrix();
	glBegin(GL_LINES);
	glVertex2f(x,y);
	glVertex2f(x2,y2);
	glEnd();
	glPopMatrix();
}

void key(unsigned char key, int x, int y){
	switch(key){
	case 'd': 
		global.Dkey=TRUE;
		break;
	case 'w': 
		global.Wkey=TRUE;
		break;
	case 'a': 
		global.Akey=TRUE;
		break;
	case 's':
		global.Skey=TRUE;
		break;
	case 'q':
		global.Qkey=TRUE;
		break;
	case ' ':
		global.spaceKey=TRUE;
		break;
	}
	update();
}

void keyUp(unsigned char key,int x, int y){
		switch(key){
	case 'd': 
		global.Dkey=FALSE;
		break;
	case 'w': 
		global.Wkey=FALSE;
		break;
	case 'a': 
		global.Akey=FALSE;
		break;
	case 's':
		global.Skey=FALSE;
		break;
	case 'q':
		global.Qkey=FALSE;
	case ' ':
		global.spaceKey=FALSE;
		break;
	}
}

void update(){
	if(global.Dkey)ship1->angle-=2;
	if(global.Akey)ship1->angle+=2;
	if(global.Wkey){ 
		ship1->xVec+=cosf(ship1->angle*M_PI/180);
		ship1->yVec+=sinf(ship1->angle*M_PI/180);
	}
	if(global.Skey){
		ship1->xVec-=cosf(ship1->angle*M_PI/180);
		ship1->yVec-=sinf(ship1->angle*M_PI/180);
	}
	if(global.Qkey){
		if(ship1->xVec>0)ship1->xVec--;
		if(ship1->xVec<0)ship1->xVec++;
		if(ship1->yVec>0)ship1->yVec--;
		if(ship1->yVec<0)ship1->yVec++;
	}	
	if(ship1->xPos>1)ship1->xPos=0;
	if(ship1->xPos<0)ship1->xPos=1;
	if(ship1->yPos>1)ship1->yPos=0;
	if(ship1->yPos<0)ship1->yPos=1;
	ship1->xPos+=ship1->xVec*global.difTime*0.01;
	ship1->yPos+=ship1->yVec*global.difTime*0.01;
	ship1->vertices[0]=ship1->xPos-0.01;
	ship1->vertices[1]=ship1->yPos-0.01;
	ship1->vertices[2]=ship1->xPos+0.01;
	ship1->vertices[3]=ship1->yPos-0.01;
	ship1->vertices[4]=ship1->xPos;
	ship1->vertices[5]=ship1->yPos+0.01;
}

/*Generates a random number below a max value*/
int randomMax(int max)
{
	int num,result, digit = max;
	srand(glutGet(GLUT_ELAPSED_TIME));
	max *= digit;
	do{ 
	num = rand(); 
	}while (num >= max);
	result=((num / digit)==0)? 1: num/digit;
	return result;
}

/*Draw specified text at screen location x,y. x,y<100*/
void drawText(unsigned char* text,int x,int y){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0,100,0,100,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(1, 1, 1);
	glRasterPos2i(x,y);
	glutBitmapString(GLUT_BITMAP_9_BY_15, text);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void drawGUI(){
	char buffer[15];
	sprintf(buffer,"Score: %d",global.score);
	drawText(buffer,0,5);
	sprintf(buffer,"Deaths: %d",global.deaths);
	drawText(buffer,0,2);
}

void drawCircle(float r, float xPos, float yPos){
	int i;
	float x,y;
	glPushMatrix();
	glTranslatef(xPos,yPos,0);
	glBegin(GL_LINE_LOOP);
	for(i=0;i<32;i++){
		x=r*cos((180/(float)16)*i*M_PI/180);
		y=r*sin((180/(float)16)*i*M_PI/180);
		glVertex2f(x,y);
	}
	glEnd();
	glPopMatrix();
}