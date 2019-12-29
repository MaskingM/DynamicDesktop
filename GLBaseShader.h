#include <iostream>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

#ifndef __GLBASESHADER__H_
#define __GLBASESHADER__H_
// 着色器接口类，方便后期拓展
class GLBaseShader{
public:
	GLBaseShader(){};
	virtual ~GLBaseShader(){
	
	};
	// 获取着色器程序
	virtual GLint getGLShaderPrograme() = 0;
	// 使用着色器程序
	virtual void useGLShaderProgram() = 0;
	// 获取着色器Uniform属性
	virtual GLint getGLUniformLocation(const char* name) = 0;
	// 发送数据
	virtual void sendGLUniform1i(const char* name, GLint val) = 0;
	virtual void sendGLUniform1i(GLint location, GLint val) = 0;
	
	virtual void sendGLUniform1f(const char* name, GLfloat val) = 0;
	virtual void sendGLUniform1f(GLint location, GLfloat val) = 0;
	
	virtual void sendGLUniform2i(const char* name, GLint val1, GLint val2) = 0;
	virtual void sendGLUniform2i(GLint location, GLint val1, GLint val2) = 0;
	virtual void sendGLUniform2iv(const char* name, GLint* pVal) = 0;
	virtual void sendGLUniform2iv(GLint location, GLint* pVal) = 0;
	
	virtual void sendGLUniform2f(const char* name, GLfloat val1, GLfloat val2) = 0;
	virtual void sendGLUniform2f(GLint location, GLfloat val1, GLfloat val2) = 0;
	virtual void sendGLUniform2fv(const char* name, GLfloat* pVal) = 0;
	virtual void sendGLUniform2fv(GLint location, GLfloat* pVal) = 0;
	
	virtual void sendGLUniform3i(const char* name, GLint val1, GLint val2, GLint val3) = 0;
	virtual void sendGLUniform3i(GLint location, GLint val1, GLint val2, GLint val3) = 0;
	virtual void sendGLUniform3iv(const char* name, GLint* pVal) = 0;
	virtual void sendGLUniform3iv(GLint location, GLint* pVal) = 0;
	
	virtual void sendGLUniform3f(const char* name, GLfloat val1, GLfloat val2, GLfloat val3) = 0;
	virtual void sendGLUniform3f(GLint location, GLfloat val1, GLfloat val2, GLfloat val3) = 0;
	virtual void sendGLUniform3fv(const char* name, GLfloat* pVal) = 0;
	virtual void sendGLUniform3fv(GLint location, GLfloat* pVal) = 0;
	
	virtual void sendGLUniform4i(const char* name, GLint val1, GLint val2, GLint val3, GLint val4) = 0;
	virtual void sendGLUniform4i(GLint location, GLint val1, GLint val2, GLint val3, GLint val4) = 0;
	virtual void sendGLUniform4iv(const char* name, GLint* pVal) = 0;
	virtual void sendGLUniform4iv(GLint location, GLint* pVal) = 0;
	
	virtual void sendGLUniform4f(const char* name, GLfloat val1, GLfloat val2, GLfloat val3, GLfloat val4) = 0;
	virtual void sendGLUniform4f(GLint location, GLfloat val1, GLfloat val2, GLfloat val3, GLfloat val4) = 0;
	virtual void sendGLUniform4fv(const char* name, GLfloat* pVal) = 0;
	virtual void sendGLUniform4fv(GLint location, GLfloat* pVal) = 0;
	
	virtual void sendGLUniformMatrix2fv(const char* name, GLfloat* pVal) = 0;
	virtual void sendGLUniformMatrix2fv(GLint location, GLfloat* pVal) = 0;
	
	virtual void sendGLUniformMatrix3fv(const char* name, GLfloat* pVal) = 0;
	virtual void sendGLUniformMatrix3fv(GLint location, GLfloat* pVal) = 0;
	
	virtual void sendGLUniformMatrix4fv(const char* name, GLfloat* pVal) = 0;
	virtual void sendGLUniformMatrix4fv(GLint location, GLfloat* pVal) = 0;
};

#endif