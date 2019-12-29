#ifndef _OPENCV_BASE_FRAME_H__
#define _OPENCV_BASE_FRAME_H__
#include <iostream>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/videoio/legacy/constants_c.h>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include "preReadFile.h"
#include "GLBaseObj.h"

using namespace std;
using namespace cv;

class BaseFrame {
public:
	virtual ~BaseFrame(){
		
	}
	virtual int getResWidth() = 0;
	virtual int getResHeight() = 0;
	virtual int getFrameWidth() = 0;
	virtual int getFrameHeight() = 0;
	virtual int getFrameCount() = 0;
	virtual int getFrameRate() = 0;
	virtual int getFrameDelay() = 0;
	virtual void setFramePos(int x, int y) = 0;
	virtual void setFrameSize(int width, int height) = 0;
	virtual void hideFrame() = 0;
	virtual void showFrame() = 0;
	virtual void destroyFrame() = 0;
	virtual void displayFrame() = 0;
	virtual bool loadResource(const char* szFileName) = 0;
	virtual void setContext(int val) = 0;
	virtual GLFWwindow* getFrameWindow() = 0;
	virtual int getFrameWndHandle() = 0;
};

#endif