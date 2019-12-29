#ifndef __OPENCV_VEDIOFRAME___H_
#define __OPENCV_VEDIOFRAME___H_
#include "openCVBaseFrame.h"
#include "GLRenderObject.h"

class VedioFrame: public BaseFrame{
private:
	Mat frame;
	VideoCapture vcap;
	int imgWidth;//图像宽度
	int imgHeight;//图像高度
	int frameWidth;//缓冲区宽度
	int frameHeight;//缓冲区高度
	int frameCount;//帧数量
	int frameRate;//帧率
	int delay;//帧间隔时间
	int x;//缓冲区x位置
	int y;//缓冲区y位置
	bool isHide;//隐藏或显示缓冲区
	GLBaseObj* GLObject;
	GLBaseShader* shader;
	const char* szFrameTitle;
	int curIdx;
	const char* vShaderCode;
	const char* fShaderCode;
	int curTime;
	int preTime;
public:
	VedioFrame(const char* szFrameTitle, const char* vShaderCode, const char* fShaderCode);
	VedioFrame(VedioFrame& orgFrame);
	~VedioFrame();
public:
	virtual int getResWidth();
	virtual int getResHeight();
	virtual int getFrameWidth();
	virtual int getFrameHeight();
	virtual int getFrameCount();
	virtual int getFrameRate();
	virtual int getFrameDelay();
	virtual void setFramePos(int x, int y);
	virtual void setFrameSize(int width, int height);
	virtual void hideFrame();
	virtual void showFrame();
	virtual void destroyFrame();
	virtual void displayFrame();
	virtual bool loadResource(const char* szFileName);
	virtual void setContext(int val);
	virtual GLFWwindow* getFrameWindow();
	virtual int getFrameWndHandle();
};


#endif