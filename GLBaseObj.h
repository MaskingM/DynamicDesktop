#include "GLBaseShader.h"
#ifndef __GLBASEOBJ__H_
#define __GLBASEOBJ__H_



class GLBaseObj
{
public:
	virtual ~GLBaseObj(){
		
	};
	virtual GLuint getGLVBO() = 0;
	virtual GLuint getGLVAO() = 0;
	virtual GLint getGLWndWidth() = 0;
	virtual GLint getGLWndHeight() = 0;
	virtual GLuint getGLTexture() = 0;
	virtual GLFWwindow* getGLWindow() = 0;
	virtual void setGLShader(GLBaseShader* shader) = 0;
	virtual void setTextureData(int width, int height, unsigned char* imgDate, int colorMode = GL_RGB) = 0;
	virtual void setVerticesData(GLfloat* data, GLuint size, int drawMode = GL_STATIC_DRAW) = 0;
	virtual void render() = 0;
	virtual void resizeGLWindowSize(GLsizei w, GLsizei h) = 0;
	virtual void hideGLWindow() = 0;
	virtual void showGLWindow() = 0;
	virtual void setGLWindowPos(int xpos, int ypos) = 0;
	virtual void setGLContext(int val) = 0;
	virtual int getGLWndHandle() = 0;
};

#endif