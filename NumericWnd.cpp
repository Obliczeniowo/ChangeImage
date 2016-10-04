#include "myControls.h"

#define SEND_ERROR_MESSAGE

extern HFONT textFont;

HWND CreateNumericWindow(HWND parent,HINSTANCE hInstance,LPSTR title,int id,int x,int y,int width,int height){
	return CreateWindow("NumericWindow",title,WS_CHILD|WS_VISIBLE,x,y,width,height,parent,(HMENU)(__int64)id,hInstance,NULL);
}

NumericWndData* SetNumericWnd(HWND hNumericWnd,double minValue,double maxValue, double devide,double value,bool enable,bool refresh){
	NumericWndData* nwd = (NumericWndData*) GetWindowLongPtr(hNumericWnd, 0);
	nwd->MinValue(minValue);
	nwd->MaxValue(maxValue);
	nwd->Devide(devide);
	nwd->Value(value);
	EnableWindow(hNumericWnd, enable);
	if(refresh)
		InvalidateRect(hNumericWnd, NULL, false);
	return nwd;
}

#pragma region Klasa NumericWndData

NumericWndData::NumericWndData(HWND hWnd,COLORREF bkColor,COLORREF textColor,double minValue,double maxValue,double value,double devide) : ControlData(hWnd,bkColor,textColor)
{
	SetMousePos(hWnd);
	SetWndSize(hWnd);
	this->minValue = minValue;
	this->maxValue = maxValue;
	this->value = value;
	MinValue(minValue);
	MaxValue(maxValue);
	Devide(devide);
	Value(value);
	bkTextColor = RGB(255,255,255);
	frameColor = RGB(125,125,125);
	bkButtonColor = RGB(228,226,213);
	ButtonSelect = RGB(248,246,233);
	ButtonClicked = RGB(193,208,216);
	PathSelected = RGB(208,216,193);
	idSel = idClicked = 0;
	startPointCircle.x = 5;
	startPointCircle.y = wndSize.bottom-6;
}
double NumericWndData::MaxValue(){
	return maxValue;
}
void NumericWndData::MaxValue(double maxValue){
	if(maxValue>minValue){
		this->maxValue = maxValue;
		Value(value);
	}
}
double NumericWndData::MinValue(){
	return minValue;
}
void  NumericWndData::MinValue(double minValue){
	if(minValue<maxValue){
		this->minValue = minValue;
		Value(value);
	}
}
double NumericWndData::Value(){
	return value;
}
void  NumericWndData::Value(double value){
	if(value<minValue)
		this->value = minValue;
	else if(value > maxValue)
		this->value = maxValue;
	else{
		double i = value / MinDValue();
		if( i - floor(i) < 0.5){
			i = floor(i);
		}else{
			i = ceil(i);
		}
		this->value= i * MinDValue();
	}
}
double NumericWndData::Devide(){
	return devide;
}
void NumericWndData::Devide(double devide){
	this->devide = devide;
}
double NumericWndData::MinDValue(){
	return (maxValue-minValue)/devide;
}
void NumericWndData::SetWndSize(HWND hWnd){
	ControlData::SetWndSize(hWnd);
	if(this){
		if(wndSize.bottom!=30 || wndSize.right<50){
			if(wndSize.bottom<30)
				wndSize.bottom=30;
			if(wndSize.right<50)
				wndSize.right=50;
			SetWindowPos(hWnd,NULL,0,0,wndSize.right,wndSize.bottom,SWP_NOMOVE);
		}
	}
}
RECT NumericWndData::GetUpButtonRect(){
	RECT r;
	SetRect(&r,wndSize.right-15,0,wndSize.right,9);
	return r;
}
RECT NumericWndData::GetDownButtonRect(){
	RECT r = GetUpButtonRect();
	//r.bottom++;
	OffsetRect(&r,0,8);
	return r;
}
RECT NumericWndData::GetTextRect(){
	RECT r = wndSize;
	r.right -= GetUpButtonRect().right-GetUpButtonRect().left-1;
	r.bottom = 17;
	return r;
}
RECT NumericWndData::GetPathRect(){
	RECT r;
	SetRect(&r,startPointCircle.x - 2,startPointCircle.y-2,wndSize.right-startPointCircle.x+2,startPointCircle.y+2);
	return r;
}
RECT NumericWndData::GetPathWideRect(){
	RECT r;
	SetRect(&r,startPointCircle.x - 2,17,wndSize.right - startPointCircle.x + 2,wndSize.bottom);
	return r;
}
int NumericWndData::GetCirclePos(){
	int xPos = (int)(startPointCircle.x + (wndSize.right-startPointCircle.x*2)*(value-minValue)/(maxValue-minValue));
	return xPos;
}
void NumericWndData::PaintUpButton(HDC &hdc,Drawing *RYS){
	RECT r = GetUpButtonRect();
	RYS->rectangle(hdc,r,frameColor,PS_SOLID,1,idSel==2?(idClicked==3?ButtonClicked:ButtonSelect):bkButtonColor);
	RYS->line(hdc,r.right - (r.right-r.left)/2-1,3,45,(UINT)5,enable?RGB(0,0,0):RGB(150,150,150),PS_SOLID,1);
	RYS->line(hdc,r.right - (r.right-r.left)/2-1,3,135,(UINT)5,enable?RGB(0,0,0):RGB(150,150,150),PS_SOLID,1);
}
void NumericWndData::PaintDownButton(HDC &hdc,Drawing *RYS){
	RECT r = GetDownButtonRect();
	RYS->rectangle(hdc,r,frameColor,PS_SOLID,1,idSel==3?(idClicked==3?ButtonClicked:ButtonSelect):bkButtonColor);
	RYS->line(hdc,r.right - (r.right-r.left)/2-1,r.bottom-3,-45,(UINT)5,enable?RGB(0,0,0):RGB(150,150,150),PS_SOLID,1);
	RYS->line(hdc,r.right - (r.right-r.left)/2-1,r.bottom-3,-135,(UINT)5,enable?RGB(0,0,0):RGB(150,150,150),PS_SOLID,1);
}
void NumericWndData::PaintPath(HDC &hdc,Drawing *RYS){
	RECT r = GetPathRect();
	RYS->roundRect(hdc,r,2,2,enable?RGB(0,0,0):RGB(150,150,150),PS_SOLID,1,enable?RGB(150,150,150):RGB(200,200,200));
	if(idSel==5){
		if(GetCirclePos()-MousePos.x<0)
			RYS->roundRect(hdc,GetCirclePos()-2,startPointCircle.y-2,(UINT)(MousePos.x-GetCirclePos()+2),(UINT)4,(UINT)2,RGB(0,0,0),PS_SOLID,1,PathSelected);
		else
			RYS->roundRect(hdc,MousePos.x,startPointCircle.y-2,(UINT)(-MousePos.x+GetCirclePos()+2),(UINT)4,(UINT)2,RGB(0,0,0),PS_SOLID,1,PathSelected);
	}
}
void NumericWndData::PaintCircle(HDC &hdc,Drawing *RYS){
	RYS->circle(hdc,GetCirclePos(),startPointCircle.y,5,frameColor,PS_SOLID,1,idSel==4?(idClicked==4?ButtonClicked:ButtonSelect):bkButtonColor);
}
void NumericWndData::WmTimer(HWND hWnd){
	if(idClicked==2){
		Value(value+MinDValue());
		//SendMessage(GetParent(hWnd),WM_COMMAND,(WPARAM)MAKELONG(GetMenu(hWnd),0),(LPARAM)hWnd);
		InvalidateRect(hWnd,NULL,false);
	}else if(idClicked==3){
		Value(value-MinDValue());
		//SendMessage(GetParent(hWnd),WM_COMMAND,(WPARAM)MAKELONG(GetMenu(hWnd),0),(LPARAM)hWnd);
		InvalidateRect(hWnd,NULL,transparent);
	}
}
void NumericWndData::WmLButtonDown(HWND hWnd){
	SetFocus(hWnd);
	if(idSel==1){
		if(idClicked!=1){
			idClicked = 1;
			InvalidateRect(hWnd,NULL,false);
		}
	}else if(idSel==2){ // Button up
		if(idClicked!=2){
			Value(value+this->MinDValue());
			idClicked = 2;
			//SendMessage(GetParent(hWnd),WM_COMMAND,(WPARAM)MAKELONG(GetMenu(hWnd),0),(LPARAM)hWnd);
			SetTimer(hWnd,100,200,NULL);
			InvalidateRect(hWnd,NULL,false);
		}
	}else if(idSel==3){ // Button down
		if(idClicked!=3){
			Value(value-this->MinDValue());
			//SendMessage(GetParent(hWnd),WM_COMMAND,(WPARAM)MAKELONG(GetMenu(hWnd),0),(LPARAM)hWnd);
			idClicked = 3;
			SetTimer(hWnd,100,200,NULL);
			InvalidateRect(hWnd,NULL,false);
		}
	}else if(idSel==4){
		if(idClicked!=4){
			idClicked = 4;
			InvalidateRect(hWnd,NULL,false);
		}
	}else if(idSel==5){
		if(idClicked!=5){
			idClicked = 5;
			double x = MousePos.x - startPointCircle.x;
			double width = wndSize.right - startPointCircle.x * 2;
			x = x / width * (maxValue-minValue) + minValue;
			Value(x);
			InvalidateRect(hWnd,NULL,false);
		}
	}else if(idSel==0){
		if(idClicked!=0){
			idClicked = 0;
			InvalidateRect(hWnd,NULL,false);
		}
	}
	SetCapture(hWnd);
}
void NumericWndData::WmLButtonUp(HWND hWnd){
	if(idClicked==2 || idClicked==3){
		KillTimer(hWnd,100);
	}
	SendMessage(GetParent(hWnd),WM_COMMAND,(WPARAM)MAKELONG(GetMenu(hWnd),0),(LPARAM)hWnd);
	idClicked = 0;
	InvalidateRect(hWnd,NULL,transparent);
	ReleaseCapture();
}
void NumericWndData::WmMouseMove(HWND hWnd){
	SetMousePos(hWnd);
	if(idClicked==4){
		double x = MousePos.x - startPointCircle.x;
		double width = wndSize.right - startPointCircle.x * 2;
		x = x / width * (maxValue-minValue) + minValue;
		Value(x);
		SendMessage(GetParent(hWnd),WM_COMMAND,(WPARAM)MAKELONG(GetMenu(hWnd),0),(LPARAM)hWnd);
		InvalidateRect(hWnd,NULL,transparent);
	}else if(PtInRect(&GetTextRect(),MousePos)){
		if(idSel!=1){
			idSel = 1;
			InvalidateRect(hWnd,NULL,transparent);
		}
	}else if(PtInRect(&GetUpButtonRect(),MousePos)){
		if(idSel!=2){
			idSel = 2;
			InvalidateRect(hWnd,NULL,transparent);
		}
	}else if(PtInRect(&GetDownButtonRect(),MousePos)){
		if(idSel!=3){
			idSel = 3;
			InvalidateRect(hWnd,NULL,transparent);
		}
	}else if(this->PtInCircle(GetCirclePos(),startPointCircle.y,(double)startPointCircle.x,MousePos)){
		if(idSel!=4){
			idSel = 4;
			InvalidateRect(hWnd,NULL,transparent);
		}
	}else if(PtInRect(&GetPathWideRect(),MousePos)){
		idSel = 5;
		InvalidateRect(hWnd,NULL,transparent);
	}else if(idSel!=0){
		idSel = 0;
		InvalidateRect(hWnd,NULL,transparent);
	}
	if(PtInRect(&wndSize,MousePos)){
		POINT pt;
		GetCursorPos(&pt);
		if(WindowFromPoint(pt)!=hWnd){
			ReleaseCapture();
			idSel = idClicked = 0;
			InvalidateRect(hWnd,NULL,transparent);
		}else{
			SetCapture(hWnd);
		}
		SetCapture(hWnd);
	}else if(idClicked!=4){
		ReleaseCapture();
		idSel = idClicked = 0;
		InvalidateRect(hWnd,NULL,transparent);
	}
}
bool NumericWndData::WmKeyDown(HWND hWnd,WPARAM wParam){
	switch(wParam){
		case VK_UP:
			Value(value + MinDValue());
			SendMessage(GetParent(hWnd),WM_COMMAND,(WPARAM)MAKELONG(GetMenu(hWnd),0),(LPARAM)hWnd);
			return true;
		case VK_DOWN:
			Value(value - MinDValue());
			SendMessage(GetParent(hWnd),WM_COMMAND,(WPARAM)MAKELONG(GetMenu(hWnd),0),(LPARAM)hWnd);
			return true;
		case VK_RIGHT:
		case VK_PRIOR:
			Value(value + 5 * MinDValue());
			SendMessage(GetParent(hWnd),WM_COMMAND,(WPARAM)MAKELONG(GetMenu(hWnd),0),(LPARAM)hWnd);
			return true;
		case VK_LEFT:
		case VK_NEXT:
			Value(value - 5 * MinDValue());
			SendMessage(GetParent(hWnd),WM_COMMAND,(WPARAM)MAKELONG(GetMenu(hWnd),0),(LPARAM)hWnd);
			return true;
		case VK_HOME:
			value = MinValue();
			SendMessage(GetParent(hWnd),WM_COMMAND,(WPARAM)MAKELONG(GetMenu(hWnd),0),(LPARAM)hWnd);
			return true;
		case VK_END:
			value = MaxValue();
			SendMessage(GetParent(hWnd),WM_COMMAND,(WPARAM)MAKELONG(GetMenu(hWnd),0),(LPARAM)hWnd);
			return true;
	}
	return false;
}
void NumericWndData::WmPaint(HDC &hdc,Drawing *RYS){
	HDC mem;
	HBRUSH hbr;
	if(this->transparent){
		mem = hdc;
	}else{
		mem = CreateCompatibleDC(hdc);
		SelectObject(mem,hBitmap);
		HBRUSH hbr = CreateSolidBrush(bkColor);
		FillRect(mem, &wndSize, hbr);
		DeleteObject(hbr);
	}
	HPEN hp = CreatePen(PS_SOLID, 1, frameColor);
	SelectObject(mem, hp);
	if(enable){
		RECT textRect = GetTextRect();
		hbr = CreateSolidBrush(idSel==1?(idClicked==1?ButtonClicked:ButtonSelect):bkTextColor);
		SelectObject(mem, hbr);
		Rectangle(mem, textRect.left, textRect.top, textRect.right, textRect.bottom);
		//RYS->rectangle(mem,GetTextRect(),frameColor,PS_SOLID,1,idSel==1?(idClicked==1?ButtonClicked:ButtonSelect):bkTextColor);
	}else{
		RECT textRect = GetTextRect();
		hbr = CreateSolidBrush(RGB(200,200,200));
		SelectObject(mem, hbr);
		Rectangle(mem, textRect.left, textRect.top, textRect.right, textRect.bottom);
		//RYS->rectangle(mem,GetTextRect(),frameColor,PS_SOLID,1,);
	}
	Text myText;
	myText.SetText(value,10);
	SetTextColor(mem,enable?textColor:RGB(100,100,100));
	SetBkMode(mem,TRANSPARENT);
	SelectObject(mem,textFont);
	RECT r;
	SetRect(&r, 4, 0, wndSize.right - 22, 17);
	DrawText(mem, myText.GetText(),-1,&r, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);
	PaintUpButton(mem,RYS);
	PaintDownButton(mem,RYS);
	PaintPath(mem,RYS);
	PaintCircle(mem,RYS);

	DeleteObject(hp);
	DeleteObject(hbr);

	if(!this->transparent){
		BitBlt(hdc,0,0,wndSize.right,wndSize.bottom,mem,0,0,SRCCOPY);
		DeleteDC(mem);
	}
}
void NumericWndData::WmEnable(HWND hWnd){
	Enable(hWnd, !Enable());
	InvalidateRect(hWnd,NULL,false);
}

#pragma endregion

#pragma region Klasa NumericControl

#pragma region Konstruktory

NumericControl::NumericControl(){
	hNumericWndData = NULL;
	nw = NULL;
}

NumericControl::NumericControl(HWND hNumericWndData){
	nw = (NumericWndData*) GetWindowLongPtr(hNumericWndData, 0);
	if(nw){
		this->hNumericWndData = hNumericWndData;
	}else{
		this->hNumericWndData = NULL;
	}
}

NumericControl::NumericControl(HWND parent,HINSTANCE hInstance,LPSTR title,int id,int xPos,int yPos,int width,int height){
	nw = NULL;
	hNumericWndData = NULL;
	CreateWnd(parent, hInstance, title, id, xPos, yPos, width, height);
}

NumericControl::NumericControl(HWND parent, HINSTANCE hInstance,LPSTR title,int id,int xPos,int yPos,int width,int height,NumericWndData *nw){
	nw = NULL;
	hNumericWndData = NULL;
	CreateWnd(parent, hInstance, title, id, xPos, yPos, width, height);
	if(hNumericWndData){
		delete this->nw;
		this->nw = nw;
	}
}

#pragma endregion

#pragma region Destruktor

NumericControl::~NumericControl(){
	// Nothing to do in destructor
	// Nic do zrobienia w destruktorze
}

#pragma endregion

#pragma region Metody

bool NumericControl::CreateWnd(HWND parent,HINSTANCE hInstance,LPSTR title,int id,int xPos,int yPos,int width,int height){
	DestroyWnd();
	hNumericWndData = CreateNumericWindow(parent, hInstance, title, id, xPos, yPos, width, height);
	if(hNumericWndData){
		nw = (NumericWndData*) GetWindowLongPtr(hNumericWndData, 0);
		return nw ? true : false;
	}
	return false;
}

void NumericControl::InvalidateWindow(){
	if(hNumericWndData)
		InvalidateRect(hNumericWndData, NULL, false);
}

void NumericControl::InvalidateWindow(LPRECT lpRect,bool bErase){
	if(hNumericWndData)
		InvalidateRect(hNumericWndData, lpRect, bErase);
}

void NumericControl::DestroyWnd(){
	if(hNumericWndData){
		DestroyWindow(hNumericWndData);
	}
	hNumericWndData = NULL;
	nw = NULL;
}

#pragma endregion

#pragma region Metody ustawiaj¹ce

void NumericControl::SetMaxValue(double MaxValue){
	if(nw)
		nw->MaxValue(MaxValue);
#ifdef SEND_ERROR_MESSAGE
	else
		MessageBox(NULL,"Nie mo¿na ustawiæ wartoœci\nBrak wskaŸnika do NumericWndData","Informacja",MB_OK);
#endif
}

void NumericControl::SetMinValue(double MinValue){
	if(nw)
		nw->MinValue(MinValue);
#ifdef SEND_ERROR_MESSAGE
	else
		MessageBox(NULL,"Nie mo¿na ustawiæ wartoœci\nBrak wskaŸnika do NumericWndData","Informacja",MB_OK);
#endif
}

void NumericControl::SetDevide(double Devide){
	if(nw)
		nw->Devide(Devide);
#ifdef SEND_ERROR_MESSAGE
	else
		MessageBox(NULL,"Nie mo¿na ustawiæ wartoœci\nBrak wskaŸnika do NumericWndData","Informacja",MB_OK);
#endif
}

void NumericControl::SetValue(double Value){
	if(nw){
		nw->Value(Value);
		InvalidateRect(this->hNumericWndData, NULL, false);
	}
#ifdef SEND_ERROR_MESSAGE
	else
		MessageBox(NULL,"Nie mo¿na ustawiæ wartoœci\nBrak wskaŸnika do NumericWndData","Informacja",MB_OK);
#endif
}

void NumericControl::SetBkColor(COLORREF bkColor){
	if(nw)
		nw->bkColor = bkColor;
#ifdef SEND_ERROR_MESSAGE
	else
		MessageBox(NULL,"Nie mo¿na ustawiæ wartoœci\nBrak wskaŸnika do NumericWndData","Informacja",MB_OK);
#endif
}

void NumericControl::SetBkButtonColor(COLORREF bkButtonColor){
	if(nw)
		nw->bkButtonColor = bkButtonColor;
#ifdef SEND_ERROR_MESSAGE
	else
		MessageBox(NULL,"Nie mo¿na ustawiæ wartoœci\nBrak wskaŸnika do NumericWndData","Informacja",MB_OK);
#endif
}

void NumericControl::SetBkTextColor(COLORREF bkTextColor){
	if(nw)
		nw->bkTextColor = bkTextColor;
#ifdef SEND_ERROR_MESSAGE
	else
		MessageBox(NULL,"Nie mo¿na ustawiæ wartoœci\nBrak wskaŸnika do NumericWndData","Informacja",MB_OK);
#endif
}

void NumericControl::SetTextColor(COLORREF TextColor){
	if(nw)
		nw->textColor = TextColor;
#ifdef SEND_ERROR_MESSAGE
	else
		MessageBox(NULL,"Nie mo¿na ustawiæ wartoœci\nBrak wskaŸnika do NumericWndData","Informacja",MB_OK);
#endif
}

void NumericControl::SetEnable(bool e){
	if(nw){
		EnableWindow(this->hNumericWndData,e);
	}
}
#pragma endregion

#pragma region Metody pobieraj¹ce

double NumericControl::GetDevide(){
	if(nw)
		return nw->Devide();
#ifdef SEND_ERROR_MESSAGE
	else
		MessageBox(NULL,"Nie mo¿na pobraæ wartoœci\nBrak wskaŸnika do NumericWndData","Informacja",MB_OK);
#endif
	return 0;
}

double NumericControl::GetMaxValue(){
	if(nw)
		return nw->MaxValue();
#ifdef SEND_ERROR_MESSAGE
	else
		MessageBox(NULL,"Nie mo¿na pobraæ wartoœci\nBrak wskaŸnika do NumericWndData","Informacja",MB_OK);
#endif
	return 0;
}

double NumericControl::GetMinValue(){
	if(nw)
		return nw->MinValue();
#ifdef SEND_ERROR_MESSAGE
	else
		MessageBox(NULL,"Nie mo¿na pobraæ wartoœci\nBrak wskaŸnika do NumericWndData","Informacja",MB_OK);
#endif
	return 0;
}

double NumericControl::GetValue(){
	if(nw)
		return nw->Value();
#ifdef SEND_ERROR_MESSAGE
	else
		MessageBox(NULL,"Nie mo¿na pobraæ wartoœci\nBrak wskaŸnika do NumericWndData","Informacja",MB_OK);
#endif
	return 0;
}

COLORREF NumericControl::GetBkColor(){
	if(nw)
		return nw->bkColor;
#ifdef SEND_ERROR_MESSAGE
	else
		MessageBox(NULL,"Nie mo¿na pobraæ wartoœci\nBrak wskaŸnika do NumericWndData","Informacja",MB_OK);
#endif
	return 0;
}

COLORREF NumericControl::GetBkButtonColor(){
	if(nw)
		return nw->bkButtonColor;
#ifdef SEND_ERROR_MESSAGE
	else
		MessageBox(NULL, "Nie mo¿na pobraæ wartoœci\nBrak wskaŸnika do NumericWndData","Informacja",MB_OK);
#endif
	return 0;
}

COLORREF NumericControl::GetBkTextColor(){
	if(nw)
		return nw->bkTextColor;
#ifdef SEND_ERROR_MESSAGE
	else
		MessageBox(NULL, "Nie mo¿na pobraæ wartoœci\nBrak wskaŸnika do NumericWndData","Informacja",MB_OK);
#endif
	return 0;
}

COLORREF NumericControl::GetTextColor(){
	if(nw)
		return nw->textColor;
#ifdef SEND_ERROR_MESSAGE
	else
		MessageBox(NULL, "Nie mo¿na pobraæ wartoœci\nBrak wskaŸnika do NumericWndData","Informacja",MB_OK);
#endif
	return 0;
}

#pragma endregion

#pragma endregion