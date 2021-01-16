#include <stdlib.h>
#include <time.h>
#include <deque>
#include <GL\glut.h>

// to random the food position
GLsizei randomFood(GLsizei min, GLsizei max) {
	static GLboolean first = true;
	if (first){
		srand(time(NULL));
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}

GLshort foodX = randomFood(-99, 99), foodY = randomFood(-99, 99), speed = 2, size;
GLboolean u, l, r, d, eat;

struct body{
	body(GLshort x, GLshort y) :x(x), y(y){}
	GLshort x, y;
	bool operator == (body &b){
		return b.x == x && b.y == y;
	}
};

std::deque<body>snake;
std::deque<body>path;

void SetupRc(){
	snake.push_back(body(0, 0));
	path.push_back(body(0, 0));
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Screen Color (Black)
}

// to resize window and not to damge shapes
void ChangeSize(GLsizei w, GLsizei h) {
	if (h == 0)	h = 1;
	glViewport(0, 0, w, h); glMatrixMode(GL_PROJECTION); glLoadIdentity();
	GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;
	if (w <= h)
		glOrtho(-100.0, 100.0, -100 / aspectRatio, 100.0 / aspectRatio, -100.0, 100.0);
	else
		glOrtho(-100.0 * aspectRatio, 100.0 * aspectRatio, -100.0, 100.0, -100.0, 100.0);
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
}

// to move the snake
void SpecialKeys(GLsizei key, GLsizei x, GLsizei y) {
	if (key == GLUT_KEY_LEFT && !r)
		l = 1, r = 0, d = 0, u = 0;
	else if (key == GLUT_KEY_RIGHT && !l)
		l = 0, r = 1, d = 0, u = 0;
	else if (key == GLUT_KEY_DOWN && !u)
		l = 0, r = 0, d = 1, u = 0;
	else if (key == GLUT_KEY_UP && !d)
		l = 0, r = 0, d = 0, u = 1;
}


// to know if the snake die
void die(){
	for (GLshort i = 2; i < snake.size(); i++){
		if (snake[0] == path[i]){
			l = 0, r = 0, d = 0, u = 0, eat = 0, size = 0, speed = 2;
			path.clear(); snake.clear();
			snake.push_back(body(0, 0)); path.push_back(body(0, 0));
		}
	}
}

// to know if the snake ate the food
void foodEaten(){
	if (abs(snake[0].x - foodX) <= speed && abs(snake[0].y - foodY) <= speed){
		snake.push_back(body(path[path.size() - 1]));
		eat = 1; size++;
	}
// to increase snake speed
	if (size == 4){
		if (speed < 4)
			speed++;
	}
}

// paint snake body & food
void RenderScene(){
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(12);
	glBegin(GL_POINTS);
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(snake[0].x, snake[0].y);
	glColor3f(1.0, 1.0, 1.0);
	for (GLshort i = 1; i < snake.size(); i++){
		glVertex2f(snake[i].x, snake[i].y);
	}
	glEnd();
	if (!eat){
		glPointSize(8);
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_POINTS);
		glVertex2f(foodX, foodY);
		glEnd();
	}
	glFlush();
}

void TimerFunction(int value){
	if (l)
		path[0].x -= speed;
	else if (r)
		path[0].x += speed;
	else if (u)
		path[0].y += speed;
	else if (d)
		path[0].y -= speed;

// to handle snake out of the screen
	if (path[0].x >= 100)
		path[0].x = -100;
	else if (path[0].x <= -100)
		path[0].x = 100;

	if (path[0].y >= 100)
		path[0].y = -100;
	else if (path[0].y <= -100)
		path[0].y = 100;

// push new path
	path.push_front(path[0]);
	if (path.size() > snake.size() + 1)
		path.pop_back();

// set the new path to each part of the body
	for (int i = 0; i < snake.size(); i++)
		snake[i] = path[i + 1];

	die();
	foodEaten();

	if (eat){
		foodX = randomFood(-99, 99);
		foodY = randomFood(-99, 99);
		eat = 0;
	}

	glutPostRedisplay();
	glutTimerFunc(30, TimerFunction, 1);
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Kemo Snake");
	glutReshapeFunc(ChangeSize);
	glutSpecialUpFunc(SpecialKeys);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(30, TimerFunction, 1);
	SetupRc();
	glutMainLoop();
}