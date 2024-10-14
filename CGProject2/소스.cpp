#define _CRT_SECURE_NO_WARNINGS

#include <random>
#include "Shader.h"

#define Quiz7
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

using namespace std;

const int SHAPES = 10;
GLuint vao, vbo[2], ebo;
void convertXY(int x, int y, float& fx, float& fy);
void UpdateBuffer();
void InitBuffer();
void Color(float& color);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Keyboard(unsigned char key, int x, int y);

float bGCr = 1.0, bGCg = 1.0, bGCb = 1.0;
GLuint shaderPID;

struct Shape
{
	GLfloat shapecoord[4][3] = { { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }, { 0, 0, 0} };
	int state = 0;
	bool states = false;
};

#ifdef Quiz7
int randomIndex()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> random(0, 9);
	return random(gen);
}

Shape shape[10];
int shapecount = 0;
#endif // Quiz7

void main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Example7");

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	make_shaderProgram();
	InitBuffer();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutReshapeFunc(Reshape);
	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(bGCr, bGCg, bGCb, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(shaderProgramID);
	UpdateBuffer();
	glBindVertexArray(vao);
#ifdef Quiz7
	for (int i = 0; i < 10; i++)
	{
		if (shape[i].states)
		{
			if (shape[i].state == 1)
			{
				glPointSize(5.0);
				glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, (void*)(i * 4 * sizeof(GLuint)));
			}
			else if (shape[i].state == 2)
			{
				glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (void*)(i * 4 * sizeof(GLuint)));
			}
			else if (shape[i].state == 3)
			{
				glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(i * 4 * sizeof(GLuint)));
			}
			else if (shape[i].state == 4)
			{
				glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, (void*)(i * 4 * sizeof(GLuint)));
			}
		}
	}
#endif // Quiz7

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);
}

void convertXY(int x, int y, float& fx, float& fy)
{
	int w = WINDOW_WIDTH;
	int h = WINDOW_HEIGHT;
	fx = (float)((x - (float)w / 2.0) * (float)(1.0 / (float)(w / 2.0)));
	fy = -(float)((y - (float)h / 2.0) * (float)(1.0 / (float)(h / 2.0)));
}

void Color(float& color)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> random(0, 1);
	color = random(gen);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
#ifdef Quiz7
	int a = randomIndex();
	switch (key)
	{
	case 'p':
		shape[shapecount].state = 1;
		break;
	case 'l':
		shape[shapecount].state = 2;
		break;
	case 't':
		shape[shapecount].state = 3;
		break;
	case 'r':
		shape[shapecount].state = 4;
		break;
	case 'w':
		switch (shape[a].state)
		{
		case 1:
			shape[a].shapecoord[0][1] += 0.05;
			break;
		case 2:
			shape[a].shapecoord[0][1] += 0.05;
			shape[a].shapecoord[1][1] += 0.05;
			break;
		case 3:
			shape[a].shapecoord[0][1] += 0.05;
			shape[a].shapecoord[1][1] += 0.05;
			shape[a].shapecoord[2][1] += 0.05;
			break;
		case 4:
			shape[a].shapecoord[0][1] += 0.05;
			shape[a].shapecoord[1][1] += 0.05;
			shape[a].shapecoord[2][1] += 0.05;
			shape[a].shapecoord[3][1] += 0.05;
			break;
		}
		break;
	case 'a':
		switch (shape[a].state)
		{
		case 1:
			shape[a].shapecoord[0][0] -= 0.05;
			break;
		case 2:
			shape[a].shapecoord[0][0] -= 0.05;
			shape[a].shapecoord[1][0] -= 0.05;
			break;
		case 3:
			shape[a].shapecoord[0][0] -= 0.05;
			shape[a].shapecoord[1][0] -= 0.05;
			shape[a].shapecoord[2][0] -= 0.05;
			break;
		case 4:
			shape[a].shapecoord[0][0] -= 0.05;
			shape[a].shapecoord[1][0] -= 0.05;
			shape[a].shapecoord[2][0] -= 0.05;
			shape[a].shapecoord[3][0] -= 0.05;
			break;
		}
		break;
	case 's':
		switch (shape[a].state)
		{
		case 1:
			shape[a].shapecoord[0][1] -= 0.05;
			break;
		case 2:
			shape[a].shapecoord[0][1] -= 0.05;
			shape[a].shapecoord[1][1] -= 0.05;
			break;
		case 3:
			shape[a].shapecoord[0][1] -= 0.05;
			shape[a].shapecoord[1][1] -= 0.05;
			shape[a].shapecoord[2][1] -= 0.05;
			break;
		case 4:
			shape[a].shapecoord[0][1] -= 0.05;
			shape[a].shapecoord[1][1] -= 0.05;
			shape[a].shapecoord[2][1] -= 0.05;
			shape[a].shapecoord[3][1] -= 0.05;
			break;
		}
		break;
	case 'd':
		switch (shape[a].state)
		{
		case 1:
			shape[a].shapecoord[0][0] += 0.05;
			break;
		case 2:
			shape[a].shapecoord[0][0] += 0.05;
			shape[a].shapecoord[1][0] += 0.05;
			break;
		case 3:
			shape[a].shapecoord[0][0] += 0.05;
			shape[a].shapecoord[1][0] += 0.05;
			shape[a].shapecoord[2][0] += 0.05;
			break;
		case 4:
			shape[a].shapecoord[0][0] += 0.05;
			shape[a].shapecoord[1][0] += 0.05;
			shape[a].shapecoord[2][0] += 0.05;
			shape[a].shapecoord[3][0] += 0.05;
			break;
		}
		break;
	case 'c':
		shapecount = 0;
		for (int i = 0; i < 10; i++)
		{
			shape[i].state = 0;
			shape[i].states = false;
		}
		break;
	}
#endif // Quiz7

	if(key == 'q')
		glutLeaveMainLoop();

	glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y)
{
	float fx = 0.0, fy = 0.0;
	convertXY(x, y, fx, fy);

#ifdef Quiz7
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && shape[shapecount].state)
	{
		if (shapecount < 10)
		{
			switch (shape[shapecount].state)
			{
			case 1:
				shape[shapecount].shapecoord[0][0] = fx;
				shape[shapecount].shapecoord[0][1] = fy;
				break;
			case 2:
				shape[shapecount].shapecoord[0][0] = fx - 0.25;
				shape[shapecount].shapecoord[0][1] = fy - 0.25;
				shape[shapecount].shapecoord[1][0] = fx + 0.25;
				shape[shapecount].shapecoord[1][1] = fy + 0.25;
				break;
			case 3:
				shape[shapecount].shapecoord[0][0] = fx;
				shape[shapecount].shapecoord[0][1] = fy;
				shape[shapecount].shapecoord[1][0] = fx - 0.1;
				shape[shapecount].shapecoord[1][1] = fy - 0.3;
				shape[shapecount].shapecoord[2][0] = fx + 0.1;
				shape[shapecount].shapecoord[2][1] = fy - 0.3;
				break;
			case 4:
				shape[shapecount].shapecoord[0][0] = fx - 0.25;
				shape[shapecount].shapecoord[0][1] = fy - 0.25;
				shape[shapecount].shapecoord[1][0] = fx + 0.25;
				shape[shapecount].shapecoord[1][1] = fy - 0.25;
				shape[shapecount].shapecoord[2][0] = fx - 0.25;
				shape[shapecount].shapecoord[2][1] = fy + 0.25;
				shape[shapecount].shapecoord[3][0] = fx + 0.25;
				shape[shapecount].shapecoord[3][1] = fy + 0.25;
				break;
			}
			shape[shapecount].states = true;
			shapecount++;
			shape[shapecount].state = shape[shapecount - 1].state;
		}
	}
#endif // Quiz7

}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기
	glBindVertexArray(vao); //--- VAO를 바인드하기


	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
	//--- triShape 배열의 사이즈: 9 * float
#ifdef Quiz7
	glBufferData(GL_ARRAY_BUFFER, SHAPES * 12 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
#endif // Quiz7

	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	//--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	//--- 변수 colors에서 버텍스 색상을 복사한다.
	//--- colors 배열의 사이즈: 9 *float
	glBufferData(GL_ARRAY_BUFFER, SHAPES * 12 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);

#ifdef Quiz7
	GLfloat colors[4][3];
	for (int i = 0; i < SHAPES; i++)
	{
		for (int z = 0; z < 4; z++)
		{
			for (int j = 0; j < 3; j++)
			{
				Color(colors[z][j]);
			}
		}
		glBufferSubData(GL_ARRAY_BUFFER, i * 12 * sizeof(GLfloat), 12 * sizeof(GLfloat), colors);
	}
#endif // Quiz7

	//--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(1);

#ifdef Quiz7
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	GLuint indices[SHAPES * 4];
	for (int i = 0; i < SHAPES; i++)
	{
		indices[i * 4] = i * 4;
		indices[i * 4 + 1] = i * 4 + 1;
		indices[i * 4 + 2] = i * 4 + 2;
		indices[i * 4 + 3] = i * 4 + 3;
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
#endif // Quiz7

}

void UpdateBuffer()
{
#ifdef Quiz7
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	for (int i = 0; i < SHAPES; i++)
	{
		glBufferSubData(GL_ARRAY_BUFFER, i * 12 * sizeof(GLfloat), 12 * sizeof(GLfloat), shape[i].shapecoord);
	}
#endif Quiz7
}