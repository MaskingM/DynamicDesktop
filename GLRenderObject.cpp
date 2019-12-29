#include "GLRenderObject.h"


// void resizeGLFWWndCallback(GLFWwindow* window,int w,int h){
	// cout << "窗口尺寸改变: w = " << w << ", h = " << h << endl;
// }


GLRenderObject::GLRenderObject(int wndWidth, int wndHeight, const GLchar* title){
	this->VBO = 0;
	this->VAO = 0;
	this->texture = 0;
	this->shader = NULL;
	this->wndWidth = wndWidth;
	this->wndHeight = wndHeight;
	this->title = title;
	this->initWindow();
}
void GLRenderObject::initWindow(){
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	
	this->window = glfwCreateWindow(this->wndWidth, wndHeight, this->title, nullptr, nullptr);
	
	glfwWindowHint(GLFW_VISIBLE, GL_TRUE);
	if(nullptr == this->window){
		glfwTerminate();
		cout << "窗口创建失败" << endl;
		return;
	}
	glfwMakeContextCurrent(this->window);
	glewExperimental = GL_TRUE;
	if(GLEW_OK != glewInit()){
		cout << "GLEW 初始化失败" << endl;
		glfwTerminate();
		return;
	}
	
	// glfwSetWindowSizeCallback(this->window, resizeGLFWWndCallback);
	
	int width = 0, height = 0;
	glfwGetFramebufferSize(this->window, &width, &height);
	this->frameWidth = width;
	this->frameHeight = height;
	glViewport(0,0,width,height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CW);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
GLRenderObject::~GLRenderObject(){
	if(this->VBO){
		glDeleteBuffers(1,&(this->VBO));
		this->VBO = 0;
	}
	if(this->VAO){
		glDeleteVertexArrays(1,&(this->VAO));
		this->VAO = 0;
	}
	if(this->texture){
		glDeleteTextures(1,&(this->texture));
		this->texture = 0;
	}
	this->hideGLWindow();
	glfwTerminate();
}
GLuint GLRenderObject::getGLVBO(){
	return this->VBO;
}
GLuint GLRenderObject::getGLVAO(){
	return this->VAO;
}
GLuint GLRenderObject::getGLTexture(){
	return this->texture;
}
void GLRenderObject::setGLShader(GLBaseShader* shader){
	this->shader = shader;
}
void GLRenderObject::setTextureData(int width, int height, unsigned char* imgDate, int colorMode){
	if(this->texture){
		glDeleteTextures(1,&(this->texture));
		this->texture;
	}
	glGenTextures(1, &(this->texture));
	glBindTexture(GL_TEXTURE_2D, this->texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, colorMode, width, height, 0, colorMode, GL_UNSIGNED_BYTE, imgDate);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);
}
void GLRenderObject::setVerticesData(GLfloat* data, GLuint size, int drawMode){
	if(this->VBO){
		glDeleteBuffers(1,&(this->VBO));
		this->VBO;
	}
	if(this->VAO){
		glDeleteVertexArrays(1,&(this->VAO));
		this->VAO;
	}
	glGenVertexArrays(1, &(this->VAO));
    glBindVertexArray(this->VAO);
	
    glGenBuffers(1, &(this->VBO));
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, size, data, drawMode);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
	glBindVertexArray(0);
}
void GLRenderObject::render(){
	glfwPollEvents();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(this->shader){
		this->shader->useGLShaderProgram();
	}
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, this->texture);
	if(this->shader){
		this->shader->sendGLUniform1i("frame",0);
	}
	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
	glfwSwapBuffers(window);
}

GLint GLRenderObject::getGLWndWidth(){
	return this->wndWidth;
}
GLint GLRenderObject::getGLWndHeight(){
	return this->wndHeight;
}
GLFWwindow* GLRenderObject::getGLWindow(){
	return this->window;
}

void GLRenderObject::resizeGLWindowSize(GLsizei w, GLsizei h){
	if(h == 0){
		h = 1;
	}
	if(w == 0){
		w = 1;
	}
	this->wndWidth = w;
	this->wndHeight = h;
	glfwSetWindowSize(this->window, this->wndWidth, this->wndHeight);
	glfwGetFramebufferSize(this->window, &(this->frameWidth), &(this->frameHeight));
	glViewport(0, 0, this->frameWidth, this->frameHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLRenderObject::hideGLWindow(){
	glfwHideWindow(this->window);
}
void GLRenderObject::showGLWindow(){
	glfwShowWindow(this->window);
}

void GLRenderObject::setGLWindowPos(int xpos, int ypos){
	glfwSetWindowPos(this->window, xpos, ypos);
}

void GLRenderObject::setGLContext(int val){
	if(val){
		glfwMakeContextCurrent(this->window);
	}else {
		glfwMakeContextCurrent(NULL);
	}
	
}

int GLRenderObject::getGLWndHandle(){
	return getGLFWWndHandle(this->window);
}
