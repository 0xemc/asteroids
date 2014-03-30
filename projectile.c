
#include "projectile.h"
#include "main.h"
#include "ship.h"

int numMissiles,numRocks;
int checkShipCollision(ShipPtr ship,projectilePtr p);

/*Initiate a new projectile struct and initiate values*/
projectilePtr createProjectile(){
	int i;
	projectilePtr p;
	if(global.pType==MISSILE&&numMissiles>MAX_PROJECTILES)return;
	if(global.pType==ROCK&&numRocks>MAX_ROCKS)return;
	p=(projectilePtr)calloc(1,sizeof(struct projectile));
	if(p==NULL){printf("ERROR: Memory allocation error"); return;}
		if(global.pType==MISSILE)
		{
		p->rotateAngle=ship1->angle*(M_PI/180); 
		p->posX=ship1->xPos;
		p->posY=ship1->yPos;
		p->projectileType=MISSILE;
		p->accel=0.5;
		p->vecX=cosf(p->rotateAngle);
		p->vecY=sinf(p->rotateAngle);
		numMissiles++;

		/*place in array in first free slot*/
		for(i=0;i<=numMissiles;i++){
			if(missileArray[i]==NULL){
				missileArray[i]=p;
				return p;
			}
		}

		}else{
		p->rotateAngle=randomMax(360);
		p->posX=0;
		p->posY=0;
		/*printf("posX:%lf, posY:%lf\n",p->posX,p->posY);*/
		p->projectileType=ROCK;
		p->size=3+(float)randomMax(10);
		p->accel= 1/((float)3+(float)randomMax(8));
		p->vecX=cosf(p->rotateAngle);
		p->vecY=sinf(p->rotateAngle);
		numRocks++;
		/*place in array in first free slot*/
			for(i=0;i<=numRocks;i++){
				if(rockArray[i]==NULL){
					rockArray[i]=p;
					return p;
				}
			}
		}
		return NULL;
}

/*Allocate memory to store MAX_PROJECTILE worth of projectile structs*/
int initialiseProjectileArray(){
	numMissiles=0;
	numRocks=0;
	missileArray=(projectilePtr*)calloc(MAX_PROJECTILES,sizeof(struct projectile));
	rockArray=(projectilePtr*)calloc(MAX_ROCKS,sizeof(struct projectile));
	if(missileArray==NULL||rockArray==NULL){
	printf("ERROR: Memory allocation error");
	return 1;
	}else{ return 0;}
	
}
/*Draws all projectiles stored in projectileArray*/
void drawAllProjectiles(){
	int i;
	projectilePtr p;
	for(i=0;i<MAX_PROJECTILES;i++)
	{
	p=missileArray[i];
		if(p!=NULL){
		glMatrixMode(GL_MODELVIEW);
		glBegin(GL_POINTS);
		glVertex2f(p->posX,p->posY);
		glEnd();
		}
	}
	for(i=0;i<MAX_ROCKS;i++)
	{
		p=rockArray[i];
		if(p!=NULL)
		{
		glTranslatef(p->posX,p->posY,0);
		glScalef(0.005,0.005,0.005);
		drawCircle(p->size,0,0);
		glScalef(1/0.005,1/0.005,1/0.005);
		glTranslatef(-p->posX,-p->posY,0);
		}
	}
}

/*Update positions and vectors of all projectiles in projectileArray,
 destroy projectile if collision occurs*/
void updateProjectiles(){
	int i;
	projectilePtr p;
	if(checkCollision(rockArray,missileArray)==MISSILE)global.score++;

	for(i=0;i<MAX_PROJECTILES;i++){
	p=missileArray[i];
		if(p!=NULL)
		{
			p->posX+=global.difTime*p->accel*p->vecX;
			p->posY+=global.difTime*p->accel*p->vecY;

			if(p->posX>1||p->posX<0||p->posY>1||p->posY<0)
			{
			missileArray[i]=NULL;
			numMissiles--;
			free(p);
			}
		}
	}
	for(i=0;i<MAX_ROCKS;i++){
	p=rockArray[i];
		if(p!=NULL){
			p->posX+=global.difTime*p->accel*p->vecX;
			p->posY+=global.difTime*p->accel*p->vecY;
				if(p->posX>1)p->posX=0;
				if(p->posX<0)p->posX=1;
				if(p->posY>1)p->posY=0;
				if(p->posY<0)p->posY=1;
		}
	}
}

void freeProjectiles(){
	int i;
	for(i=0;i<MAX_PROJECTILES;i++){
		if(missileArray[i]!=NULL)free(missileArray[i]);
	}
	free(missileArray);
	for(i=0;i<MAX_ROCKS;i++){
		if(rockArray[i]!=NULL)free(rockArray[i]);
	}
	free(rockArray);
}

int checkCollision(projectilePtr * rockArr,projectilePtr * missileArr){
	int i,j;
	projectilePtr missile,rock;
	for(j=0;j<MAX_PROJECTILES;j++)
	{
		missile=missileArr[j];
		if(missile==NULL)continue;
		for(i=0;i<MAX_ROCKS;i++)
		{
			rock=rockArr[i];
			if(rock==NULL)continue;
			if(rock->posX-missile->posX<(rock->size/200)
				&&rock->posY-missile->posY<(rock->size/200)
				&&rock->posX-missile->posX>-(rock->size/200)
				&&rock->posY-missile->posY>-(rock->size/200))
			{
				free(missile);
				missileArray[j]=NULL;
				numMissiles--;
				if(rock->size>3){splitProjectile(rock);
				free(rock);
				rockArray[i]=NULL;
				numRocks--;
				}
				else{
				free(rock);
				rockArray[i]=NULL;
				numRocks--;
				}
				return MISSILE;
			}
		}
	}
	for(i=0;i<MAX_ROCKS;i++)
	{
		rock=rockArr[i];
		if(rock==NULL)continue;
		if(checkShipCollision(ship1,rock)==TRUE&&global.Inv==FALSE)
		{
			/*printf("rockX:%lf, rockY:%lf, shipX:%lf, shipY:%lf, X-x=%lf, Y-y=%lf\n",rockArr[i]->posX,
																rockArr[i]->posY,
																ship1->xPos,
																ship1->yPos,
																ship1->xPos-rockArr[i]->posX,
																ship1->yPos-rockArr[i]->posY);*/
				global.Inv=TRUE;
				global.deaths++;
				ship1->xPos=0.5;
				ship1->yPos=0.5;
				ship1->xVec=0;
				ship1->yVec=0;
				ship1->angle=-90;
				return ROCK;
		}
	}
	return FALSE;
}

int checkShipCollision(ShipPtr ship,projectilePtr p){
	int i;
	for(i=0;i<6;i+=2){
		if(ship->vertices[i]-p->posX<p->size/200
			&&ship->vertices[i+1]-p->posY<p->size/200
			&&ship->vertices[i]-p->posX>-p->size/200
			&&ship->vertices[i+1]-p->posY>-p->size/200){
			/*printf("vertexX:%lf vertexY:%lf rockSize/2:%lf\n",ship->vertices[i],ship->vertices[i+1],p->size/200);*/
			return TRUE;
		}
	}
	return FALSE;
}

void splitProjectile(projectilePtr r){
	int i;
	projectilePtr p1,p2;
	if(global.pType==MISSILE&&numMissiles>MAX_PROJECTILES)return;
	if(global.pType==ROCK&&numRocks>MAX_ROCKS)return;
	p1=(projectilePtr)calloc(1,sizeof(struct projectile));
	p2=(projectilePtr)calloc(1,sizeof(struct projectile));
	if(p1==NULL||p2==NULL){printf("ERROR: Memory allocation error"); return;}
		p1->rotateAngle=r->rotateAngle+270;
		p2->rotateAngle=r->rotateAngle-270;
		p1->posX=r->posX;
		p2->posX=r->posX;
		p1->posY=r->posY;
		p2->posY=r->posY;
		/*printf("posX:%lf, posY:%lf\n",p->posX,p->posY);*/
		p1->projectileType=ROCK;
		p2->projectileType=ROCK;
		p1->size=r->size/(float)2;
		p2->size=r->size/(float)2;
		p1->accel= r->accel;
		p2->accel= r->accel;
		p1->vecX=cosf(p1->rotateAngle);
		p1->vecY=sinf(p1->rotateAngle);
		p2->vecX=cosf(p2->rotateAngle);
		p2->vecY=sinf(p2->rotateAngle);
		numRocks+=2;
		/*place in array in first free slot*/
			for(i=0;i<=numRocks;i++){
				if(rockArray[i]==NULL){
					rockArray[i]=p1;
					break;
				}
			}
			/*place in array in first free slot*/
			for(i=0;i<=numRocks;i++){
				if(rockArray[i]==NULL){
					rockArray[i]=p2;
					break;
				}
			}
}