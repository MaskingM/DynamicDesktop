#include "openCVVedioFrame.h"



VedioFrame::VedioFrame(const char* szFrameTitle, const char* vShaderCode, const char* fShaderCode){
	this->szFrameTitle = szFrameTitle;
	this->vShaderCode = vShaderCode;
	this->fShaderCode = fShaderCode;
	this->frameWidth = 10;
	this->frameHeight = 10;
	this->GLObject = new GLRenderObject(this->frameWidth,this->frameHeight, this->szFrameTitle);
	this->shader = new GLShader(this->vShaderCode,this->fShaderCode);
	this->GLObject->setGLShader(this->shader);
	this->imgWidth = 0;
	this->imgHeight = 0;
	this->frameCount = 0;
	this->frameRate = 25;
	this->x = 0;
	this->y = 0;
	this->delay = 1000 / this->frameRate;
	this->curIdx = 0;
	this->isHide = false;
	this->curTime = 0;
	this->preTime = 0;
}
VedioFrame::VedioFrame(VedioFrame& orgFrame){
	this->szFrameTitle = orgFrame.szFrameTitle;
	this->vShaderCode = orgFrame.vShaderCode;
	this->fShaderCode = orgFrame.fShaderCode;
	this->imgWidth = orgFrame.imgWidth;
	this->imgHeight = orgFrame.imgHeight;
	this->frameWidth = orgFrame.frameWidth;
	this->frameHeight = orgFrame.frameHeight;
	this->frameCount = orgFrame.frameCount;
	this->frameRate = orgFrame.frameRate;
	this->delay = orgFrame.delay;
	this->x = orgFrame.x;
	this->y =orgFrame.y;
	this->GLObject = new GLRenderObject(this->frameWidth,this->frameHeight, this->szFrameTitle);
	this->shader = new GLShader(this->vShaderCode,this->fShaderCode);
	this->GLObject->setGLShader(this->shader);
	this->curIdx = 0;
	this->curTime = 0;
	this->preTime = 0;
}
VedioFrame::~VedioFrame(){
	this->destroyFrame();
}
// 销毁窗口
void VedioFrame::destroyFrame(){
	delete this->GLObject;
	delete this->shader;
	this->GLObject = NULL;
	this->shader = NULL;
	this->frame.release();
	this->vcap.release();
}
// 获取图片宽度
int VedioFrame::getResWidth(){
	return this->imgWidth;
}
// 获取图片高度
int VedioFrame::getResHeight(){
	return this->imgHeight;
}
// 获取窗口宽度
int VedioFrame::getFrameWidth(){
	return this->frameWidth;
}
// 获取窗口高度
int VedioFrame::getFrameHeight(){
	return this->frameHeight;
}
// 获取窗口帧数，图片固定为1
int VedioFrame::getFrameCount(){
	return this->frameCount;
}
// 获取帧率 固定为0
int VedioFrame::getFrameRate(){
	return this->frameRate;
}
//获取帧时间间隔
int VedioFrame::getFrameDelay(){
	return this->delay;
}
// 设置窗口位置
void VedioFrame::setFramePos(int x, int y){
	this->x = x;
	this->y = y;
	this->GLObject->setGLWindowPos(x, y);
}
// 设置窗口大小
void VedioFrame::setFrameSize(int _width, int _height){
	this->frameWidth = _width;
	this->frameHeight = _height;
	if(!this->frameWidth){
		this->frameWidth = 1;
	}
	if(!this->frameHeight){
		this->frameHeight = 1;
	}
	
	float x0 = 0.0f, x1 = 0.0f, y0 = 0.0f, y1 = 0.0f;
	if((float)this->frameWidth / this->frameHeight > (float)this->imgWidth / this->imgHeight){
		y0 = 1.0f;
		y1 = -1.0f;
		x0 = -(float)this->frameHeight / this->imgHeight * this->imgWidth / this->frameWidth;
		x1 = (float)this->frameHeight / this->imgHeight * this->imgWidth / this->frameWidth;
	}else {
		x0 = -1.0;
		x1 = 1.0;
		y0 = (float)this->frameWidth / this->imgWidth * this->imgHeight / this->frameHeight;
		y1 = -(float)this->frameWidth / this->imgWidth * this->imgHeight / this->frameHeight;
	}
	
	float vertices[] = {
        // 第一个三角形
		x1, y0,  1.0f,1.0f,	// 右上角
		x1, y1,  1.0f,0.0f,	// 右下角
		x0, y0,  0.0f,1.0f,	// 左上角
		// 第二个三角形
		x1, y1,  1.0f,0.0f,	// 右下角
		x0, y1, 0.0f,0.0f,	// 左下角
		x0, y0,  0.0f,1.0f 	// 左上角
    };
	this->GLObject->resizeGLWindowSize(this->frameWidth, this->frameHeight);
	this->GLObject->setVerticesData(vertices, sizeof(vertices));
}
// 隐藏窗口
void VedioFrame::hideFrame(){
	this->isHide = true;
	this->GLObject->hideGLWindow();
}
// 显示窗口
void VedioFrame::showFrame(){
	this->isHide = false;;
	this->GLObject->showGLWindow();
}
// 展示窗口
void VedioFrame::displayFrame(){
	if(this->isHide){
		waitKey(this->delay);
		return;
	}
	this->curTime = GetTickCount();//glfwGetTime() * 1000;
	int delta = this->curTime - this->preTime - this->delay;
	if(delta > this->delay){
		delta = 0;
	}else if(delta < 0){
		delta = 0;
	}
	if(this->curIdx >= this->frameCount){
		this->curIdx = 0;
		this->vcap.set(CV_CAP_PROP_POS_FRAMES,0);
	}
	this->vcap.read(this->frame);
	if(!frame.empty()){
		this->GLObject->setTextureData(this->imgWidth, this->imgHeight, (unsigned char*)this->frame.data);
		this->frame.release();
	}
	
	this->GLObject->render();
	this->curIdx++;
	waitKey(this->delay - delta - 1);
	this->preTime = this->curTime;
}
// 加载资源
bool VedioFrame::loadResource(const char* szFileName){
	bool result = false;
	do {
		if(!szFileName[0]){
			break;
		}
		PBYTE pBits = preCVReadFile(szFileName);
		if(!pBits){
			break;
		}
		if(//mpg
			pBits[0] == 0x00
			&& pBits[1] == 0x00
			&& pBits[2] == 0x01
			&& (pBits[3] == 0xBA || pBits[4] == 0xB3)
		){
			
		} else if(
			pBits[4] == 0x66
			&& pBits[5] == 0x74
			&& pBits[6] == 0x79
			&& pBits[7] == 0x70
		){// mp4
			
		}  else if(
			pBits[0] == 0x41
			&& pBits[1] == 0x56
			&& pBits[2] == 0x49
			&& pBits[3] == 0x20
		){// avi
			
		} else if( // mov
			pBits[0] == 0x6D
			&& pBits[1] == 0x6F
			&& pBits[2] == 0x6F
			&& pBits[3] == 0x76
		){
			
		}else if( // rm
			pBits[0] == 0x2E
			&& pBits[1] == 0x52
			&& pBits[2] == 0x4D
			&& pBits[3] == 0x46
		){
			
		}else if( // mkv 42 82 88 6d 61 74 72 6f 73 6b 61
			pBits[5] == 0x42
			&& pBits[6] == 0x82
			&& pBits[7] == 0x88
			&& pBits[8] == 0x6D
			&& pBits[9] == 0x61
			&& pBits[10] == 0x74
		){
			
		}else {
			free(pBits);
			break;
		}
		free(pBits);
		VideoCapture src(szFileName);
		this->vcap.release();
		this->vcap = src;
		this->imgWidth = (int)this->vcap.get(CV_CAP_PROP_FRAME_WIDTH);
		this->imgHeight = (int)this->vcap.get(CV_CAP_PROP_FRAME_HEIGHT);
		this->frameRate = this->vcap.get(CV_CAP_PROP_FPS);
		this->frameCount = this->vcap.get(CV_CAP_PROP_FRAME_COUNT);
		if(this->frameRate == 0){
			this->frameRate = 30;
		}
		this->delay = 1000 / this->frameRate;
		if(!this->frameWidth && !this->frameHeight){
			this->frameWidth = this->imgWidth;
			this->frameHeight = this->imgHeight;
		}
		this->curIdx = 0;
		result = true;
		src.release();
		this->setFrameSize(this->frameWidth, this->frameHeight);
		
	}while(false);
	
	return result;
}

void VedioFrame::setContext(int val){
	this->GLObject->setGLContext(val);
}

GLFWwindow* VedioFrame::getFrameWindow(){
	return this->GLObject->getGLWindow();
}


int VedioFrame::getFrameWndHandle(){
	return this->GLObject->getGLWndHandle();
}

