#include<cmath>
#include <GL\glut.h>
GLfloat xRot[4], yRot[4], pi = acos(-1);
GLboolean ul, dl, ur, dr;
void SetupRc(){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Screen Color (Black)
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

void shape(GLfloat  x, GLfloat  y, GLint i, GLfloat r, GLfloat g, GLfloat b){
	glPushMatrix();
	glColor3f(r, g, b);  // shape color
	// to rotate when press special key
	glTranslatef(x, y, 0);
	glRotatef(xRot[i], 1.0, 0.0, 0.0);
	glRotatef(yRot[i], 0.0f, 1.0, 0.0f);
	glTranslatef(-x, -y, 0);
	// draw the shape
	glTranslatef(x, y, 0);
	glBegin(GL_LINE_STRIP);
	GLfloat rid = 50.0f, angle = 0.0f;
	for (GLfloat z = -rid; z < rid; z += .1f) {
		GLfloat sin_theta = z / rid, cos_theta = sqrt(1 - sin_theta*sin_theta);
		x = rid*sin(angle)*cos_theta;
		y = rid*cos(angle)*cos_theta;
		angle += 0.1f;
		glVertex3f(x, y, z);
	}
	glEnd(); glPopMatrix();
}

void SpecialKeys(int key, int x, int y) {
	int i = -1;
	if (ul)i = 0;
    else if(dl) i = 1;
	else if (ur) i = 2;
	else if (dr) i = 3;
	if (i != -1){
		if (key == GLUT_KEY_UP) xRot[i] -= 5.0f;
		if (key == GLUT_KEY_DOWN)  xRot[i] += 5.0f;
		if (key == GLUT_KEY_LEFT) yRot[i] -= 5.0f;
		if (key == GLUT_KEY_RIGHT) yRot[i] += 5.0f;
	}
	// Refresh the Window
	glutPostRedisplay();
}

void RenderScene(){
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
	shape(-150, 100, 0, 1.0f, 1.0f, 1.0f);
	shape(-150, -100, 1, 1.0f, 0.0f, 0.0f);
	shape(150, 100, 2, 0.0f, 1.0f, 0.0f);
	shape(150, -100, 3, 0.0f, 0.0f, 1.0f);
	glutSwapBuffers();
}

void ProcessMenu(int val) {
	if (val == 1)
		dl = ur = dr = 0, ul = 1;
	else if (val == 2)
		ul = ur = dr = 0, dl = 1;
	else if (val == 3)
		ul = dl = dr = 0, ur = 1;
	else if (val == 4)
		ul = dl = ur = 0, dr = 1;
	glutPostRedisplay();
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(700, 600);
	glutCreateWindow("3D - 4Shapes");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutSpecialFunc(SpecialKeys);
	glutCreateMenu(ProcessMenu);
	glutAddMenuEntry("UP_LEFT", 1);
	glutAddMenuEntry("DOWN_LEFT", 2);
	glutAddMenuEntry("UP_Right", 3);
	glutAddMenuEntry("DOWN_Right", 4);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	SetupRc();
	glutMainLoop();
}