#include "vecmath.h"

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

class Camera {
private:
	Vector3 upVector;
	Vector3 rightVector;

	float rotatedX, rotatedY, rotatedZ;	
	
public:
	Camera();
	
	Vector3 viewDirection;
	Vector3 position;
	
	void render();

	void move(Vector3 direction);

	void moveForward(float distance);
	void moveUp(float distance);
	void strafe(float distance);

	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);
};