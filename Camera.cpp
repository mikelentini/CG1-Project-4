#include "Camera.h"

/*Vector3 vector(float x, float y, float z) {
	Vector3 tmp(x, y, z);
	return tmp;
}*/

Camera::Camera() {
	position = Vector3(0, 0, 0);
	viewDirection = Vector3(0, 0, -1);
	rightVector = Vector3(1, 0, 0);
	upVector = Vector3(0, 1, 0);
	
	rotatedX = 0;
	rotatedY = 0;
	rotatedZ = 0;
}

void Camera::move(Vector3 direction) {
	position = position + direction;
}

void Camera::rotateX(float angle) {
	rotatedX += angle;
	
	viewDirection = Vector3(viewDirection * cos(angle * M_PI / 180)	+ upVector * sin(angle * M_PI / 180));
	viewDirection.normalize();
	
	upVector = viewDirection^rightVector * -1;
}

void Camera::rotateY(float angle) {
	rotatedY += angle;
	
	viewDirection = viewDirection * cos(angle * M_PI / 180)	- rightVector * sin(angle * M_PI / 180);
	viewDirection.normalize();
	
	rightVector = viewDirection^upVector;
}

void Camera::rotateZ(float angle) {
	rotatedZ += angle;
	
	rightVector = rightVector * cos(angle * M_PI / 180) + upVector * sin(angle * M_PI / 180);
	rightVector.normalize();
	
	upVector = viewDirection^rightVector * -1;
}

void Camera::render() {
	Vector3 viewPoint = position + viewDirection;
	
	gluLookAt(position.x, position.y, position.z,
				viewPoint.x, viewPoint.y, viewPoint.z,
				upVector.x, upVector.y, upVector.z);

}

void Camera::moveForward(float distance) {
	position = position + (viewDirection * -distance);
}

void Camera::strafe(float distance) {
	position = position + (rightVector * distance);
}

void Camera::moveUp(float distance) {
	position = position + (upVector * distance);
}