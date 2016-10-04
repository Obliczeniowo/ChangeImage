#include "myControls.h"

LRESULT CALLBACK myScaleProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
	myScaleWnd *scale = (myScaleWnd*)GetWindowLong(hWnd,GWL_USERDATA);
	switch(msg){
		case WM_CREATE:
			scale = new myScaleWnd(hWnd,
			break;
		case WM_PAINT:
			break;
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}
myScaleWnd::myScaleWnd(HWND hWnd,CImage image,int StartPosition,bool verticalHorizontal) : myControl(hWnd,RGB(238,236,223),RGB(0,0,0)){
	this->verticalHorizontal = verticalHorizontal;
	ImageSize(image);
	this->StartPosition(StartPosition);
}
void myScaleWnd::ImageSize(CImage image){
	if(verticalHorizontal){
		imageSize = image.GetHeight();
	}else{
		imageSize = image.GetWidth();
	}
}
int myScaleWnd::ImageSize(){
	return imageSize;
}
void myScaleWnd::StartPosition(int startPosition){
	this->startPosition = startPosition;
}
int myScaleWnd::StartPosition(){
	return startPosition;
}