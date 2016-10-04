#include "Document.h"

extern Drawing *RYS;

#pragma region Procedura okna ViewImage

LRESULT CALLBACK ViewImageProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam){
	switch(msg){
		case WM_CREATE:
			{
				ViewImageData *vid = new ViewImageData(hWnd);
				SetWindowLongPtr(hWnd,0,(LONG_PTR)vid);
			}
			break;
		case VI_SETIMAGEDATA:
			{
				ViewImageData *vid = (ViewImageData*) GetWindowLongPtr(hWnd, 0);
				if(vid){
					ImageData* id = (ImageData*)wParam;
					if(id)
						vid->ImData(id);
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				ViewImageData *vid = (ViewImageData*) GetWindowLongPtr(hWnd, 0);
				if(vid){
					POINT MousePos;
					GetCursorPos(&MousePos);
					ScreenToClient(hWnd, &MousePos);
					if(vid->WmLButtonDown(MousePos))
						InvalidateRect(hWnd, NULL, false);
				}
			}
			break;
		case WM_SIZE:
			{
				ViewImageData *vid = (ViewImageData*) GetWindowLongPtr(hWnd, 0);
				if(vid)
					vid->WmSize(hWnd);
			}
			break;
		case WM_PAINT:
			{
				PAINTSTRUCT ps;
				HDC hdc = BeginPaint(hWnd,&ps);
				ViewImageData *vid = (ViewImageData*) GetWindowLongPtr(hWnd, 0);
				if(vid)
					vid->WmPaint(hdc,RYS);
				EndPaint(hWnd,&ps);
			}
			break;
		case WM_DESTROY:
			{
				ViewImageData *vid = (ViewImageData*) GetWindowLongPtr(hWnd, 0);
				if(vid)
					delete vid;
			}
			break;
	}
	return DefWindowProc(hWnd,msg,wParam,lParam);
}

#pragma endregion

#pragma region	Inicjalizacja klasy

bool	InitViewImageClass(HINSTANCE hInstance){
	WNDCLASS wnd;

	wnd.cbClsExtra		= NULL;
	wnd.cbWndExtra		= sizeof(ViewImageData);
	wnd.hbrBackground	= NULL;
	wnd.hCursor			= LoadCursor(NULL,IDC_CROSS);
	wnd.hIcon			= LoadIcon(NULL,IDI_APPLICATION);
	wnd.hInstance		= hInstance;
	wnd.lpfnWndProc		= ViewImageProc;
	wnd.lpszClassName	= "View Image Window";
	wnd.lpszMenuName	= NULL;
	wnd.style			= CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;

	if(RegisterClass(&wnd)) return true;

	return false;
}

#pragma endregion

#pragma region Klasa ViewImageData

ViewImageData::ViewImageData(HWND hWnd){
	this->imData = NULL;
	WmSize(hWnd);
	drawingStyle = viDrawingStyle::DrawPoint;
}
ViewImageData::~ViewImageData(){
	if(imData)
		delete imData;
	if(hBmp)
		DeleteObject(hBmp);
}
// W³aœciwoœci
void	ViewImageData::ImData(ImageData *imData){
	if(imData){
		if(!this->imData)
			delete this->imData;
		this->imData = imData;
		xScale = (double)wndSize.right / imData->Width();
		yScale = (double)wndSize.bottom / imData->Height();
		
		POINT pt;
		pt.x = imData->Width() / 2;
		pt.y = imData->Height() / 2;
		PtOnBitmap(pt);
	}
}
enum ViewImageData::viDrawingStyle ViewImageData::DrawingStyle(){
	return drawingStyle;
}
void ViewImageData::DrawingStyle(ViewImageData::viDrawingStyle drawingStyle){
	this->drawingStyle = drawingStyle;
}
void ViewImageData::PtOnScreen(POINT point){
	RECT r;
	if(xScale < yScale){
		SetRect(&r, 0,(int)((wndSize.bottom - xScale * imData->Height()) * 0.5), (int)(xScale * imData->Width()), (int)((wndSize.bottom - xScale * imData->Height()) * 0.5 + xScale * imData->Height()));
		if(PtInRect(&r, point)  && xScale != 0){
			ptOnScreen = point;
			ptOnBitmap = point;
			ptOnBitmap.x = (int)(ptOnBitmap.x / xScale);
			ptOnBitmap.y -= r.top;
			ptOnBitmap.y = (int)(ptOnBitmap.y / xScale);
		}
	}else{
		SetRect(&r,(int)(((double)wndSize.right - yScale * (double)imData->Width()) * 0.5),0, (int)(((double)wndSize.right - yScale * (double)imData->Width()) * 0.5 + yScale * imData->Width()), (int)(yScale * imData->Height()));
		if(PtInRect(&r, point) && yScale != 0){
			ptOnScreen = point;
			ptOnBitmap = point;
			ptOnBitmap.x -= r.left;
			ptOnBitmap.x = (int) (ptOnBitmap.x / yScale);
			ptOnBitmap.y = (int) (ptOnBitmap.y / yScale);
		}
	}
}

POINT ViewImageData::PtOnBitmap(){
	return ptOnBitmap;
}
void ViewImageData::PtOnBitmap(POINT point){
	if(imData){
		if(point.x >= 0 && point.x < this->imData->Width() && point.y >= 0 && point.y < this->imData->Height()){
			ptOnBitmap = point;
			RECT r;
			if(xScale < yScale){
				SetRect(&r, 0,(int)((wndSize.bottom - xScale * imData->Height()) * 0.5), (int)(xScale * imData->Width()), (int)(xScale * imData->Height()));
				if(xScale != 0){
					ptOnScreen = point;
					ptOnScreen.x = (int)(ptOnScreen.x * xScale);
					ptOnScreen.y *= (int)(ptOnScreen.y * xScale);
					ptOnScreen.y += r.top;
				}
			}else{
				SetRect(&r,(int)(((double)wndSize.right - yScale * (double)imData->Width()) * 0.5),0, (int)(yScale * imData->Width()), (int)(yScale * imData->Height()));
				if(yScale != 0){
					ptOnScreen = point;
					ptOnScreen.x = (int)(ptOnScreen.x * yScale);
					ptOnScreen.x += r.left;
					ptOnScreen.y *= (int)(ptOnScreen.y * yScale);
				}
			}
		}
	}
}
// Komunikaty
bool	ViewImageData::WmLButtonDown(POINT MousePos){
	POINT tempPoint = ptOnScreen;
	PtOnScreen(MousePos);
	if(tempPoint.x == ptOnScreen.x && tempPoint.y == ptOnScreen.x){
		return false;
	}
	return true;
}
void	ViewImageData::WmSize(HWND hWnd){
	GetClientRect(hWnd,&wndSize);
	DeleteObject(hBmp);
	HDC hdc = GetDC(NULL);
	hBmp = CreateCompatibleBitmap(hdc,wndSize.right,wndSize.bottom);
	DeleteDC(hdc);
	if(imData){
		xScale = (double)wndSize.right / imData->Width();
		yScale = (double)wndSize.bottom / imData->Height();
	}
}
void	ViewImageData::WmPaint(HDC hdc,Drawing *DRAW){
	HDC mem = CreateCompatibleDC(hdc);
	SelectObject(mem,this->hBmp);
	DRAW->fillRectangle(mem,0,0,(UINT)wndSize.right,(UINT)wndSize.bottom,RGB(0,0,0));
	if(imData){
		SetStretchBltMode(mem, COLORONCOLOR);
		if(xScale < yScale){
			DRAW->stretchBltBmpOnHdc(mem,imData->image,0,(int)((wndSize.bottom - xScale * imData->Height()) * 0.5),(UINT)(xScale * imData->Width()),(UINT)(xScale * imData->Height()),SRCCOPY);
		}else{
			DRAW->stretchBltBmpOnHdc(mem,imData->image,(int)(((double)wndSize.right - yScale * (double)imData->Width()) * 0.5),0,(UINT)(yScale * imData->Width()),(UINT)(yScale * imData->Height()),SRCCOPY);
		}
	}
	if(drawingStyle != viDrawingStyle::NoStyle){
		int rop = SetROP2(mem, R2_NOT);
		DRAW->line(mem, ptOnScreen.x - 10, ptOnScreen.y, ptOnScreen.x + 10, ptOnScreen.y);
		DRAW->line(mem, ptOnScreen.x, ptOnScreen.y + 10, ptOnScreen.x, ptOnScreen.y - 10);
		SetROP2(mem, rop);
	}
	BitBlt(hdc,0,0,wndSize.right,wndSize.bottom,mem,0,0,SRCCOPY);
	DeleteDC(mem);
}
void	ViewImageData::CopyBitsFromOrginal(){
	this->imData->CopyBitsFromOrginal();
}
void	ViewImageData::CopyBitsToOrginal(){
	this->imData->CopyBitsToOrginal();
}

#pragma endregion

#pragma region Klasa ImageData

ImageData::ImageData(HBITMAP OrginalImage){
	GetObject(OrginalImage,sizeof(bmp),&bmp);
	orginalByte = (BYTE*)bmp.bmBits;
	copyByte = new BYTE[bmp.bmWidthBytes * bmp.bmHeight];
	for(int i = 0; i < bmp.bmWidthBytes * bmp.bmHeight; i++){
		copyByte[i] = orginalByte[i];
	}
	image = OrginalImage;

	//this->drawingStyle = viDrawingStyle::NoStyle;

	//BITMAPINFOHEADER bih;
	//bih.biBitCount		= bmp.bmBitsPixel;
	//bih.biClrImportant	= 0;
	//bih.biClrUsed			= 0;
	//bih.biCompression		= BI_RGB;
	//bih.biHeight			= bmp.bmHeight;
	//bih.biPlanes			= bmp.bmPlanes;
	//bih.biSize			= sizeof(BITMAPINFOHEADER);
	//bih.biSizeImage		= bmp.bmWidthBytes * bmp.bmHeight;
	//bih.biWidth			= bmp.bmWidth;
	//bih.biXPelsPerMeter	= 0;
	//bih.biYPelsPerMeter	= 0;

	//PBITMAPINFO bi = RYS->CreateBitmapInfoStruct(OrginalImage);
}
ImageData::~ImageData(){
	if(copyByte)
	{
		delete[] copyByte;
		copyByte = NULL;
	}
}

#pragma region Metody

void	ImageData::CopyBitsFromOrginal(){
	BYTE *lastAddress = orginalByte + bmp.bmWidthBytes * bmp.bmHeight;
	for(BYTE *oAddress = orginalByte, *cAddress = copyByte;
		oAddress < lastAddress; oAddress ++){
		*cAddress = *oAddress;
		cAddress ++;
	}
}
void	ImageData::CopyBitsToOrginal(){
	BYTE *lastAddress = orginalByte + bmp.bmWidthBytes * bmp.bmHeight;
	for(BYTE *oAddress = orginalByte, *cAddress = copyByte;
		oAddress < lastAddress; oAddress ++){
		*oAddress = *cAddress;
		cAddress ++;
	}
}

#pragma endregion

#pragma region W³aœciwoœci

int	ImageData::Width(){
	return bmp.bmWidth;
}
int	ImageData::Height(){
	return bmp.bmHeight;
}
int	ImageData::WidthBytes(){
	return bmp.bmWidthBytes;
}
int	ImageData::BitsPixel(){
	return this->bmp.bmBitsPixel;
}

#pragma endregion

#pragma endregion