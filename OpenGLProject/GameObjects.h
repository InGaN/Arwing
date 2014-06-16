#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H


class FieldMover {
public:
	FieldMover(float x);
	void drawFour();
	void setX(float x);
	float getX();
private:
	void drawCube(float size, float red, float green, float blue);
	float x;
	float originX;
};

#endif