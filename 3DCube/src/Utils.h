#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Utils
{
public:
	static std::string readShaderSource(const char* filePath);
	//捕获GLSL代码编译失败信息函数
	static void printShaderLog(unsigned int shader);
	//捕获GLSL链接失败信息函数
	static void printProgramLog(int prog);
	//检查OpenGL错误函数
	static bool checkOpenGLError();
	static unsigned int createShaderProgram();
};
