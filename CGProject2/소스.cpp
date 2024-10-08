#define _CRT_SECURE_NO_WARNINGS

#include <random>
#include "Shader.h"

#define Quiz1
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

using namespace std;

random_device rd;
mt19937 gen(rd());

void InitBuffer();
void UpdateBuffer();
GLvoid drawScene();
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid Timer(int value);

GLuint vao, vbo[2], ebo;

struct Coordinate { GLfloat x = 0, y = 0, z = 0; };
struct RGB { GLfloat Red = 0, Green = 0, Blue = 0; };

class Point
{
public:
	Coordinate Shape;
	RGB colors;

	Point() {}

	Point(Coordinate coord, RGB rgb)
	{
		Shape = coord;
		colors = rgb;
	}

	~Point() {}

	GLfloat* GetCoord()
	{
		GLfloat coord[3] = { Shape.x, Shape.y, Shape.z };
		return coord;
	}

	GLfloat* GetColor()
	{
		GLfloat color[3] = { colors.Red, colors.Green, colors.Blue };
		return color;
	}
};

class Line
{
public:
	Coordinate Shape[2];
	RGB colors;

	Line() {}

	Line(Coordinate coord[2], RGB rgb)
	{
		Shape[0] = coord[0];
		Shape[1] = coord[1];
		colors = rgb;
	}

	~Line() {}

	GLfloat* GetCoord()
	{
		GLfloat coord[6] = { Shape[0].x, Shape[0].y, Shape[0].z, 
							Shape[1].x, Shape[1].y, Shape[1].z };
		return coord;
	}

	GLfloat* GetColor()
	{
		GLfloat color[6] = { colors.Red, colors.Green, colors.Blue,
							colors.Red, colors.Green, colors.Blue };
		return color;
	}

};

class Triangle
{
public:
	Coordinate Shape[3];
	RGB colors;

	Triangle() {}

	Triangle(Coordinate coord[3], RGB rgb)
	{
		Shape[0] = coord[0];
		Shape[1] = coord[1];
		Shape[2] = coord[2];
		colors = rgb;
	}

	~Triangle() {}

	GLfloat* GetCoord()
	{
		GLfloat coord[9] = { Shape[0].x, Shape[0].y, Shape[0].z,
							Shape[1].x, Shape[1].y, Shape[1].z,
							Shape[2].x, Shape[2].y, Shape[2].z };
		return coord;
	}

	GLfloat* GetColor()
	{
		GLfloat color[9] = { colors.Red, colors.Green, colors.Blue,
							colors.Red, colors.Green, colors.Blue,
							colors.Red, colors.Green, colors.Blue };
		return color;
	}

};

class REctangle
{
public:
	Coordinate Shape[4];
	RGB colors;

	REctangle() {}

	REctangle(Coordinate coord[4], RGB rgb)
	{
		Shape[0] = coord[0];
		Shape[1] = coord[1];
		Shape[2] = coord[2];
		Shape[3] = coord[3];
		colors = rgb;
	}

	~REctangle() {}

	GLfloat* GetCoord()
	{
		GLfloat coord[12] = { Shape[0].x, Shape[0].y, Shape[0].z,
							Shape[1].x, Shape[1].y, Shape[1].z, 
							Shape[2].x, Shape[2].y, Shape[2].z, 
							Shape[3].x, Shape[3].y, Shape[3].z };
		return coord;
	}

	GLfloat* GetColor()
	{
		GLfloat color[12] = { colors.Red, colors.Green, colors.Blue,
							colors.Red, colors.Green, colors.Blue,
							colors.Red, colors.Green, colors.Blue,
							colors.Red, colors.Green, colors.Blue };
		return color;
	}

};

#ifdef Quiz1
enum Shape { NONE = -1, POiNT, LINE, TRIANGLE, RECTANGLE };
Point points[10];
Line lines[10];
Triangle triangles[10];
REctangle rects[10];
Shape shape[10];
Shape selectedshape;
int shapecount;
int shapeindex;
int drawindex;

uniform_real_distribution<GLfloat> randcolor(0.0f, 1.0f);

RGB RandomColor()
{
	return { randcolor(gen), randcolor(gen) , randcolor(gen) };
}

void Initialize()
{
	for (int i = 0; i < 10; ++i)
	{
		points[i] = Point();
		lines[i] = Line();
		triangles[i] = Triangle();
		rects[i] = REctangle();
		shape[i] = NONE;
	}

	selectedshape = POiNT;
	shapecount = 0;
	shapeindex = 0;
}
#endif // Quiz1

//메인 함수
void main (int argc, char** argv)	//윈도우 출력하고 콜백함수 설정
{
	//윈도우 생성하기
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Example1");

	//GLEW 초기화하기
	glewExperimental = GL_TRUE;
	glewInit();

	//세이더 읽어와서 세이더 프로그램 만들기
	make_shaderProgram();		//세이더 프로그램 만들기
	InitBuffer();

#ifdef Quiz1
	Initialize();
#endif // Quiz1

	glutDisplayFunc(drawScene);		//출력 콜백 함수
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop();
}

//--- 버퍼 생성하고 데이터 받아오기
void InitBuffer()
{
	glGenVertexArrays(1, &vao); //VAO 를 지정하고 할당하기

	glBindVertexArray(vao); //VAO 를 바인드하기

	glGenBuffers(2, vbo); //2 개의 VBO 를 지정하고 할당하기

	//1 번째 VBO 를 활성화하여 바인드하고 , 버텍스 속성 좌표값 을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다
#ifdef Quiz1
	glBufferData(GL_ARRAY_BUFFER, shapecount * 12 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
#endif // Quiz1

	//좌표값을 attribute 인덱스 0 번에 명시한다 : 버텍스 당 3 * float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//attribute 인덱스 0 번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	//2 번째 VBO 를 활성화 하여 바인드 하고 , 버텍스 속성 색상 을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	//변수 colors 에서 버텍스 색상을 복사한다
#ifdef Quiz1
	glBufferData(GL_ARRAY_BUFFER, 10 * 12 * sizeof(GLfloat), NULL, GL_STATIC_DRAW);
#endif // Quiz1

	//색상값을 attribute 인덱스 1 번에 명시한다 : 버텍스 당 3 * float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//attribute 인덱스 1 번을 사용 가능하게 함
	glEnableVertexAttribArray(1);

#ifdef Quiz1
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	GLuint indices[10 * 4];
	for (int i = 0; i < 10; i++)
	{
		indices[i * 4] = i * 4;
		indices[i * 4 + 1] = i * 4 + 1;
		indices[i * 4 + 2] = i * 4 + 2;
		indices[i * 4 + 3] = i * 4 + 3;
	}
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
#endif // Quiz1

}

void UpdateBuffer()
{
#ifdef Quiz1
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	for (int i = 0; i < shapecount; i++)
	{
		switch (shape[i])
		{
		case POiNT:
			glBufferSubData(GL_ARRAY_BUFFER, i * 12 * sizeof(GLfloat), 3 * sizeof(GLfloat), points[i].GetCoord());
			break;
		case LINE:
			glBufferSubData(GL_ARRAY_BUFFER, i * 12 * sizeof(GLfloat), 6 * sizeof(GLfloat), lines[i].GetCoord());
			break;
		case TRIANGLE:
			glBufferSubData(GL_ARRAY_BUFFER, i * 12 * sizeof(GLfloat), 9 * sizeof(GLfloat), triangles[i].GetCoord());
			break;
		case RECTANGLE:
			glBufferSubData(GL_ARRAY_BUFFER, i * 12 * sizeof(GLfloat), 12 * sizeof(GLfloat), rects[i].GetCoord());
			break;
		default:
			break;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	for (int i = 0; i < shapecount; i++)
	{
		switch (shape[i])
		{
		case POiNT:
			glBufferSubData(GL_ARRAY_BUFFER, i * 12 * sizeof(GLfloat), 3 * sizeof(GLfloat), points[i].GetColor());
			break;
		case LINE:
			glBufferSubData(GL_ARRAY_BUFFER, i * 12 * sizeof(GLfloat), 6 * sizeof(GLfloat), lines[i].GetColor());
			break;
		case TRIANGLE:
			glBufferSubData(GL_ARRAY_BUFFER, i * 12 * sizeof(GLfloat), 9 * sizeof(GLfloat), triangles[i].GetColor());
			break;
		case RECTANGLE:
			glBufferSubData(GL_ARRAY_BUFFER, i * 12 * sizeof(GLfloat), 12 * sizeof(GLfloat), rects[i].GetColor());
			break;
		default:
			break;
		}
	}
#endif // Quiz1

}

//--- 출력 콜백 함수
GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	GLfloat rColor, gColor, bColor;

	rColor = gColor = bColor = 1.0; //--- 배경색을 하양색으로 설정
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(shaderProgramID);
	UpdateBuffer();

	glBindVertexArray(vao);
#ifdef Quiz1
	for (int i = 0; i < shapecount; ++i)
	{
		switch (shape[i])
		{
		case POiNT:
			glPointSize(5.0f);
			glDrawElements(GL_POINT, 1, GL_UNSIGNED_INT, (void*)(i * 4 * sizeof(GLuint)));
			break;
		case LINE:
			glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, (void*)(i * 4 * sizeof(GLuint)));
			break;
		case TRIANGLE:
			break;
		case RECTANGLE:
			break;
		default:
			break;
		}
	}
#endif // Quiz1

	glutSwapBuffers(); // 화면에 출력하기
}

//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
#ifdef Quiz1
	switch (key)
	{
	case 'p':
		selectedshape = POiNT;
		break;
	case 'l':
		selectedshape = LINE;
		break;
	case 't':
		selectedshape = TRIANGLE;
		break;
	case 'r':
		selectedshape = RECTANGLE;
		break;
	case 'w':
		break;
	case 'a':
		break;
	case 's':
		break;
	case 'd':
		break;
	case 'c':
		Initialize();
		break;
	default:
		break;
	}
#endif // Quiz1

	//프로그램 종료
	if (key == 'q')
		glutLeaveMainLoop();

	InitBuffer();
	glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y)
{
	GLfloat mouseX = (x - WINDOW_WIDTH / 2.0f) / (WINDOW_WIDTH / 2.0f);
	GLfloat mouseY = (WINDOW_HEIGHT / 2.0f - y) / (WINDOW_HEIGHT / 2.0f);

#ifdef Quiz1
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && shapecount < 10)
	{
		switch (selectedshape)
		{
		case POiNT:
		{
			points[shapecount] = Point({ mouseX, mouseY, 0.0f }, RandomColor());
			shape[shapecount] = POiNT;
			shapecount++;
			break;
		}
		case LINE:
		{
			Coordinate c[2] = {{mouseX - 0.05f, mouseY - 0.05f, 0.0f}, {mouseX + 0.05f, mouseY + 0.05f, 0.0f}};
			lines[shapecount] = Line(c, RandomColor());
			shape[shapecount] = LINE;
			shapecount++;
			break;
		}
		case TRIANGLE:
		{

			break;
		}
		case RECTANGLE:
		{

			break;
		}
		default:
			break;
		}
	}

#endif // Quiz1

	InitBuffer();
}