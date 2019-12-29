#include "GLShader.h"



GLShader::GLShader(const char* vShaderCode, const char* fShaderCode){
	GLuint vShader = this->createShader(GL_VERTEX_SHADER, vShaderCode);
	GLuint fShader = this->createShader(GL_FRAGMENT_SHADER, fShaderCode);
	
	this->program = glCreateProgram();
	glAttachShader(this->program, vShader);
	glAttachShader(this->program, fShader);
	glLinkProgram(this->program);
	GLint success = 0;
	GLchar infoLog[512] = {0};
	glGetProgramiv(this->program, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(this->program, 512, NULL, infoLog);
		cout << "错误: 着色器程序链接错误: " << infoLog << endl;
	}
	glDeleteShader(vShader);
	glDeleteShader(fShader);
}
GLShader::GLShader(const char* vShaderCode, const char* fShaderCode, const char* gShaderCode){
	GLuint vShader = this->createShader(GL_VERTEX_SHADER, vShaderCode);
	GLuint fShader = this->createShader(GL_FRAGMENT_SHADER, fShaderCode);
	GLuint gShader = this->createShader(GL_GEOMETRY_SHADER, gShaderCode);
	this->program = glCreateProgram();
	glAttachShader(this->program, vShader);
	glAttachShader(this->program, fShader);
	glAttachShader(this->program, gShader);
	glLinkProgram(this->program);
	GLint success = 0;
	GLchar infoLog[512] = {0};
	glGetProgramiv(this->program, GL_LINK_STATUS, &success);
	if(!success){
		glGetProgramInfoLog(this->program, 512, NULL, infoLog);
		cout << "错误: 着色器程序链接错误: " << infoLog << endl;
		glDetachShader(this->program, vShader);
		glDetachShader(this->program, fShader);
		glDetachShader(this->program, gShader);
	}
	glDeleteShader(vShader);
	glDeleteShader(fShader);
	glDeleteShader(gShader);
}

GLShader::~GLShader(){
	glDeleteProgram(this->program);
}
string GLShader::readShaderFile(const char* shaderPath){
	string shaderCode;
	ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::badbit);
	try{
		shaderFile.open(shaderPath);
		stringstream sStream;
		sStream << shaderFile.rdbuf();
		shaderFile.close();
		shaderCode = sStream.str();
	}catch(ifstream::failure e){
		cout << "错误: 文件读取错误" << endl;
	}
	return shaderCode;
}
GLuint GLShader::createShader(int shaderType, const char* shaderCode){
	GLuint shader = 0;
	GLint success = 0;
	char buf[512] = {0};
	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glShaderSource(shader, 1, &shaderCode, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success){
		glGetShaderInfoLog(shader, 512, NULL, buf);
		cout << "错误: 着色器编译错误: " << buf << endl;
	}
	return shader;
}
// 获取着色器程序
GLint GLShader::getGLShaderPrograme(){
	return this->program;
}
// 使用着色器程序
void GLShader::useGLShaderProgram(){
	glUseProgram(this->program);
}
// 获取着色器Uniform属性
GLint GLShader::getGLUniformLocation(const char* name){
	return glGetUniformLocation(this->program, name);
}
// 发送数据
void GLShader::sendGLUniform1i(const char* name, GLint val){
	glUniform1i(this->getGLUniformLocation(name), val);
}
void GLShader::sendGLUniform1i(GLint location, GLint val){
	glUniform1i(location, val);
}

void GLShader::sendGLUniform1f(const char* name, GLfloat val){
	glUniform1f(this->getGLUniformLocation(name), val);
}
void GLShader::sendGLUniform1f(GLint location, GLfloat val){
	glUniform1f(location, val);
}

void GLShader::sendGLUniform2i(const char* name, GLint val1, GLint val2){
	glUniform2i(this->getGLUniformLocation(name), val1, val2);
}
void GLShader::sendGLUniform2i(GLint location, GLint val1, GLint val2){
	glUniform2i(location, val1, val2);
}
void GLShader::sendGLUniform2iv(const char* name, GLint* pVal){
	glUniform2iv(this->getGLUniformLocation(name), 1, pVal);
}
void GLShader::sendGLUniform2iv(GLint location, GLint* pVal){
	glUniform2iv(location, 1, pVal);
}

void GLShader::sendGLUniform2f(const char* name, GLfloat val1, GLfloat val2){
	glUniform2f(this->getGLUniformLocation(name), val1, val2);
}
void GLShader::sendGLUniform2f(GLint location, GLfloat val1, GLfloat val2){
	glUniform2f(location, val1, val2);
}
void GLShader::sendGLUniform2fv(const char* name, GLfloat* pVal){
	glUniform2fv(this->getGLUniformLocation(name), 1, pVal);
}
void GLShader::sendGLUniform2fv(GLint location, GLfloat* pVal){
	glUniform2fv(location, 1, pVal);
}

void GLShader::sendGLUniform3i(const char* name, GLint val1, GLint val2, GLint val3){
	glUniform3i(this->getGLUniformLocation(name), val1, val2, val3);
}
void GLShader::sendGLUniform3i(GLint location, GLint val1, GLint val2, GLint val3){
	glUniform3i(location, val1, val2, val3);
}
void GLShader::sendGLUniform3iv(const char* name, GLint* pVal){
	glUniform3iv(this->getGLUniformLocation(name), 1, pVal);
}
void GLShader::sendGLUniform3iv(GLint location, GLint* pVal){
	glUniform3iv(location, 1, pVal);
}

void GLShader::sendGLUniform3f(const char* name, GLfloat val1, GLfloat val2, GLfloat val3){
	glUniform3f(this->getGLUniformLocation(name), val1, val2, val3);
}
void GLShader::sendGLUniform3f(GLint location, GLfloat val1, GLfloat val2, GLfloat val3){
	glUniform3f(location, val1, val2, val3);
}
void GLShader::sendGLUniform3fv(const char* name, GLfloat* pVal){
	glUniform3fv(this->getGLUniformLocation(name), 1, pVal);
}
void GLShader::sendGLUniform3fv(GLint location, GLfloat* pVal){
	glUniform3fv(location, 1, pVal);
}

void GLShader::sendGLUniform4i(const char* name, GLint val1, GLint val2, GLint val3, GLint val4){
	glUniform4i(this->getGLUniformLocation(name), val1, val2, val3, val4);
}
void GLShader::sendGLUniform4i(GLint location, GLint val1, GLint val2, GLint val3, GLint val4){
	glUniform4i(location, val1, val2, val3, val4);
}
void GLShader::sendGLUniform4iv(const char* name, GLint* pVal){
	glUniform4iv(this->getGLUniformLocation(name), 1, pVal);
}
void GLShader::sendGLUniform4iv(GLint location, GLint* pVal){
	glUniform4iv(location, 1, pVal);
}

void GLShader::sendGLUniform4f(const char* name, GLfloat val1, GLfloat val2, GLfloat val3, GLfloat val4){
	glUniform4f(this->getGLUniformLocation(name), val1, val2, val3, val4);
}
void GLShader::sendGLUniform4f(GLint location, GLfloat val1, GLfloat val2, GLfloat val3, GLfloat val4){
	glUniform4f(location, val1, val2, val3, val4);
}
void GLShader::sendGLUniform4fv(const char* name, GLfloat* pVal){
	glUniform4fv(this->getGLUniformLocation(name), 1, pVal);
}
void GLShader::sendGLUniform4fv(GLint location, GLfloat* pVal){
	glUniform4fv(location, 1, pVal);
}

void GLShader::sendGLUniformMatrix2fv(const char* name, GLfloat* pVal){
	glUniformMatrix2fv(this->getGLUniformLocation(name), 1, GL_FALSE, pVal);
}
void GLShader::sendGLUniformMatrix2fv(GLint location, GLfloat* pVal){
	glUniformMatrix2fv(location, 1, GL_FALSE, pVal);
}

void GLShader::sendGLUniformMatrix3fv(const char* name, GLfloat* pVal){
	glUniformMatrix3fv(this->getGLUniformLocation(name), 1, GL_FALSE, pVal);
}
void GLShader::sendGLUniformMatrix3fv(GLint location, GLfloat* pVal){
	glUniformMatrix3fv(location, 1, GL_FALSE, pVal);
}

void GLShader::sendGLUniformMatrix4fv(const char* name, GLfloat* pVal){
	glUniformMatrix4fv(this->getGLUniformLocation(name), 1, GL_FALSE, pVal);
}
void GLShader::sendGLUniformMatrix4fv(GLint location, GLfloat* pVal){
	glUniformMatrix4fv(location, 1, GL_FALSE, pVal);
}





