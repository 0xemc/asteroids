#include "ship.h"
#include "main.h"

void drawShip(ShipPtr ship){
	glPushMatrix();
	glTranslatef(ship->xPos,ship->yPos,0);
	glRotatef(ship->angle-90,0,0,1);
	glScalef(SHIP_SCALE,SHIP_SCALE,SHIP_SCALE);
	glBegin(GL_TRIANGLES);
	glVertex2f(-0.1,-0.1);
	glVertex2f(0.1,-0.1);
	glVertex2f(0,0.1);
	glEnd();
	glScalef(1/SHIP_SCALE,1/SHIP_SCALE,1/SHIP_SCALE);
	glRotatef(-ship->angle+90,0,0,1);
	glTranslatef(-ship->xPos,-ship->yPos,0);
	glPopMatrix();
}


