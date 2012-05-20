/*
* project4.cpp
* CG1 Project 4
*
* Author: Mike Lentini <mjl1445@rit.edu>
* Created: May 7, 2012
* Modified for Project 4: May 17, 2012
*/

#if defined(__APPLE__) && defined(__MACH__)
#include <GLUT/glut.h>
#include <SOIL.h>
#else
#include <SOIL/SOIL.h>
#include <GL/glut.h>
#endif

#include "Camera.h"
#include "Bullet.h"
#include "Sun.h"
#include <list>

Camera camera;
Sun sun;
bool fillShapes = true;
bool lightOn = true;
bool fogOn = false;
list<Asteroid> asteroids;
list<Bullet> bullets;

float ambLight[] = { 0.01f, 0.01f, 0.01f, 0.01f };
float sunLight[] = { 1, 0.5f, 0, 1 };

float fogColor[] = { 0.70f, 0.70f, 0.70f, 1 };

GLuint asteroidTexture;
GLuint smallAsteroidTexture;
GLuint spaceTexture;
GLuint projectileTexture;
GLuint sunTexture;

float lastX = 0;
float lastY = 0;

const int ESC = 27, Q = 113, ONE = 49, TWO = 50, SPACE = 32, Z = 122, 
	W = 119, A = 97, S = 115, D = 100, R = 114, F = 102, N = 110;

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
		case ESC:
			exit(0);
			break;
		case Q:
			exit(0);
			break;
		case ONE:
			lightOn = !lightOn;
			
			if (lightOn) {
				glEnable(GL_LIGHTING);
				glLightfv(GL_LIGHT0, GL_AMBIENT, ambLight);
				glEnable(GL_LIGHT0);
				
				glLightfv(GL_LIGHT1, GL_DIFFUSE, sunLight);
				glLightfv(GL_LIGHT1, GL_AMBIENT, sunLight);
				glEnable(GL_LIGHT1);
			} else {
				glDisable(GL_LIGHTING);
				glDisable(GL_LIGHT0);
				glDisable(GL_LIGHT1);
				glDisable(GL_COLOR_MATERIAL);
			}
			
			break;
		case TWO:
			fillShapes = !fillShapes;
			break;
		case SPACE:
			camera.moveUp(0.2);
			break;
		case Z:
			camera.moveUp(-0.2);
			break;
		case W:
			camera.moveForward(-0.2);
			break;
		case A:
			camera.strafe(-0.2);
			break;
		case S:
			camera.moveForward(0.2);
			break;
		case D:
			camera.strafe(0.2);
			break;
		case R:
			camera = Camera();
			fillShapes = true;
			lightOn = true;

			asteroids.clear();

			for (int i = 0; i < 99; i++) {
				asteroids.push_back(Asteroid(false));
			}

			bullets.clear();
			break;
		case F:
			fogOn = !fogOn;
			
			if (fogOn) {
				glEnable(GL_FOG);
			} else {
				glDisable(GL_FOG);
			}
			
			break;
		case N:
			
			break;
	}
}

/*
 * Called when the mouse button is pressed.
 */
void mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		bullets.push_back(Bullet(camera.position,
								 Vector3(camera.viewDirection.x,
										 camera.viewDirection.y,
										 camera.viewDirection.z)
								));
	}
}

/*
 * Called when mouse moves without button pressed.
 */
void passiveMotion(int x, int y) {
	float rotateY = ((lastX - x) / glutGet(GLUT_WINDOW_WIDTH)) * 1000;
	camera.rotateY(rotateY);
	
	float rotateX = ((lastY - y) / glutGet(GLUT_WINDOW_HEIGHT)) * 1000;
	camera.rotateX(rotateX);
	
	lastX = x;
	lastY = y;
}

void drawScene() {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	
	if (lightOn) {
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
		glEnable(GL_COLOR_MATERIAL);
	}
	
	if (fillShapes) {
		if (!lightOn) {
			glColor4ub(50, 50, 50, 255);
			glutSolidSphere(10, 30, 30);
		} else {
			glColor3f(1, 1, 1);
			
			glEnable(GL_TEXTURE_2D);
			glEnable(GL_TEXTURE_GEN_S);
			glEnable(GL_TEXTURE_GEN_T);
			
			glBindTexture(GL_TEXTURE_2D, spaceTexture);
			
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			
			glutSolidSphere(10, 30, 30);
			
			glDisable(GL_TEXTURE_GEN_S);
			glDisable(GL_TEXTURE_GEN_T);
			glDisable(GL_TEXTURE_2D);
		}
	} else {
		glColor4ub(50, 50, 50, 255);
		glutWireSphere(10, 30, 30);
	}
	
	if (lightOn) {
		glDisable(GL_COLOR_MATERIAL);
	}
	
	float spec[] = { 1, 1, 1, 1 };
	glMaterialfv(GL_FRONT, GL_DIFFUSE, spec);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glColor3f(1, 1, 1);
	
	for (list<Asteroid>::iterator iter(asteroids.begin()); iter != asteroids.end(); iter++) {
		GLUquadric *asteroid = gluNewQuadric();

		glPushMatrix();
		glTranslatef(iter->position.x, iter->position.y, iter->position.z);
		glRotatef(iter->rotate, iter->position.x, iter->position.y, iter->position.z);

		if (iter->isSmall) {
			glScalef(0.5f, 0.5f, 0.5f);
		}
		
		if (!fillShapes) {
			gluQuadricDrawStyle(asteroid, GLU_LINE);
		} else {
			if (!lightOn) {
				gluQuadricDrawStyle(asteroid, GLU_FILL);
			} else {
				gluQuadricNormals(asteroid, GLU_SMOOTH);
				gluQuadricTexture(asteroid, GL_TRUE);
				
				glEnable(GL_TEXTURE_2D);
				
				if (iter->isSmall) {
					glBindTexture(GL_TEXTURE_2D, smallAsteroidTexture);
				} else {
					glBindTexture(GL_TEXTURE_2D, asteroidTexture);
				}
			}
		}

		gluSphere(asteroid, 0.25f, 10, 10);
		
		glDisable(GL_TEXTURE_2D);
		
		glPopMatrix();

		gluDeleteQuadric(asteroid);
	}
	
	glColor3f(1, 0, 0);
	
	for (list<Bullet>::iterator iter(bullets.begin()); iter != bullets.end(); ) {
		bool hitAsteroid = false;
		
		GLUquadric *bullet = gluNewQuadric();
		
		if (!fillShapes) {
			gluQuadricDrawStyle(bullet, GLU_LINE);
		} else {
			if (!lightOn) {
				gluQuadricDrawStyle(bullet, GLU_FILL);
			} else {
				gluQuadricNormals(bullet, GLU_SMOOTH);
				gluQuadricTexture(bullet, GL_TRUE);
				
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, projectileTexture);
			}
		}
		
		glPushMatrix();
			glTranslatef(iter->position.x, iter->position.y, iter->position.z);
			
			gluSphere(bullet, 0.05f, 10, 10);
		glPopMatrix();
		
		gluDeleteQuadric(bullet);
		
		for (list<Asteroid>::iterator asteroid(asteroids.begin()); asteroid != asteroids.end(); ) {
			if (iter->overlaps(asteroid->position.x, asteroid->position.y, asteroid->position.z, asteroid->isSmall)) {
				if (!asteroid->isSmall) {
					Asteroid new1(true);
					Asteroid new2(true);

					new1.position = asteroid->position;
					new2.position = new1.position;
					
					new2.direction = new1.direction * -1;

					asteroids.push_back(new1);
					asteroids.push_back(new2);
				}
				
				asteroid = asteroids.erase(asteroid);
				
				hitAsteroid = true;
			} else {
				asteroid++;
			}
		}
		
		if (hitAsteroid) {
			iter = bullets.erase(iter);
		} else {
			iter++;
		}
	}
	
	glColor3f(1, 1, 0);
	
	GLUquadric *sunQuadric = gluNewQuadric();
	
	if (!fillShapes) {
		gluQuadricDrawStyle(sunQuadric, GLU_LINE);
	} else {
		if (!lightOn) {
			gluQuadricDrawStyle(sunQuadric, GLU_FILL);
		} else {
			gluQuadricNormals(sunQuadric, GLU_SMOOTH);
			gluQuadricTexture(sunQuadric, GL_TRUE);
			
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, sunTexture);
		}
	}
	
	float sunPos[] = { sun.position.x, sun.position.y, sun.position.z, 1 };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, sunLight);
	glLightfv(GL_LIGHT1, GL_POSITION, sunPos);
	
	glPushMatrix();
		glTranslatef(sun.position.x, sun.position.y, sun.position.z);
		glRotatef(sun.rotate, sun.position.x, sun.position.y, sun.position.z);
		
		gluSphere(sunQuadric, 0.5f, 20, 20);
	glPopMatrix();
	
	gluDeleteQuadric(sunQuadric);
	
	glDisable(GL_TEXTURE_2D);
	
	if (fogOn) {
		glFogi(GL_FOG_MODE, GL_LINEAR);
		glFogfv(GL_FOG_COLOR, fogColor);
		glFogf(GL_FOG_START, 0.5f);
		glFogf(GL_FOG_END, 20);
	}
	
	glutPostRedisplay();
}

void setMainViewport(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	gluPerspective(60, w / h, 0.5f, 40);
	glMatrixMode(GL_MODELVIEW);
}

void setSecondaryViewport(int width, int height) {
	glScissor(width - 210, height - 210, 200, 200);
	
	glEnable(GL_SCISSOR_TEST);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glViewport(width - 210, height - 210, 200, 200);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	glOrtho(-10.1f, 10.1f, -10.1f, 10.1f, 0.5f, 25);
	glMatrixMode(GL_MODELVIEW);
	
	glDisable(GL_SCISSOR_TEST);
}

void display() {
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	setMainViewport(width, height);
	glPushMatrix();
		camera.render();
		drawScene();
	glPopMatrix();
	
	setSecondaryViewport(width, height);
	drawScene();
	
	glutSwapBuffers();
}

void initGL() {
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambLight);
	glEnable(GL_LIGHT0);
	
	glLightfv(GL_LIGHT1, GL_DIFFUSE, sunLight);
	glLightfv(GL_LIGHT1, GL_AMBIENT, sunLight);
	glEnable(GL_LIGHT1);
	
	asteroidTexture = SOIL_load_OGL_texture("asteroid.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB);
	smallAsteroidTexture = SOIL_load_OGL_texture("small-asteroid.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB);
	spaceTexture = SOIL_load_OGL_texture("space.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB);
	projectileTexture = SOIL_load_OGL_texture("projectile.jpg", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB);
	sunTexture = SOIL_load_OGL_texture("sun.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB);
}

void reshape(int w, int h) {
	
}

void idle() {
	
}

void animateTimer(int value) {
	for (list<Asteroid>::iterator iter(asteroids.begin()); iter != asteroids.end(); ) {
		if (iter->position.x > 10 || iter->position.x < -10 || 
			iter->position.y > 10 || iter->position.y < -10 || 
			iter->position.z > 10 || iter->position.z < -10) {
			iter = asteroids.erase(iter);
			
			asteroids.push_back(Asteroid(false));
		} else {
			iter->rotate += 1;
			
			if (iter->direction.x > 0) {
				iter->position.x += 0.01f;
			} else {
				iter->position.x -= 0.01f;
			}
			
			if (iter->direction.y > 0) {
				iter->position.y += 0.01f;
			} else {
				iter->position.y -= 0.01f;
			}
			
			if (iter->direction.z > 0) {
				iter->position.z += 0.01f;
			} else {
				iter->position.z -= 0.01f;
			}
			
			iter++;
		}
	}
	
	for (list<Bullet>::iterator iter(bullets.begin()); iter != bullets.end(); ) {
		if (iter->position.x > 10 || iter->position.x < -10 || 
			iter->position.y > 10 || iter->position.y < -10 || 
			iter->position.z > 10 || iter->position.z < -10) {
			iter = bullets.erase(iter);
		} else {
			iter->move();
			
			iter++;
		}
	}
	
	sun.move();
	
	if (sun.position.x > 10 || sun.position.x < -10) {
		sun.switchDirection();
	}
	
	glutTimerFunc(10, animateTimer, 0);
}

int main( int argc, char** argv ) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));
	glutReshapeFunc(reshape);
	
	glutCreateWindow("CG1 Project 3 (Mike Lentini)");
	glutFullScreen();
	
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(passiveMotion);
	glutMotionFunc(passiveMotion);
	glutSetCursor(GLUT_CURSOR_NONE);
	glutWarpPointer(glutGet(GLUT_WINDOW_WIDTH) / 2, glutGet(GLUT_WINDOW_HEIGHT) / 2);
	
	glutTimerFunc(10, animateTimer, 0);
	
	initGL();
	
	for (int i = 0; i < 99; i++) {
		asteroids.push_back(Asteroid(false));
	}

	glutMainLoop();

	return 0;
}
