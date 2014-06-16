#include <GL/freeglut.h>
#include <GL/GL.h>
#include "GameObjects.h"

FieldMover::FieldMover(float x) {
	this->x = x;
	this->originX = -60;
}

void FieldMover::setX(float x) {
	this->x = x;
}
float FieldMover::getX() {
	return this->x;
}

void FieldMover::drawFour() {	
	glPushMatrix();
	if (getX() > 10)
		setX(this->originX);
	glTranslatef(this->x, -4.9f, 25);
	for (char i = 0; i < 4; i++) {	
		glTranslatef(0, 0, -10);
		drawCube(0.1f, 1.0f, 1.0f, 1.0f);	
	}
	glTranslatef(0, 0, 0);
	glPopMatrix();
}

void FieldMover::drawCube(float size, float red, float green, float blue) {
	
	glBegin(GL_QUADS);
	glColor3f(red, green, blue);
	glVertex3f(-1 * size, -1 * size, -1 * size);
	glVertex3f(1 * size, -1 * size, -1 * size);
	glVertex3f(1 * size, -1 * size, 1 * size);
	glVertex3f(-1 * size, -1 * size, 1 * size);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(red, green, blue);
	glVertex3f(-1 * size, 1 * size, -1 * size);
	glVertex3f(1 * size, 1 * size, -1 * size);
	glVertex3f(1 * size, 1 * size, 1 * size);
	glVertex3f(-1 * size, 1 * size, 1 * size);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(red, green, blue);
	glVertex3f(-1 * size, 1 * size, -1 * size);
	glVertex3f(-1 * size, -1 * size, -1 * size);
	glVertex3f(-1 * size, -1 * size, 1 * size);
	glVertex3f(-1 * size, 1 * size, 1 * size);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(red, green, blue);
	glVertex3f(1 * size, 1 * size, -1 * size);
	glVertex3f(1 * size, -1 * size, -1 * size);
	glVertex3f(1 * size, -1 * size, 1 * size);
	glVertex3f(1 * size, 1 * size, 1 * size);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(red, green, blue);
	glVertex3f(-1 * size, -1 * size, -1 * size);
	glVertex3f(1 * size, -1 * size, -1 * size);
	glVertex3f(1 * size, 1 * size, -1 * size);
	glVertex3f(-1 * size, 1 * size, -1 * size);
	glEnd();

	glBegin(GL_QUADS);
	glColor3f(red, green, blue);
	glVertex3f(-1 * size, -1 * size, 1 * size);
	glVertex3f(1 * size, -1 * size, 1 * size);
	glVertex3f(1 * size, 1 * size, 1 * size);
	glVertex3f(-1 * size, 1 * size, 1 * size);
	glEnd();	

	
}