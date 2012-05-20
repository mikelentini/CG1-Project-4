#include "vecmath.h"

class Sun {
public:
	Sun();
	
	Vector3 position;
	Vector3 direction;
	
	float rotate;
	
	void move();
	void switchDirection();
};