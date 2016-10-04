#include "Document.h"

extern Drawing *RYS;

#pragma region Procedura kontrolki vRuler

LRESULT CALLBACK vRulerProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
	switch(msg){
		case WM_SIZE:
			{
				vRulerDataClass *data = (vRulerDataClass*)GetWindowLongPtr(hWnd,0);
				if(data){
					data->WmSize(hWnd);
				}
			}
			break;
		case WM_PAINT:
			{
				vRulerDataClass *data = (vRulerDataClass*)GetWindowLongPtr(hWnd,0);
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd,&ps);
				if(data)
					data->WmPaint(hdc,RYS);
				EndPaint(hWnd,&ps);
			}
			break;
		case WM_DESTROY:
			{
			}
			break;
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

#pragma endregion

#pragma region Klasa vRulerDataClass

vRulerDataClass::vRulerDataClass(HWND hWnd,enum unit sUnit){
	this->sUnit = sUnit;
	WmSize(hWnd);
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
	// Obrócenie czcionki o 90 stopni
	ncm.lfStatusFont.lfEscapement	= 900;
	ncm.lfStatusFont.lfOrientation	= 900;
	// Ustawienie czcionki
	font = CreateFontIndirect(&ncm.lfStatusFont);
}
vRulerDataClass::~vRulerDataClass(){
	if(hBmp)
		DeleteObject(hBmp);
	if(font)
		DeleteObject(font);
}
// W³aœciwoœci
double	vRulerDataClass::VResolution(){
	return this->vResolution;
}
void	vRulerDataClass::VResolution(double vResolution){
	this->vResolution = vResolution;
}
int		vRulerDataClass::Y(){
	return y;
}
void	vRulerDataClass::Y(int y){
	this->y = y;
}
int		vRulerDataClass::Height(){
	return height;
}
void	vRulerDataClass::Height(int height){
	this->height = height;
}
double	vRulerDataClass::Scale(){
	return scale;
}
void	vRulerDataClass::Scale(double scale){
	this->scale = scale;
}
enum unit	vRulerDataClass::SUnit(){
	return sUnit;
}
void	vRulerDataClass::SUnit(enum unit sUnit,double vResolution){
	this->sUnit			= sUnit;
	this->vResolution	= vResolution;
}
// Komunikaty
void	vRulerDataClass::WmPaintScale(HDC mem,Drawing *DRAW){
	double h = 0;
	double value = 0;
	DRAW->SetPen(mem,RGB(0,0,0),PS_SOLID,1);
	SetTextAlign(mem,TA_CENTER);
	#define LINE_WSP	0.75
	switch(sUnit){
		case pixel:
			{
				double baseUnit = 10.0;
				double sBaseUnit = baseUnit * scale;
				while(sBaseUnit < 40){
					baseUnit += 5;
					sBaseUnit = baseUnit * scale;
				}
				while(h < this->height){
					DRAW->line(mem,int(WndSize.right * LINE_WSP),(int)h + y,WndSize.right,(int)h + y);
					value = h / scale;
					DRAW->textOut(mem,0,(int)h + y,value,10,RGB(0,0,0),TRANSPARENT);
					h += sBaseUnit;
				}
			}
			break;
		case milimiter:
			{
				double baseUnit = 10.0 * this->vResolution;
				double sBaseUnit = baseUnit * scale;
				while(sBaseUnit < 40){
					baseUnit += 5.0 * this->vResolution;
					sBaseUnit = baseUnit * scale;
				}
				double h = 0;
				DRAW->SetPen(mem,RGB(0,0,0),PS_SOLID,1);
				double value;
				while(h < this->height){
					DRAW->line(mem,int(WndSize.right * LINE_WSP),(int)h + y,WndSize.right,(int)h + y);
					value = h / (scale * vResolution);
					DRAW->textOut(mem,0,(int)h + y,(value - floor(value) < 0.5?floor(value):ceil(value)),10,RGB(0,0,0),TRANSPARENT);
					h += sBaseUnit;
				}
			}
			break;
		case inch:
			{
				double baseUnit = this->vResolution;
				double sBaseUnit = baseUnit * scale;
				while(sBaseUnit < 40){
					baseUnit += 1.0 * this->vResolution;
					sBaseUnit = baseUnit * scale;
				}
				DRAW->SetPen(mem,RGB(0,0,0),PS_SOLID,1);
				while(h < this->height){
					DRAW->line(mem,int(WndSize.right * LINE_WSP),(int)h + y,WndSize.right,(int)h + y);
					value = h / (scale * vResolution);
					DRAW->textOut(mem,0,(int)h + y,(value - floor(value) < 0.5?floor(value):ceil(value)),10,RGB(0,0,0),TRANSPARENT);
					h += sBaseUnit;
				}
			}
			break;
	}
}
void	vRulerDataClass::WmPaint(HDC hdc,Drawing *DRAW){
	HDC mem = CreateCompatibleDC(hdc);
	SelectObject(mem,hBmp);
	if(font)
		SelectObject(mem,font);
	DRAW->SetPen(mem,RGB(150,150,150),PS_SOLID,1);
	DRAW->rectangle(mem,WndSize);
	if(y>0){
		DRAW->fillRectangle(mem,0,0,(UINT)WndSize.right,(UINT)y,RGB(210,210,210));
	}
	if(y+height<WndSize.bottom){
		DRAW->fillRectangle(mem,0,y+height,(UINT)WndSize.right,(UINT)(WndSize.bottom-y-height),RGB(210,210,210));
	}
	WmPaintScale(mem,DRAW);
	BitBlt(hdc,0,0,WndSize.right,WndSize.bottom,mem,0,0,SRCCOPY);
	DeleteDC(mem);
}
void	vRulerDataClass::WmSize(HWND hWnd){
	GetClientRect(hWnd,&WndSize);
	DeleteObject(hBmp);
	HDC hdc = GetDC(NULL);
	hBmp = CreateCompatibleBitmap(hdc,WndSize.right,WndSize.bottom);
	DeleteDC(hdc);
}

#pragma endregion

#pragma region Klasa vRulerClass

vRulerClass::vRulerClass(){
	hVRuler = NULL;
}
vRulerClass::~vRulerClass(){
	if(hVRuler){
		DestroyWindow(hVRuler);
	}

	vRulerDataClass *rClass = RClass();

	if(rClass){
		delete rClass;
	}
}
bool	vRulerClass::InitControl(HWND hParent){
	WNDCLASS wnd;

	wnd.cbClsExtra		= NULL;
	wnd.cbWndExtra		= sizeof(vRulerDataClass);
	wnd.hbrBackground	= NULL;
	wnd.hCursor			= LoadCursor(NULL,IDC_HAND);
	wnd.hInstance		= (HINSTANCE)GetWindowLongPtr(hParent,GWL_HINSTANCE);
	wnd.hIcon			= LoadIcon(NULL,IDI_APPLICATION);
	wnd.lpfnWndProc		= vRulerProc;
	wnd.lpszClassName	= "vertical ruler class";
	wnd.lpszMenuName	= NULL;
	wnd.style			= CS_VREDRAW|CS_HREDRAW;

	if(!RegisterClass(&wnd))
	{
		MessageBox(NULL, "Nie uda³o siê zarejestrowaæ klasy vertical ruler class","ERROR",MB_OK|MB_ICONERROR);
		return false;
	}

	return true;
}
bool	vRulerClass::CreateVRulerWindow(HWND hParent,int xPos,int yPos,int wndWidth,int wndHeight,int id,
										 double vResolution,enum unit sUnit,int y,int height)
{
	this->hVRuler = CreateWindow("vertical ruler class","",WS_CHILD|WS_VISIBLE,xPos,yPos,wndWidth,wndHeight,hParent,(HMENU)id,(HINSTANCE)GetWindowLongPtr(hParent,GWL_HINSTANCE),NULL);

	if(!hVRuler){
		if(!InitControl(hParent)){
			return false;
		}
		this->hVRuler = CreateWindow("vertical ruler class","",WS_CHILD|WS_VISIBLE,xPos,yPos,wndWidth,wndHeight,hParent,(HMENU)id,(HINSTANCE)GetWindowLongPtr(hParent,GWL_HINSTANCE),NULL);
	}

	vRulerDataClass *rClass = new vRulerDataClass(hVRuler,sUnit);
	rClass->VResolution(vResolution);
	rClass->Height(height);
	rClass->Y(y);

	SetWindowLongPtr(hVRuler,0,(LONG_PTR)rClass);

	return true;
}
void	vRulerClass::DestroyVRulerWindow(){
	if(this->hVRuler){
		DestroyWindow(hVRuler);
	}

	vRulerDataClass *rClass = RClass();

	if(rClass){
		delete rClass;
	}
}
vRulerDataClass*	vRulerClass::RClass(){
	if(!this->hVRuler)
		return NULL;
	vRulerDataClass *rClass = (vRulerDataClass*)GetWindowLongPtr(hVRuler,0);
	return rClass;
}
void	vRulerClass::SetRulerSize(int width, int height){
	SetWindowPos(hVRuler,NULL,0,0,width,height,SWP_NOMOVE);
}
void	vRulerClass::Invalidate(){
	InvalidateRect(this->hVRuler,NULL,false);
}
enum unit	vRulerClass::SUnit(){
	vRulerDataClass *vrdc = RClass();
	return vrdc->SUnit();
}
void		vRulerClass::SUnit(enum unit sUnit,double vResolution){
	vRulerDataClass *vrdc = RClass();
	vrdc->SUnit(sUnit,vResolution);
}

#pragma endregion