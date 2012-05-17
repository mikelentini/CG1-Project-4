#include "vecmath.h"
#include "Asteroid.h"

class Bullet {
public:
	Bullet(Vector3 position, Vector3 direction);
	
	Vector3 position;
	Vector3 direction;
	
	void move();
	bool overlaps(float x, float y, float z, bool isSmall);
};