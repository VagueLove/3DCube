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
	//����GLSL�������ʧ����Ϣ����
	static void printShaderLog(unsigned int shader);
	//����GLSL����ʧ����Ϣ����
	static void printProgramLog(int prog);
	//���OpenGL������
	static bool checkOpenGLError();
	static unsigned int createShaderProgram();
};
