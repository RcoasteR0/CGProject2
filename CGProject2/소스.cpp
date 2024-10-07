#define _CRT_SECURE_NO_WARNINGS

#include "Shader.h"

#define Quiz1
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void InitBuffer();
GLvoid drawScene();
GLvoid Reshape(int w, int h);

const GLfloat triShape[3][3] = //삼각형 위치 값
{ {-0.5, -0.5, 0.0 }, { 0.5, -0.5, 0.0 }, { 0.0, 0.5, 0.0 } };
const GLfloat colors[3][3] = //삼각형 꼭지점 색상
{ {1.0, 0.0, 0.0 }, { 0.0, 1.0, 0.0 }, { 0.0, 0.0, 1.0 } };
GLuint vao, vbo[2];

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

	glutDisplayFunc(drawScene);		//출력 콜백 함수
	glutReshapeFunc(Reshape);
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
	//triShape 배열의 사이즈 : 9 * float
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	//좌표값을 attribute 인덱스 0 번에 명시한다 : 버텍스 당 3 * float
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//attribute 인덱스 0 번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	//2 번째 VBO 를 활성화 하여 바인드 하고 , 버텍스 속성 색상 을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	//변수 colors 에서 버텍스 색상을 복사한다
	//colors 배열의 사이즈 : 9 * float
	glBufferData(GL_ARRAY_BUFFER, 9 * (sizeof GLfloat), colors, GL_STATIC_DRAW);

	//색상값을 attribute 인덱스 1 번에 명시한다 : 버텍스 당 3 * float
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//attribute 인덱스 1 번을 사용 가능하게 함
	glEnableVertexAttribArray(1);
}

//--- 출력 콜백 함수
GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수
{
	GLfloat rColor, gColor, bColor;

	rColor = gColor = bColor = 1.0; //--- 배경색을 하양색으로 설정
	glClearColor(rColor, gColor, bColor, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(shaderProgramID);

	//사용할 VAO 불러오기
	glBindVertexArray(vao);

	//삼각형 그리기
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glutSwapBuffers(); // 화면에 출력하기
}

//--- 다시그리기 콜백 함수
GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}