#include "projectile.h"

Projectile::Projectile(float x, float y, float z) {
	this->x = x; this->y = y; this->z = z;
	this->alive = true;
}
void Projectile::changeXYZ(float x, float y, float z) {
	this->x = x; this->y = y; this->z = z;
}
float Projectile::getX() {
	return this->x;
}
float Projectile::getY() {
	return this->y;
}
float Projectile::getZ() {
	return this->z;
}
bool Projectile::isAlive() {
	return alive;
}
void Projectile::kill() {
	this->alive = false;
}