#ifndef PROJECTILE_H
#define PROJECTILE_H

class Projectile {
public:	
	Projectile(float x, float y, float z);
	void changeXYZ(float x, float y, float z);
	float getX();
	float getY();
	float getZ();
	bool isAlive();
	void kill();
private:
	float x, y, z;
	bool alive;
};

#endif