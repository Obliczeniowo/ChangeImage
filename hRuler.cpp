#include "Document.h"

#include "Document.h"

extern Drawing *RYS;

#pragma region Procedura kontrolki hRuler

LRESULT CALLBACK hRulerProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
	switch(msg){
		case WM_SIZE:
			{
				hRulerDataClass *data = (hRulerDataClass*)GetWindowLongPtr(hWnd,0);
				if(data){
					data->WmSize(hWnd);
				}
			}
			break;
		case WM_PAINT:
			{
				hRulerDataClass *data = (hRulerDataClass*)GetWindowLongPtr(hWnd,0);
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

#pragma region Klasa hRulerDataClass

hRulerDataClass::hRulerDataClass(HWND hWnd,enum unit sUnit){
	this->sUnit = sUnit;
	WmSize(hWnd);
	NONCLIENTMETRICS ncm;
	ncm.cbSize = sizeof(NONCLIENTMETRICS);
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);
	// Ustawienie czcionki
	font = CreateFontIndirect(&ncm.lfStatusFont);
}
hRulerDataClass::~hRulerDataClass(){
	if(hBmp)
		DeleteObject(hBmp);
	if(font)
		DeleteObject(font);
}
// W³aœciwoœci
double	hRulerDataClass::HResolution(){
	return hResolution;
}
void	hRulerDataClass::HResolution(double hResolution){
	this->hResolution = hResolution;
}
int		hRulerDataClass::X(){
	return x;
}
void	hRulerDataClass::X(int x){
	this->x = x;
}
int		hRulerDataClass::Width(){
	return width;
}
void	hRulerDataClass::Width(int width){
	this->width = width;
}
double	hRulerDataClass::Scale(){
	return scale;
}
void	hRulerDataClass::Scale(double scale){
	this->scale = scale;
}
enum unit	hRulerDataClass::SUnit(){
	return this->sUnit;
}
void		hRulerDataClass::SUnit(enum unit sUnit,double hResolution){
	this->sUnit			= sUnit;
	this->hResolution	= hResolution;
}
// Komunikaty
void	hRulerDataClass::WmPaintScale(HDC mem,Drawing *DRAW){
	SetTextColor(mem,RGB(0,0,0));
	SetBkMode(mem,TRANSPARENT);
	DRAW->SetPen(mem,RGB(0,0,0),PS_SOLID,1);
	SetTextAlign(mem,TA_CENTER);
	double value;
	#define LINE_WSP	0.75
	switch(sUnit){
		case pixel:
			{
				double baseUnit = 10.0;
				double sBaseUnit = baseUnit * scale;
				while(sBaseUnit < 40){
					baseUnit += 5.0;
					sBaseUnit = baseUnit * scale;
				}
				double w = 0;
				while(w < this->width){
					DRAW->line(mem,(int)w + x,int(WndSize.bottom * LINE_WSP),(int)w + x,WndSize.bottom);
					value = w / scale;
					DRAW->textOut(mem,(int)w + x,0,value,10);
					w += sBaseUnit;
				}
			}
			break;
		case milimiter:
			{
				double baseUnit = 10.0 * this->hResolution;
				double sBaseUnit = baseUnit * scale;
				while(sBaseUnit < 40){
					baseUnit += 5.0 * this->hResolution;
					sBaseUnit = baseUnit * scale;
				}
				double w = 0;
				DRAW->SetPen(mem,RGB(0,0,0),PS_SOLID,1);
				while(w < this->width){
					DRAW->line(mem,(int)w + x,int(WndSize.bottom * LINE_WSP),(int)w + x,WndSize.bottom);
					value = w / (scale * hResolution);
					DRAW->textOut(mem,(int)w + x,0,(value-floor(value)<0.5?floor(value):ceil(value)),10);
					w += sBaseUnit;
				}
			}
			break;
		case inch:
			{
				double baseUnit = this->hResolution;
				double sBaseUnit = baseUnit * scale;
				while(sBaseUnit < 40){
					baseUnit += 1.0 * this->hResolution;
					sBaseUnit = baseUnit * scale;
				}
				double w = 0;
				DRAW->SetPen(mem,RGB(0,0,0),PS_SOLID,1);
				while(w < this->width){
					DRAW->line(mem,(int)w + x,int(WndSize.bottom * LINE_WSP),(int)w + x,WndSize.bottom);
					value = w / scale / hResolution;
					DRAW->textOut(mem,(int)w + x,0,(value-floor(value)<0.5?floor(value):ceil(value)),10);
					w += sBaseUnit;
				}
			}
			break;
	}
}
void	hRulerDataClass::WmPaint(HDC hdc,Drawing *DRAW){
	HDC mem = CreateCompatibleDC(hdc);
	SelectObject(mem,hBmp);
	if(font)
		SelectObject(mem,font);
	DRAW->SetPen(mem,RGB(150,150,150),PS_SOLID,1);
	DRAW->rectangle(mem,WndSize);
	if(x > 0){
		DRAW->fillRectangle(mem,0,0,(UINT)x,(UINT)WndSize.bottom,RGB(210,210,210));
	}
	if(x + width<WndSize.right){
		DRAW->fillRectangle(mem,x + width,0,(UINT)(WndSize.right - x - width),(UINT)WndSize.bottom,RGB(210,210,210));
	}
	WmPaintScale(mem,DRAW);
	BitBlt(hdc,0,0,WndSize.right,WndSize.bottom,mem,0,0,SRCCOPY);
	DeleteDC(mem);
}
void	hRulerDataClass::WmSize(HWND hWnd){
	GetClientRect(hWnd,&WndSize);
	DeleteObject(hBmp);
	HDC hdc = GetDC(NULL);
	hBmp = CreateCompatibleBitmap(hdc,WndSize.right,WndSize.bottom);
	DeleteDC(hdc);
}

#pragma endregion

#pragma region Klasa hRulerClass

hRulerClass::hRulerClass(){
	hHRuler = NULL;
}
hRulerClass::~hRulerClass(){
	if(hHRuler){
		DestroyWindow(hHRuler);
	}

	hRulerDataClass *rClass = RClass();

	if(rClass){
		delete rClass;
	}
}
bool	hRulerClass::InitControl(HWND hParent){
	WNDCLASS wnd;

	wnd.cbClsExtra		= NULL;
	wnd.cbWndExtra		= sizeof(vRulerDataClass);
	wnd.hbrBackground	= NULL;
	wnd.hCursor			= LoadCursor(NULL,IDC_HAND);
	wnd.hIcon			= LoadIcon(NULL,IDI_APPLICATION);
	wnd.hInstance		= (HINSTANCE)GetWindowLongPtr(hParent,GWL_HINSTANCE);
	wnd.lpfnWndProc		= hRulerProc;
	wnd.lpszClassName	= "horizontal ruler class";
	wnd.lpszMenuName	= NULL;
	wnd.style			= CS_VREDRAW|CS_HREDRAW;

	if(!RegisterClass(&wnd))
	{
		MessageBox(NULL, "Nie uda³o siê zarejestrowaæ klasy vertical ruler class","ERROR",MB_OK|MB_ICONERROR);
		return false;
	}

	return true;
}
bool	hRulerClass::CreateHRulerWindow(HWND hParent,int xPos,int yPos,int wndWidth,int wndHeight,int id,
										 double hResolution,enum unit sUnit,int x,int width)
{
	this->hHRuler = CreateWindow("horizontal ruler class","",WS_CHILD|WS_VISIBLE,xPos,yPos,wndWidth,wndHeight,hParent,(HMENU)id,(HINSTANCE)GetWindowLongPtr(hParent,GWL_HINSTANCE),NULL);

	if(!hHRuler){
		if(!InitControl(hParent)){
			return false;
		}
		this->hHRuler = CreateWindow("horizontal ruler class","",WS_CHILD|WS_VISIBLE,xPos,yPos,wndWidth,wndHeight,hParent,(HMENU)id,(HINSTANCE)GetWindowLongPtr(hParent,GWL_HINSTANCE),NULL);
	}

	hRulerDataClass *rClass = new hRulerDataClass(hHRuler,sUnit);
	rClass->HResolution(hResolution);
	rClass->Width(width);
	rClass->X(x);

	SetWindowLong(hHRuler,0,(LONG)rClass);

	return true;
}
void	hRulerClass::DestroyHRulerWindow(){
	if(this->hHRuler){
		DestroyWindow(hHRuler);
	}

	hRulerDataClass *rClass = RClass();

	if(rClass){
		delete rClass;
	}
}
hRulerDataClass*	hRulerClass::RClass(){
	if(!this->hHRuler)
		return NULL;
	hRulerDataClass *rClass = (hRulerDataClass*)GetWindowLongPtr(hHRuler,0);
	return rClass;
}
void	hRulerClass::SetRulerSize(int width, int height){
	SetWindowPos(hHRuler,NULL,0,0,width,height,SWP_NOMOVE);
}
void	hRulerClass::Invalidate(){
	InvalidateRect(this->hHRuler,NULL,false);
}
enum unit	hRulerClass::SUnit(){
	hRulerDataClass *vrdc = RClass();
	return vrdc->SUnit();
}
void		hRulerClass::SUnit(enum unit sUnit,double hResolution){
	hRulerDataClass *vrdc = RClass();
	vrdc->SUnit(sUnit,hResolution);
}

#pragma endregion