#include "Arwing.h"

Arwing::Arwing() {
	this->speed = 0.2f;
}

Arwing::Arwing(float speed) {
	this->speed = speed;
}

float Arwing::getSpeed() {
	return this->speed;
}
void Arwing::setSpeed(float speed) {
	this->speed = speed;
}
void Arwing::booster(bool boost) {
	if (boost) {
		if (getSpeed() < 0.5f) {
			PlaySound(TEXT("sfx\\boost.WAV"), NULL, SND_ASYNC);
			setSpeed(getSpeed() + 0.05f);
			cout << "Speed: " << getSpeed() << endl;
		}
	}
	else {
		if (getSpeed() > 0.1f) {
			PlaySound(TEXT("sfx\\slowdown.WAV"), NULL, SND_ASYNC);
			setSpeed(getSpeed() - 0.05f);
			cout << "Speed: " << getSpeed() << endl;
		}
	}
}
void Arwing::fire(void) {
	PlaySound(TEXT("sfx\\fire.WAV"), NULL, SND_ASYNC);	
	cout << "fire!" << endl;
}