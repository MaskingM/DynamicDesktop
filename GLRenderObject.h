#include "GLBaseObj.h"
#include "GLShader.h"

#ifndef __GLRENDEROBJECT__H_
#define __GLRENDEROBJECT__H_

class GLRenderObject: public GLBaseObj
{
private:
	GLBaseShader* shader;
	GLuint VBO;
	GLuint VAO;
	GLuint texture;
	GLint wndWidth; 
	GLint wndHeight;
	GLint frameWidth;
	GLint frameHeight;
	GLFWwindow* window;
	const GLchar* title;
private:
	void initWindow();
public:
	GLRenderObject(int wndWidth, int wndHeight, const GLchar* title);
	~GLRenderObject();
	virtual GLuint getGLVBO();
	virtual GLuint getGLVAO();
	virtual GLint getGLWndWidth();
	virtual GLint getGLWndHeight();
	virtual GLuint getGLTexture();
	virtual GLFWwindow* getGLWindow();
	virtual void setGLShader(GLBaseShader* shader);
	virtual void setTextureData(int width, int height, unsigned char* imgDate, int colorMode = GL_RGB);
	virtual void setVerticesData(GLfloat* data, GLuint size, int drawMode = GL_STATIC_DRAW);
	virtual void render();
	virtual void resizeGLWindowSize(GLsizei w, GLsizei h);
	virtual void hideGLWindow();
	virtual void showGLWindow();
	virtual void setGLWindowPos(int xpos, int ypos);
	virtual void setGLContext(int val);
	virtual int getGLWndHandle();
};

#endif