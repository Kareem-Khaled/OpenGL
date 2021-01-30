#include <string>	 // to use string and it's functions
#include <time.h>	 // to create random numbers
#include <deque>    	 // useful data structure
#include <Windows.h>	 // communication with the OS or filesystem
#include <GL\glut.h> 	 // usnig OpenGL

GLshort foodX, foodY, speed, size, score, scoreUp, trapX, trapY;
GLboolean u, l, r, d, eat;
GLfloat width, height;
GLbyte soungId;

void sound(){
	// specify sound location and name
	std::string str = "sound\\"; str += soungId; str += ".wav";
	LPCSTR lpcstr = str.c_str();
	PlaySound(lpcstr, NULL, SND_FILENAME | SND_ASYNC);
	soungId++;
	if (soungId == '9')
		soungId = '0';
}

// snake body
struct body{
	body(GLshort x, GLshort y) :x(x), y(y){}
	GLshort x, y;
	GLboolean operator == (body &b){
		return b.x == x && b.y == y;
	}
};

std::deque<body>snake, path;

// to random the food position
GLsizei randomFood(GLsizei min, GLsizei max) {
	static GLboolean first = true;
	if (first){
		srand(time(NULL));
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}

// initialize elements
void SetupRc(){
	width = 100, height = 100;
	foodX = randomFood(-width + 1, width - 1), foodY = randomFood(-height + 1, height - 1);
	trapX = randomFood(-width + 1, width - 1), trapY = randomFood(-height + 1, height - 1);
	speed = 2, scoreUp = 2, soungId = '0';

	snake.push_back(body(0, 0));
	path.push_back(body(0, 0));
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);  // Screen Color (Black)
}

// to prevent resizing window 
void ChangeSize(GLsizei w, GLsizei h) {
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glutReshapeWindow(600, 600);
	glOrtho(-width, width, -height, height, -1, 1);
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

// to rebegin the game after dieing
void die(){
	l = 0, r = 0, d = 0, u = 0, eat = 0, size = 0, speed = 2, score = 0, scoreUp = 2;
	path.clear(); snake.clear(); soungId = '9'; sound(); soungId = '0';
	snake.push_back(body(0, 0)); path.push_back(body(0, 0));
}

// to know if the snake die
void isDie(){
	for (GLshort i = 2; i < snake.size(); i++){
		// die with eating itself
		if (snake[0] == path[i])
			die();
	}
	// die with trap
	if (abs(snake[0].x - trapX) <= speed && abs(snake[0].y - trapY) <= speed)
		die();
}

// to know if the snake ate the food
void isEat(){
	if (abs(snake[0].x - foodX) <= speed && abs(snake[0].y - foodY) <= speed){
		snake.push_back(path.back());
		snake.push_back(path.back());
		snake.push_back(path.back());

		eat = 1; size++; score += scoreUp;
		sound();
	}
	// to increase snake speed
	if (size && !(size % 4) && speed < 3)
		speed++;
}

// to paint the Score
void drawScore(std::string text){
	// position of the score
	glRasterPos2i(-width + 10, -height + 10);
	for (int i = 0; i < text.size(); i++){
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
	}
}

// paint snake body & food
void RenderScene(){
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(12);
	glBegin(GL_POINTS);
	// snake head
	glColor3f(1.0, 0.0, 0.0);
	glVertex2f(snake[0].x, snake[0].y);
	// snake body
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
		glColor3f(1.0, 0.9, 0.0);
		glVertex2f(trapX, trapY);
		glEnd();
	}
	glColor3f(0.0, 1.0, 0.0);
	drawScore(std::to_string(score));
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
	if (path[0].x >= width)
		path[0].x = -width;
	else if (path[0].x <= -width)
		path[0].x = width;

	if (path[0].y >= height)
		path[0].y = -height;
	else if (path[0].y <= -height)
		path[0].y = height;

	// push new path
	path.push_front(path[0]);
	if (path.size() > snake.size() + 3)
		path.pop_back();

	// set the new path to each part of the body
	for (int i = 0; i < snake.size(); i++)
		snake[i] = path[i + 1];

	isDie();
	isEat();

	// to randomize new food and new trap
	if (eat){
		foodX = randomFood(-width + 1, width - 1);
		foodY = randomFood(-height + 1, height - 1);
		trapX = randomFood(-width + 1, width - 1);
		trapY = randomFood(-height + 1, height - 1);
		eat = 0;
		if (size % 8 == 0)
			scoreUp *= 2;
	}

	glutPostRedisplay();
	glutTimerFunc(30, TimerFunction, 1);
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(600, 600);				     // identify window size
	glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - 600) / 2,
		(glutGet(GLUT_SCREEN_HEIGHT) - 600) / 2); 	   // identify window position to be centered
	glutCreateWindow("Kemo Snake");				  // window name :)
	glutReshapeFunc(ChangeSize);			  	 // know if window is resized
	glutSpecialUpFunc(SpecialKeys);			 	// to use keys
	glutDisplayFunc(RenderScene);			       // to paint the shapes
	glutTimerFunc(30, TimerFunction, 1);	 	      // to timer the whole game
	SetupRc();					     // to initialize elements
	glutMainLoop();					    // starting of everything
}
