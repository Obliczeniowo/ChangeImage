#include "myControls.h"

#pragma region ControlData

ControlData::ControlData(HWND hWnd){
	SetWndSize(hWnd);
	SetMousePos(hWnd);
	this->bkColor=RGB(228,226,213);
	this->textColor=RGB(0,0,0);
	this->SetWndSize(hWnd);
	enable = true;
}

ControlData::ControlData(HWND hWnd,COLORREF bkColor,COLORREF textColor){
	SetWndSize(hWnd);
	SetMousePos(hWnd);
	this->bkColor = bkColor;
	this->textColor = textColor;
	this->transparent = false;
	enable = true;
}

ControlData::~ControlData(){
	DeleteObject(hBitmap);
}
bool ControlData::Enable(){
	return enable;
}
void ControlData::Enable(HWND hWnd, bool enable){
	EnableWindow(hWnd, enable);
	this->enable = enable;
}
void ControlData::Enable(bool enable){
	this->enable = enable;
}
void ControlData::SetWndSize(HWND hWnd){
	if(this){
		GetClientRect(hWnd,&wndSize);
		DeleteObject(hBitmap);
		HDC hdc = GetDC(NULL);
		hBitmap = CreateCompatibleBitmap(hdc,wndSize.right,wndSize.bottom);
		DeleteDC(hdc);
	}
}

void ControlData::SetMousePos(HWND hWnd){
	GetCursorPos(&MousePos);
	ScreenToClient(hWnd,&MousePos);
}

void ControlData::SetMousePos(LPARAM lParam){
	MousePos.x = LOWORD(lParam);
	MousePos.y = HIWORD(lParam);
}

POINT ControlData::GetClientMousePos(){
	return MousePos;
}

POINT ControlData::GetScreenMousePos(){
	POINT pt;
	GetCursorPos(&pt);
	return pt;
}

int ControlData::Left(){
	return wndSize.right;
}
int ControlData::Right(){
	return wndSize.right;
}
int ControlData::Top(){
	return wndSize.top;
}
int ControlData::Bottom(){
	return wndSize.bottom;
}
RECT ControlData::WndSize(){
	return wndSize;
}

#pragma endregion