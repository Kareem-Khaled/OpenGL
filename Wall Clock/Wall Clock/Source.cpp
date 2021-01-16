#include <GL\glut.h>
#include<cmath>
GLfloat pi = acos(-1), r = 60, r2 = 55;
static GLsizei Sang = 0, Mang = 0, Hang = 0;
void SetupRc(){
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Screen Color (Black)
}
// to resize window and not to damge shapes
void ChangeSize(GLint w, GLint h) {
	if (h == 0)	h = 1;
	glViewport(0, 0, w, h); glMatrixMode(GL_PROJECTION); glLoadIdentity();
	GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;
	if (w <= h)
		glOrtho(-100.0, 100.0, -100 / aspectRatio, 100.0 / aspectRatio, -100.0, 100.0);
	else
		glOrtho(-100.0 * aspectRatio, 100.0 * aspectRatio, -100.0, 100.0, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
}

void RenderScene(){
	glMatrixMode(GL_MODELVIEW); glLoadIdentity(); glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f); // clock body color (white)
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(2); // lineWidth for clock body 
	glBegin(GL_LINE_LOOP); // draw clock body
	for (GLfloat i = 0; i < 2 * pi; i += pi / 50){
		GLfloat x = r*sin(i), y = r*cos(i);
		glVertex2f(x, y);
	}
	glEnd();

	glBegin(GL_LINES); // draw clock minutes
	GLsizei cnt = 0; // to mark 5's 
	for (GLfloat i = 0; i < 2 * pi; i += pi / 30, cnt++){
		GLfloat x = r2*sin(i), y = r2*cos(i), x2 = r2 - 2, y2 = r2 - 2;
		if (cnt % 5 == 0)
			x2 -= 3, y2 -= 3, cnt = 0;
		x2 *= sin(i); y2 *= cos(i);
		glVertex2f(x, y);
		glVertex2f(x2, y2);
	}
	glEnd(); glPopMatrix();

	// hours
	glPushMatrix(); glRotatef(Hang, 0.0f, 0.0f, -1.0f);
	glLineWidth(8);
	glBegin(GL_LINES);
	glVertex2f(0, -3); glVertex2f(0, 35);
	glEnd(); glPopMatrix();

	// minutes
	glPushMatrix(); glRotatef(Mang, 0.0f, 0.0f, -1.0f);
	glLineWidth(4); glColor3f(0.8f, 1.0f, 0.3f);
	glBegin(GL_LINES);
	glVertex2f(0, -6); glVertex2f(0, 40);
	glEnd(); glPopMatrix();

	// seconds
	glPushMatrix(); glRotatef(Sang, 0.0f, 0.0f, -1.0f);
	glLineWidth(2); glColor3f(1.0f, 0.9f, 0.6f);
	glBegin(GL_LINES);
	glVertex2f(0, -6); glVertex2f(0, 45);
	glEnd(); glPopMatrix();

	glutSwapBuffers();
}

void TimerFunction(int value){
	if (Sang == 354){
		Mang += 6;
		if (Mang % 72 == 0){ // (360/5)=72 Movement of the hour hand
			Hang += 6;
			if (Hang == 360)
				Hang = 0;
		}
		if (Mang == 360)
			Mang = 0;
	}
	Sang += 6;
	if (Sang == 360)
		Sang = 0;
	glutPostRedisplay();
	glutTimerFunc(1000, TimerFunction, 1);
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutCreateWindow("Kemo ClocK");
	glutReshapeFunc(ChangeSize);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(1000, TimerFunction, 1);
	SetupRc();
	glutMainLoop();
}