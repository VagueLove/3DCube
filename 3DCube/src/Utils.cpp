#include "Utils.h"

std::string Utils::readShaderSource(const char* filePath)
{
	std::string content;
	std::ifstream fileStream(filePath);
	std::string line = "";
	while (!fileStream.eof())
	{
		getline(fileStream, line);
		content.append(line + "\n");
	}
	fileStream.close();
	return content;
}
//����GLSL�������ʧ����Ϣ����
void Utils::printShaderLog(unsigned int shader)
{
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		log = (char*)malloc(len);
		glGetShaderInfoLog(shader, len, &chWrittn, log);
		std::cout << "Shader Info Log: " << log << std::endl;
		free(log);
	}
}
//����GLSL����ʧ����Ϣ����
void Utils::printProgramLog(int prog)
{
	int len = 0;
	int chWrittn = 0;
	char* log;
	glGetShaderiv(prog, GL_INFO_LOG_LENGTH, &len);
	if (len > 0)
	{
		log = (char*)malloc(len);
		glGetProgramInfoLog(prog, len, &chWrittn, log);
		std::cout << "Program Info Log: " << std::endl;
		free(log);
	}
}
//���OpenGL������
bool Utils::checkOpenGLError()
{
	bool foundError = false;
	int glErr = glGetError();
	while (glErr != GL_NO_ERROR)
	{
		std::cout << "glError: " << glErr << std::endl;
		foundError = true;
		glErr = glGetError();
	}
	return foundError;
}
unsigned int Utils::createShaderProgram()
{
	std::string vertShaderStr = readShaderSource("./res/shaders/vertShader.glsl");
	std::string fragShaderStr = readShaderSource("./res/shaders/fragShader.glsl");
	const char* vertShaderSrc = vertShaderStr.c_str();
	const char* fragShaderSrc = fragShaderStr.c_str();
	//���������ɫ��ʱ�Ĵ���
	int vertComplied;
	int fragComplied;
	int linked;
	//����������ɫ����Ƭ����ɫ��
	unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vShader, 1, &vertShaderSrc, NULL);
	glShaderSource(fShader, 1, &fragShaderSrc, NULL);
	glCompileShader(vShader);
	checkOpenGLError();
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &vertComplied);
	if (vertComplied != 1)
	{
		std::cout << "vertex compilation faild" << std::endl;
		printShaderLog(vShader);
	}
	glCompileShader(fShader);
	checkOpenGLError();
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &fragComplied);
	if (fragComplied != 1)
	{
		std::cout << "fragment compilation faild" << std::endl;
		printShaderLog(fShader);
	}

	//����������ɫ���Ĵ���
	unsigned int vfProgram = glCreateProgram();
	glAttachShader(vfProgram, vShader);
	glAttachShader(vfProgram, fShader);
	glLinkProgram(vfProgram);
	checkOpenGLError();
	glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
	if (linked != 1)
	{
		std::cout << "linking faild" << std::endl;
		printProgramLog(vfProgram);
	}
	return vfProgram;
}
