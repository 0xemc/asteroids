#define SHIP_SCALE 0.2

typedef struct ship{
	float angle;
	float xPos;
	float yPos;
	float xVec;
	float yVec;
	float vertices[6];
}Ship;

typedef struct ship * ShipPtr;

ShipPtr ship1;

void drawShip(ShipPtr);