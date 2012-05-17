#include "Bullet.h"

Bullet::Bullet(Vector3 position, Vector3 direction) {
	this->position = position;
	this->direction = direction;
}

void Bullet::move() {
	this->position += (this->direction * 0.1f);
}

bool Bullet::overlaps(float x, float y, float z, bool isSmall) {
	float asteroidWidth;
	
	if (isSmall) {
		asteroidWidth = 0.25f / 2.0f;
	} else {
		asteroidWidth = 0.25f;
	}
	
	return this->position.x < x + asteroidWidth &&
			this->position.x > x - asteroidWidth &&
			this->position.y > y - asteroidWidth && 
			this->position.y < y + asteroidWidth &&
			this->position.z < z + asteroidWidth &&
			this->position.z > z - asteroidWidth;
}