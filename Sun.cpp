#include "Sun.h"

Sun::Sun() {
	this->position = Vector3(0, 0, 0);
	this->direction = Vector3(1, 0, 0);
	this->rotate = 0;
}

void Sun::move() {
	if (this->direction.x == 1) {
		this->position.x += 0.02f;
	} else if (this->direction.x == -1) {
		this->position.x -= 0.02f;
	}
	
	this->rotate += 0.25f;
}

void Sun::switchDirection() {
	this->direction.x *= -1;
}