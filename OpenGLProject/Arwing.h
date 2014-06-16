#ifndef ARWING_H
#define ARWING_H

#include <iostream>
#include <vector>
#include "windows.h"
#include "mmsystem.h"
#include "Projectile.h"
using namespace std;

const float ROTATE_FACTOR = 0.14f;
const float MOVE_FACTOR = 0.02f;
const float PITCH_FACTOR = 0.2f;
const float MAX_PITCH = 15.0f;
const float MAX_ROTATE = 25.0f;

class Arwing {
public:
	Arwing();
	Arwing(float speed);
	float getSpeed();
	void setSpeed(float speed);
	void booster(bool boost);
	void fire(void);
private:
	float speed;
};

#endif