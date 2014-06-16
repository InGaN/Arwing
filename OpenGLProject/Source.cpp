#include <GL/freeglut.h>
#include <GL/GL.h>
#include <iostream>
#include "windows.h"
#include "mmsystem.h"

#include "Texture.h"
#include "ObjModel.h"
#include "Arwing.h";
#include "Projectile.h"
#include "GameObjects.h"

using namespace std;

float angle, eyeAngle = 0;
float moveX, moveY, moveZ = 0;
int eyeX, eyeY = 0;
float mouseX, mouseY;
float pitch = 0;
float cameraX, cameraZ;
float cameraOriginY, cameraOriginZ;
const float ORIGIN_MOVE_FACTOR = 0.008f;
int windowWidth = 1000;
int windowHeight = 800;
GLuint textureId = 0;
Arwing arwing = Arwing(0.1f);
int currentModel = 0;
float rotationX, rotationY, rotationZ = 0;
vector<pair<int, ObjModel*> > models;
vector<Projectile*> projectiles;
vector<FieldMover> fieldMovers;
bool keyStates[256] = {false};

void drawAxis(void);
void drawPlane(void);
void drawLaser(void);
void moveShip(void);
void checkProjectiles(void);
void listenKeys(void);

void InitGraphics(void)
{
	// Lighting
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHT0);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//models.push_back(pair<int, ObjModel*>(75, new ObjModel("models/vampire/3D_vampire.obj")));	
	models.push_back(pair<int, ObjModel*>(75, new ObjModel("models/starwing/starwing.obj")));
	models.push_back(pair<int, ObjModel*>(75, new ObjModel("models/starwing/laserbeam.obj")));
}

// OPENGL stuff, needs to be seperated in its own class

void drawAxis(void) {

	glPushMatrix();
	// X-axis
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glPointSize(3.0);
	glVertex3f(0, 0, 0);
	glVertex3f(10, 0, 0);
	glEnd();

	// Y-axis
	glBegin(GL_LINES);
	glColor3f(0.0f, 1.0f, 0.0f);
	glPointSize(3.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 10, 0);
	glEnd();

	// Z-axis
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 1.0f);
	glLineWidth(5.0);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 10);
	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f); // Stops color blending, but it does not seem best practice this way
	glPopMatrix();
}

void Display(void)
{
	glEnable(GL_DEPTH_TEST); // prevents texture overlay
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	gluPerspective(90, (windowWidth / windowHeight), 1, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	cameraX = 10 * cos(eyeAngle);
	cameraZ = 10 * sin(eyeAngle);

	gluLookAt(cameraX, mouseY, mouseX,	// eye
		0, cameraOriginY, cameraOriginZ,	// center
		0, 1, 0);	// up

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_BLEND);

	drawAxis();	
	
	drawPlane();
	for (int i = 0; i < fieldMovers.size(); i++)
		fieldMovers[i].drawFour();

	glTranslatef(moveX, moveY, moveZ);
	glTranslatef(0, 0, 0); // move back to origin so ship rotates in the origin
	glRotatef(rotationX, 1, 0, 0);
	glRotatef(pitch, 0, 0, 1);
	if (models.size() > 0)
		models[currentModel].second->draw();


	// draw projectiles
	for (char i = 0; i < projectiles.size(); i++) {
		glTranslatef(projectiles[i]->getX(), projectiles[i]->getY(), projectiles[i]->getZ());		
		models[1].second->draw();
		glTranslatef(0, 0, 0);
	}
	
	glutSwapBuffers();
}

void Reshape(GLint width, GLint height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(70, width / (float)height, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
}


void MouseButton(int button, int state, int x, int y)
{
}

void MouseMotion(int x, int y)
{
	if ((x > 0) && (y > 0) && (x < windowWidth) && (y < windowHeight)) {
		if (x > windowWidth / 2) {
			mouseX = 0 - ((x - (windowWidth / 2)) / 100.0f);
		}
		else {
			mouseX = 0 + (((windowWidth / 2) / 100.0f) - (x / 100.0f));
		} 
		if (y > windowHeight / 2) {
			mouseY = 0 - ((y - (windowHeight / 2)) / 100.0f);
		}	
		else {
			mouseY = 0 + (((windowHeight / 2) / 100.0f) - (y / 100.0f));
		}
	}
}

void IdleFunc(void)
{
	angle += 0.1;
	glutPostRedisplay();
	listenKeys();
	for (int i = 0; i< fieldMovers.size(); i++) {
		fieldMovers[i].setX(fieldMovers[i].getX() + arwing.getSpeed());
	}	

	//moveShip();
	// stabalize the ship
	if (pitch > 0)
		pitch -= PITCH_FACTOR / 10;
	else if (pitch < 0)
		pitch += PITCH_FACTOR / 10;
	if (rotationX > 0)
		rotationX -= ROTATE_FACTOR / 3;
	else if (rotationX < 0)
		rotationX += ROTATE_FACTOR / 3;
	
	checkProjectiles();
}

void keyPressed(unsigned char key, int x, int y) {
	keyStates[key] = true;
}

void keyUp(unsigned char key, int x, int y) {
	keyStates[key] = false;
}

int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutCreateWindow("Arwing FlightSim");
	// Initialize OpenGL graphics state
	InitGraphics();
	// Register callbacks:
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);

	//glutKeyboardFunc(Keyboard);
	glutKeyboardFunc(keyPressed);
	glutKeyboardUpFunc(keyUp);

	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutIdleFunc(IdleFunc);
	// Turn the flow of control over to GLUT

	for (int i = 0; i < 7; i++) {
		fieldMovers.push_back(FieldMover(-10.0f*i));
	}

	// Mist
	//glEnable(GL_FOG);
	float FogCol[3] = { 0.9f, 0.0f, 0.0f };
	glFogfv(GL_FOG_COLOR, FogCol);
	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogf(GL_FOG_START, 10.0f);
	glFogf(GL_FOG_END, 60.0f);

	glutMainLoop();

	return 0;
}

void checkProjectiles(void) {	
	for (char i = 0; i < projectiles.size(); i++) {
		if (!projectiles[i]->isAlive()) {
			delete (projectiles[i]);
			projectiles.erase(projectiles.begin() + i); 			
		}
		else {
			projectiles[i]->changeXYZ(projectiles[i]->getX() - 0.04f, 0, 0);
			if (projectiles[i]->getX() < -30)
				projectiles[i]->kill();
		}
		
	}	
}

void moveShip(void) {
	if (pitch > -MAX_PITCH)
		pitch -= 1.0f;
}

void drawPlane(void) {
	glPushMatrix();
	for (int i = 0; i < 7; i++) {
		glBegin(GL_QUADS);
		glColor3f(0.1f + (0.1f * i), 1.0f, 0.2f + (0.1f * i));
		glPointSize(3.0);
		glVertex3f(0 - (i * 10), -5, 20);
		glVertex3f(10 - (i * 10), -5, 20);
		glVertex3f(10 - (i * 10), -5, -20);
		glVertex3f(0 - (i * 10), -5, -20);
		glEnd();
	}
	glColor3f(1.0f, 1.0f, 1.0f); // Stops color blending, but it does not seem best practice this way
	glPopMatrix();
}

void listenKeys(void) {
	if (keyStates['s']) {
		if (moveY < 5) {
			moveY += MOVE_FACTOR;
			cameraOriginY += ORIGIN_MOVE_FACTOR;
			if (pitch > -MAX_PITCH)
				pitch -= PITCH_FACTOR;
		}
	}
	if (keyStates['w']) {
		if (moveY > -4) {
			moveY -= MOVE_FACTOR;
			cameraOriginY -= ORIGIN_MOVE_FACTOR;
			if (pitch < MAX_PITCH)
				pitch += PITCH_FACTOR;
		}
	}
	if (keyStates['a']) {
		if (rotationX < MAX_ROTATE)
			rotationX += ROTATE_FACTOR;
		if (moveZ < 10) {
			moveZ += MOVE_FACTOR;
			cameraOriginZ += ORIGIN_MOVE_FACTOR;
		}
	}
	if (keyStates['d']) {
		if (rotationX > -MAX_ROTATE)
			rotationX -= ROTATE_FACTOR;
		if (moveZ > -10) {
			moveZ -= MOVE_FACTOR;
			cameraOriginZ -= ORIGIN_MOVE_FACTOR;
		}
	}
	if (keyStates['z']) {
		keyStates['z'] = false; // prevent keyboardSpam
		arwing.booster(true);
	}
	if (keyStates['x']) {
		keyStates['x'] = false;
		arwing.booster(false);
	}
	if (keyStates[32]) { //spacebar
		keyStates[32] = false;
		arwing.fire();		
		projectiles.push_back(new Projectile(0.0f, 0.0f, 0.0f));
	}
}



void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:             // ESCAPE key
		exit(0);
		break;
	case '[':
		currentModel = (currentModel + models.size() - 1) % models.size();
		break;
	case ']':
		currentModel = (currentModel + 1) % models.size();
		break;
	case 's':
		if (moveY < 5) {
			moveY += MOVE_FACTOR;
			cameraOriginY += ORIGIN_MOVE_FACTOR;
			if (pitch > -MAX_PITCH)
				pitch -= 1.0f;
		}
		break;
	case 'w':
		if (moveY > -4) {
			moveY -= MOVE_FACTOR;
			cameraOriginY -= ORIGIN_MOVE_FACTOR;
			if (pitch < MAX_PITCH)
				pitch += 1.0f;
		}
		break;
	case 'a':
		if (rotationX < MAX_ROTATE)
			rotationX += ROTATE_FACTOR;
		if (moveZ < 10) {
			moveZ += MOVE_FACTOR;
			cameraOriginZ += ORIGIN_MOVE_FACTOR;
		}
		break;
	case 'd':
		if (rotationX > -MAX_ROTATE)
			rotationX -= ROTATE_FACTOR;
		if (moveZ > -10) {
			moveZ -= MOVE_FACTOR;
			cameraOriginZ -= ORIGIN_MOVE_FACTOR;
		}
		break;
	case 'z':
		if (arwing.getSpeed() < 0.5f) {
			PlaySound(TEXT("sfx\\boost.WAV"), NULL, SND_ASYNC);
			arwing.setSpeed(arwing.getSpeed() + 0.1f);
		}
		break;
	case 'x':
		if (arwing.getSpeed() > 0.2f) {
			PlaySound(TEXT("sfx\\slowdown.WAV"), NULL, SND_ASYNC);
			arwing.setSpeed(arwing.getSpeed() - 0.1f);
		}
		break;
	case 32: //spacebar
		PlaySound(TEXT("sfx\\fire.WAV"), NULL, SND_ASYNC);
		projectiles.push_back(new Projectile(0.0f, 0.0f, 0.0f));
		cout << "fire!" << endl;
		break;
	}
}

/*
void printtext(int x, int y, string String)
{
//(x,y) is from the bottom left of the window
glMatrixMode(GL_PROJECTION);
glPushMatrix();
glLoadIdentity();
glOrtho(0, 1024, 0, 768, -1.0f, 1.0f);
glMatrixMode(GL_MODELVIEW);
glPushMatrix();
glLoadIdentity();
glPushAttrib(GL_DEPTH_TEST);
glDisable(GL_DEPTH_TEST);
glRasterPos2i(x, y);
for (int i = 0; i<String.size(); i++)
{
glutBitmapCharacter(GLUT_BITMAP_9_BY_15, String[i]);
}
glPopAttrib();
glMatrixMode(GL_PROJECTION);
glPopMatrix();
glMatrixMode(GL_MODELVIEW);
glPopMatrix();
}
[9:35:54 AM] Raoul:   char string[64];


sprintf(string, "Press 'w' and 's' to go up and down");
printtext(10, 720, string);
*/