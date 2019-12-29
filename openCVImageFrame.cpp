#include "openCVImageFrame.h"


ImageFrame::ImageFrame(const char* szFrameTitle, const char* vShaderCode, const char* fShaderCode){
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
	this->isHide = false;
}
ImageFrame::ImageFrame(ImageFrame& orgFrame){
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
}
ImageFrame::~ImageFrame(){
	this->destroyFrame();
}
// ���ٴ���
void ImageFrame::destroyFrame(){
	delete this->GLObject;
	delete this->shader;
	this->GLObject = NULL;
	this->shader = NULL;
	this->frame.release();
}
// ��ȡͼƬ���
int ImageFrame::getResWidth(){
	return this->imgWidth;
}
// ��ȡͼƬ�߶�
int ImageFrame::getResHeight(){
	return this->imgHeight;
}
// ��ȡ���ڿ��
int ImageFrame::getFrameWidth(){
	return this->frameWidth;
}
// ��ȡ���ڸ߶�
int ImageFrame::getFrameHeight(){
	return this->frameHeight;
}
// ��ȡ����֡����ͼƬ�̶�Ϊ1
int ImageFrame::getFrameCount(){
	return this->frameCount;
}
// ��ȡ֡�� �̶�Ϊ0
int ImageFrame::getFrameRate(){
	return this->frameRate;
}
//��ȡ֡ʱ����
int ImageFrame::getFrameDelay(){
	return this->delay;
}
// ���ô���λ��
void ImageFrame::setFramePos(int x, int y){
	this->x = x;
	this->y = y;
	this->GLObject->setGLWindowPos(x, y);
}
// ���ô��ڴ�С
void ImageFrame::setFrameSize(int _width, int _height){
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
        // ��һ��������
		x1, y0,  1.0f,1.0f,	// ���Ͻ�
		x1, y1,  1.0f,0.0f,	// ���½�
		x0, y0,  0.0f,1.0f,	// ���Ͻ�
		// �ڶ���������
		x1, y1,  1.0f,0.0f,	// ���½�
		x0, y1, 0.0f,0.0f,	// ���½�
		x0, y0,  0.0f,1.0f 	// ���Ͻ�
    };
	this->GLObject->resizeGLWindowSize(this->frameWidth, this->frameHeight);
	this->GLObject->setVerticesData(vertices, sizeof(vertices));
}
// ���ش���
void ImageFrame::hideFrame(){
	this->isHide = true;
	this->GLObject->hideGLWindow();
}
// ��ʾ����
void ImageFrame::showFrame(){
	this->isHide = false;
	this->GLObject->showGLWindow();
}
// չʾ����
void ImageFrame::displayFrame(){
	if(this->isHide){
		waitKey(this->delay);
		return;
	}
	this->GLObject->render();
	waitKey(this->delay);
}
// ������Դ
bool ImageFrame::loadResource(const char* szFileName){
	bool result = false;
	do {
		if(!szFileName[0]){
			break;
		}
		PBYTE pBits = preCVReadFile(szFileName);
		if(!pBits){
			break;
		}
		if(pBits[0] == 0x42 && pBits[1] == 0x4D){
			
		} else if(pBits[0] == 0xFF && pBits[1] == 0xD8){// JPEG��ʽ
			
		} else if(
			pBits[0] == 0x89
			&& pBits[1] == 0x50
			&& pBits[2] == 0x4E
			&& pBits[3] == 0x47
			&& pBits[4] == 0x0D
			&& pBits[5] == 0x0A
			&& pBits[6] == 0x1A
			&& pBits[7] == 0x0A
		){
			
		}else if(
			pBits[0] == 0x47
			&& pBits[1] == 0x49
			&& pBits[2] == 0x46
			&& pBits[3] == 0x38
			&& (pBits[4] == 0x39 || pBits[4] == 0x37)
			&& pBits[5] == 0x61
		){
			
		}else {
			free(pBits);
			break;
		}
		free(pBits);
		Mat src = imread(szFileName, IMREAD_COLOR);
		if(!this->frame.empty()){
			this->frame.release();
		}
		this->frame = src.clone();
		this->imgWidth = src.cols;
		this->imgHeight = src.rows;
		if(!this->frameWidth && !this->frameHeight){
			this->frameWidth = this->imgWidth;
			this->frameHeight = this->imgHeight;
		}
		result = true;
		this->GLObject->setTextureData(this->imgWidth, this->imgHeight, (unsigned char*)this->frame.data);
		this->setFrameSize(this->frameWidth, this->frameHeight);
		
	}while(false);
	
	return result;
}

void ImageFrame::setContext(int val){
	this->GLObject->setGLContext(val);
}

GLFWwindow* ImageFrame::getFrameWindow(){
	return this->GLObject->getGLWindow();
}

int ImageFrame::getFrameWndHandle(){
	return this->GLObject->getGLWndHandle();
}


