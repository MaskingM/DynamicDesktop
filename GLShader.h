#include "GLBaseShader.h"
#ifndef __GLSHADER___H_
#define __GLSHADER___H_

class GLShader: public GLBaseShader
{
private:
	GLint program;
private:
	string readShaderFile(const char* shaderPath);
	GLuint createShader(int shaderType, const char* shaderCode);
public:
	GLShader(const char* vShaderCode, const char* fShaderCode);
	GLShader(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode);
	virtual ~GLShader();
	// 获取着色器程序
	virtual GLint getGLShaderPrograme();
	// 使用着色器程序
	virtual void useGLShaderProgram();
	// 获取着色器Uniform属性
	virtual GLint getGLUniformLocation(const char* name);
	// 发送数据
	virtual void sendGLUniform1i(const char* name, GLint val);
	virtual void sendGLUniform1i(GLint location, GLint val);
	
	virtual void sendGLUniform1f(const char* name, GLfloat val);
	virtual void sendGLUniform1f(GLint location, GLfloat val);
	
	virtual void sendGLUniform2i(const char* name, GLint val1, GLint val2);
	virtual void sendGLUniform2i(GLint location, GLint val1, GLint val2);
	virtual void sendGLUniform2iv(const char* name, GLint* pVal);
	virtual void sendGLUniform2iv(GLint location, GLint* pVal);
	
	virtual void sendGLUniform2f(const char* name, GLfloat val1, GLfloat val2);
	virtual void sendGLUniform2f(GLint location, GLfloat val1, GLfloat val2);
	virtual void sendGLUniform2fv(const char* name, GLfloat* pVal);
	virtual void sendGLUniform2fv(GLint location, GLfloat* pVal);
	
	virtual void sendGLUniform3i(const char* name, GLint val1, GLint val2, GLint val3);
	virtual void sendGLUniform3i(GLint location, GLint val1, GLint val2, GLint val3);
	virtual void sendGLUniform3iv(const char* name, GLint* pVal);
	virtual void sendGLUniform3iv(GLint location, GLint* pVal);
	
	virtual void sendGLUniform3f(const char* name, GLfloat val1, GLfloat val2, GLfloat val3);
	virtual void sendGLUniform3f(GLint location, GLfloat val1, GLfloat val2, GLfloat val3);
	virtual void sendGLUniform3fv(const char* name, GLfloat* pVal);
	virtual void sendGLUniform3fv(GLint location, GLfloat* pVal);
	
	virtual void sendGLUniform4i(const char* name, GLint val1, GLint val2, GLint val3, GLint val4);
	virtual void sendGLUniform4i(GLint location, GLint val1, GLint val2, GLint val3, GLint val4);
	virtual void sendGLUniform4iv(const char* name, GLint* pVal);
	virtual void sendGLUniform4iv(GLint location, GLint* pVal);
	
	virtual void sendGLUniform4f(const char* name, GLfloat val1, GLfloat val2, GLfloat val3, GLfloat val4);
	virtual void sendGLUniform4f(GLint location, GLfloat val1, GLfloat val2, GLfloat val3, GLfloat val4);
	virtual void sendGLUniform4fv(const char* name, GLfloat* pVal);
	virtual void sendGLUniform4fv(GLint location, GLfloat* pVal);
	
	virtual void sendGLUniformMatrix2fv(const char* name, GLfloat* pVal);
	virtual void sendGLUniformMatrix2fv(GLint location, GLfloat* pVal);
	
	virtual void sendGLUniformMatrix3fv(const char* name, GLfloat* pVal);
	virtual void sendGLUniformMatrix3fv(GLint location, GLfloat* pVal);
	
	virtual void sendGLUniformMatrix4fv(const char* name, GLfloat* pVal);
	virtual void sendGLUniformMatrix4fv(GLint location, GLfloat* pVal);
};

#endif