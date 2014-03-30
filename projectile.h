/****************************************************************************
* COSC1186/1187 - Interactive 3D Graphics and Animation
* Semester 1 2013 Assignment #2 
* Full Name        : Michael Collins
* Student Number   : 3370092
* Yallara Username : s3370092
* Course Code      : COSC1186
* Program Code     : BP096
***************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*Struct to store data for each projectile:
	vectors,
	current position,
	initial firing angle*/
typedef struct projectile{
	float vecX,vecY;
	float posX,posY;
	float accel,rotateAngle;
	float size;
	int projectileType;
}projectileStruct;

typedef struct projectile * projectilePtr;

projectilePtr createProjectile();
int initialiseProjectileArray();
void drawAllProjectiles();
void updateProjectiles();
void freeProjectiles();
void splitProjectile(projectilePtr);
int checkCollision(projectilePtr*,projectilePtr*);

projectilePtr * missileArray;
projectilePtr * rockArray;