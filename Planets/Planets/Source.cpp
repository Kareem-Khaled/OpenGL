#include<random>
#include<cmath>
#include <GL\glut.h>
static GLfloat xRot = 0.0f, yRot = 0.0f, rot[5];
static GLfloat sp[] = { 1, 1.5, 2, 2.3, 2.8 }, strx[200], stry[200];
GLfloat pi = acos(-1);
void SetupRc(){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Screen Color (Black)
	for (int i = 0; i < 200; i++){
		float x = -200 + (float)(rand()) / (RAND_MAX / (400));
		float y = -200 + (float)(rand()) / (RAND_MAX / (400));
		strx[i] = x; stry[i] = y;
	}
}

// to resize window and not to damge shapes
void ChangeSize(GLint w, GLint h) {
	if (h == 0)	h = 1;
	glViewport(0, 0, w, h); glMatrixMode(GL_PROJECTION); glLoadIdentity();
	GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;
	if (w <= h)
		glOrtho(-200.0, 200.0, -200.0 / aspectRatio, 200.0 / aspectRatio, -200.0, 200.0);
	else
		glOrtho(-200.0 * aspectRatio, 200.0 * aspectRatio, -200.0, 200.0, -200.0, 200.0);
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
}

// to rotate the screen
void SpecialKeys(int key, int x, int y) {
	if (key == GLUT_KEY_UP) xRot -= 5.0f;
	if (key == GLUT_KEY_DOWN)  xRot += 5.0f;

	if (xRot > 356.0f) xRot = 0.0f;
	if (xRot < -1.0f) xRot = 355.0f;
	if (yRot > 356.0f) yRot = 0.0f;
	if (yRot < -1.0f) yRot = 355.0f;

	glutPostRedisplay();
}

// to paint the orbits
void orbit(GLfloat r){
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f); glRotatef(yRot, 0.0f, 1.0f, 0.0f); // to rotate when press special key
	glColor3f(1.0f, 1.0f, 1.0f); // orbit color (white)
	glBegin(GL_LINE_LOOP);
	for (GLfloat i = 0; i < 2 * pi; i += pi / 50){
		GLfloat x = r*sin(i), z = r*cos(i);
		glVertex3f(x, 0, z);
	}
	glEnd();
	glPopMatrix();
}

// to paient the planets
void planet(GLfloat r, GLfloat g, GLfloat b, GLsizei speed, GLsizei tr, GLsizei sz){
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f); glRotatef(yRot, 0.0f, 1.0f, 0.0f); // to rotate when press special key
	glColor3f(r, g, b); // planet color
	glRotatef(rot[speed], 0.0f, 1.0f, 0.0f);  // planet speed
	glTranslatef(tr, 0, 0); // planet position
	glutSolidSphere(sz, 100, 100); // the planet :]
	glPopMatrix();
}

// to paint the moons
void moon(GLsizei planetSpeed, GLsizei speed, GLsizei r, GLsizei tr1, GLsizei tr2, GLsizei sz){
	glPushMatrix();
	glRotatef(xRot, 1.0f, 0.0f, 0.0f); glRotatef(yRot, 0.0f, 1.0f, 0.0f); // to rotate when press special key
	glRotatef(rot[planetSpeed], 0.0f, 1.0f, 0.0f); // to make moon orbit speed the same as it's planet
	glColor3f(1, 1, 1); // moon orbit color
	glTranslatef(tr1, 0, 0); // to get planet position
	glRotatef(rot[speed], 0.0f, 1.0f, 0.0f); // moon speed
	glBegin(GL_LINE_LOOP);
	for (GLfloat i = 0; i < 2 * pi; i += pi / 50){
		GLfloat x = r * sin(i), z = r * cos(i);
		glVertex3f(x, 0, z);
	}
	glEnd();
	glTranslatef(tr2, 0, 0); // moon position
	glutSolidSphere(sz, 100, 100); // the moon :D
	glPopMatrix();
}

// t0 paint the stars
void stars(){
	glPushMatrix();
	glBegin(GL_POINTS);
	for (int i = 0; i < 200; i++)
		glVertex2f(strx[i], stry[i]);
	glEnd();
	glPopMatrix();
}

void RenderScene(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();

	// plantes orbit
	orbit(190); orbit(140); orbit(80);

	// The Sun
	glColor3f(1, 1, 0); // Sun color
	glutSolidSphere(30, 100, 100);

	// planet(red, green, blue, speed, translate, size)
	// moon(planetSpeed, moonSpeed, orbit radius, planTranslate, moonTranslate, size){

	// planet 1 and 2 moons 
	planet(0.373, 0.059, 0.251, 2, 190, 19);
	moon(2, 3, 30, 190, 30, 5);
	moon(2, 1, 40, 190, 40, 3);

	// planet 2 and 3 moons
	planet(0.957, 0.635, 0.38, 0, 140, 23);
	moon(0, 0, 30, 140, 30, 6);
	moon(0, 1, 40, 140, 40, 4);
	moon(0, 2, 50, 140, 50, 2);

	// planet 3 and 1 moon
	planet(0.906, 0.435, 0.318, 1, 80, 15);
	moon(1, 3, 30, 80, 30, 6);
	moon(1, 1, 40, 80, 40, 4);

	// stars (*-*)
	stars();

	glutSwapBuffers();
}


void TimerFunction(int value){
	for (int i = 0; i < 5; i++){
		rot[i] += sp[i];
		if (rot[i] >= 360)
			rot[i] -= 360;
	}
	glutPostRedisplay();
	glutTimerFunc(30, TimerFunction, 1);
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(700, 600);
	glutCreateWindow("Kemo Planets");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutSpecialFunc(SpecialKeys);
	glutTimerFunc(30, TimerFunction, 1);
	SetupRc();
	glutMainLoop();
}