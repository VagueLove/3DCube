#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Utils.h"
using namespace std;

#define numVAOs 1
#define numVBOs 2

float cameraX, cameraY, cameraZ;
float cubeLocX, cubeLocY, cubeLocZ;

unsigned int tfLoc;
float timeFactor;

unsigned int renderingProgram;
unsigned int vao[numVAOs];
unsigned int vbo[numVBOs];


//分配在display()函数中使用的空间变量，这样在渲染过程中就不用在重新分配了
unsigned int vLoc, projLoc;
int width, height;
float aspect;
glm::mat4 pMat, mMat, vMat, mvMat, rMat, tMat;

void setupVertices(void)
{
	float cubePositions[108] =
	{
		-1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,-1.0f,
		1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f,
		1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,
		-1.0f,-1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,
		-1.0f,-1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,1.0f,
		-1.0f,-1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,1.0f,
		-1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,
		1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,
		-1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,
		1.0f,1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,-1.0f
	};
	float pyramidPositions[54] =
	{
		-1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,0.0f,1.0f,0.0f, //前面
		1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,0.0f,1.0f,0.0f, //右面
		1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,0.0f,1.0f,0.0f, //后面
		-1.0f,-1.0f,1.0f,-1.0f,-1.0f,-1.0f,0.0f,1.0f,0.0f, //左面
		-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,	//底面左前
		1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,-1.0f //地面右前
	};
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubePositions), cubePositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPositions), pyramidPositions, GL_STATIC_DRAW);
}
void init(GLFWwindow* window)
{
	renderingProgram = Utils::createShaderProgram();
	cameraX = 0.0f; cameraY = 0.0f; cameraZ = 420.0f;
	cubeLocX = 0.0f; cubeLocY = -2.0f; cubeLocX = 0.0f;//沿Y轴下移以展示透视
	setupVertices();
}


void display(GLFWwindow* window, double currentTime)
{
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(renderingProgram);

	vLoc = glGetUniformLocation(renderingProgram, "v_matrix");
	projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

	//构建透视矩阵
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	//1.0472 radians = 60 degrees
	pMat = glm::perspective(1.047f, aspect, 0.1f, 1000.0f);

	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(-cameraX, -cameraY, -cameraZ));

	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
	timeFactor = (float)currentTime;
	tfLoc = glGetUniformLocation(renderingProgram, "tf");
	glUniform1f(tfLoc, timeFactor);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	//将vbo关联给顶点着色器中相应的顶点属性
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//调整OpenGL设置绘制模型
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, 10000);

	glUniformMatrix4fv(vLoc, 1, GL_FALSE, glm::value_ptr(vMat));
	timeFactor = (float)currentTime;
	tfLoc = glGetUniformLocation(renderingProgram, "tf");
	glUniform1f(tfLoc, timeFactor);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

	//将vbo关联给顶点着色器中相应的顶点属性
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//调整OpenGL设置绘制模型
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 18, 10000);


}
int main()
{
	using namespace std;
	if (!glfwInit())
		exit(EXIT_FAILURE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(600, 600, "3D-Cube", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK)
		exit(EXIT_FAILURE);
	glfwSwapInterval(1);

	init(window);

	while (!glfwWindowShouldClose(window))
	{
		display(window, glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}