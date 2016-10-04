#include "cdibClass.h"

cdib::cdib(const cdib &b){
	if(b.hBitmap && b.pBits){

		bmih = b.bmih;

		pBits = NULL;

		hBitmap = CreateDIBSection(NULL, (BITMAPINFO *) &bmih, 0,(VOID**) &pBits,NULL,0);

		int dw = bmih.biWidth % 4;

		int maxInd = (bmih.biWidth * bmih.biHeight) * 3 + bmih.biHeight * dw;

		for(int i = 0; i < maxInd; i++){
			pBits[i] = b.pBits[i];
		}
	}
}

void cdib::operator =(const cdib &b){
	if(b.hBitmap && b.pBits){
		bmih = b.bmih;

		pBits = NULL;

		DeleteObject(hBitmap);

		hBitmap = CreateDIBSection(NULL, (BITMAPINFO *) &bmih, 0,(VOID**) &pBits,NULL,0);
		
		int dw = bmih.biWidth % 4;

		int maxInd = (bmih.biWidth * bmih.biHeight) * 3 + bmih.biHeight * dw;

		for(int i = 0; i < maxInd; i++){
			pBits[i] = b.pBits[i];
		}
	}
}

cdib::cdib(UINT width, UINT height,BYTE red,BYTE green,BYTE blue){
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = width;
	bmih.biHeight = height;
	bmih.biPlanes = 1;
	bmih.biBitCount = 24;
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;

	hBitmap = CreateDIBSection(NULL, (BITMAPINFO *) &bmih, 0,(VOID**) &pBits,NULL,0);

	int dw = bmih.biWidth % 4;

	for(int x = 0; x < bmih.biWidth; x ++){
		for(int y = 0; y < bmih.biHeight; y ++){
			pBits[(x + y * bmih.biWidth) * 3 + y * dw] = red;
			pBits[(x + y * bmih.biWidth) * 3 + 1 + y * dw] = green;
			pBits[(x + y * bmih.biWidth) * 3 + 2 + y * dw] = blue;
		}
	}
}

cdib::cdib(UINT width, UINT height, BYTE red, BYTE green, BYTE blue, double alpha){
	alpha = max(min(1.0,alpha),0.0);

	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = width;
	bmih.biHeight = height;
	bmih.biPlanes = 1;
	bmih.biBitCount = 24;
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;

	hBitmap = CreateDIBSection(NULL, (BITMAPINFO *) &bmih, 0,(VOID**) &pBits,NULL,0);

	BYTE b = 0;

	int dw = bmih.biWidth % 4;

	for(int x = 0; x < bmih.biWidth; x ++){
		for(int y = 0; y < bmih.biHeight; y ++){
			b = (x / trSize) % 2 ? ((y/trSize) % 2 ? trColorD:trColorL) : ((y/trSize) % 2 ? trColorL:trColorD);
			pBits[(x + y * bmih.biWidth) * 3 + dw * y]		= BYTE(b * (1. - alpha) + blue * alpha);
			pBits[(x + y * bmih.biWidth) * 3 + 1 + dw * y]	= BYTE(b * (1. - alpha) + green * alpha);
			pBits[(x + y * bmih.biWidth) * 3 + 2 + dw * y]	= BYTE(b * (1. - alpha) + red * alpha);
		}
	}
}

cdib::cdib(UINT width, UINT height){
	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = width;
	bmih.biHeight = height;
	bmih.biPlanes = 1;
	bmih.biBitCount = 24;
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;

	hBitmap = CreateDIBSection(NULL, (BITMAPINFO *) &bmih, 0,(VOID**) &pBits,NULL,0);
	
	BYTE b = 0;

	int dw = bmih.biWidth % 4;

	for(int x = 0; x < bmih.biWidth; x ++){
		for(int y = 0; y < bmih.biHeight; y ++){
			b = (x / trSize) % 2 ? ((y/trSize) % 2 ? trColorD:trColorL) : ((y/trSize) % 2 ? trColorL:trColorD);
			pBits[(x + y * bmih.biWidth) * 3 + y * dw] = pBits[(x + y * bmih.biWidth) * 3 + 1 + y * dw] =	pBits[(x + y * bmih.biWidth) * 3 + 2 + y * dw] = b;
		}
	}
}

void cdib::SetWidth(UINT width,UINT height){
	DeleteObject(hBitmap);

	bmih.biSize = sizeof(BITMAPINFOHEADER);
	bmih.biWidth = width;
	bmih.biHeight = height;
	bmih.biPlanes = 1;
	bmih.biBitCount = 24;
	bmih.biCompression = BI_RGB;
	bmih.biSizeImage = 0;
	bmih.biXPelsPerMeter = 0;
	bmih.biYPelsPerMeter = 0;
	bmih.biClrUsed = 0;
	bmih.biClrImportant = 0;

	pBits = NULL;

	hBitmap = CreateDIBSection(NULL, (BITMAPINFO *) &bmih, 0,(VOID**) &pBits,NULL,0);
	
	BYTE b = 0;

	int dw = bmih.biWidth % 4;

	for(int x = 0; x < bmih.biWidth; x ++){
		for(int y = 0; y < bmih.biHeight; y ++){
			b = (x / trSize) % 2 ? ((y/trSize) % 2 ? trColorD:trColorL) : ((y/trSize) % 2 ? trColorL:trColorD);
			pBits[(x + y * bmih.biWidth) * 3 + y * dw] = pBits[(x + y * bmih.biWidth) * 3 + 1 + y * dw] =	pBits[(x + y * bmih.biWidth) * 3 + 2 + y * dw] = b;
		}
	}
}

void cdib::SetRGBA_A(float r, float g, float b){
	float dx = 1. / bmih.biWidth;

	int dw = bmih.biWidth % 4;

	BYTE bf;

	for(int x = 0; x < bmih.biWidth; x ++){
		for(int y = 0; y < bmih.biHeight; y ++){
			bf = (x / trSize) % 2 ? ((y/trSize) % 2 ? trColorD:trColorL) : ((y/trSize) % 2 ? trColorL:trColorD);
			pBits[(x + y * bmih.biWidth) * 3 + y * dw] = bf * (1.f - dx * (float)x) + b * dx * (float)x * 255;
			pBits[(x + y * bmih.biWidth) * 3 + 1 + y * dw] = bf * (1.f - dx * (float)x) + g * dx * (float)x * 255;
			pBits[(x + y * bmih.biWidth) * 3 + 2 + y * dw] = bf * (1.f - dx * (float)x) + r * dx * (float)x * 255;
		}
	}
}

void cdib::SetRGBA_R(float g, float b, float a){
	float dx = 1. / bmih.biWidth;

	int dw = bmih.biWidth % 4;

	BYTE bf;

	for(int x = 0; x < bmih.biWidth; x ++){
		for(int y = 0; y < bmih.biHeight; y ++){
			bf = (x / trSize) % 2 ? ((y/trSize) % 2 ? trColorD:trColorL) : ((y/trSize) % 2 ? trColorL:trColorD);
			pBits[(x + y * bmih.biWidth) * 3 + y * dw] = bf * (1.f - a) + b * a * 255;
			pBits[(x + y * bmih.biWidth) * 3 + 1 + y * dw] = bf * (1.f - a) + g * a * 255;
			pBits[(x + y * bmih.biWidth) * 3 + 2 + y * dw] = bf * (1.f - a) + a * dx * (float)x * 255;
		}
	}
}

void cdib::SetRGBA_G(float r, float b, float a){
	float dx = 1. / bmih.biWidth;

	int dw = bmih.biWidth % 4;

	BYTE bf;

	for(int x = 0; x < bmih.biWidth; x ++){
		for(int y = 0; y < bmih.biHeight; y ++){
			bf = (x / trSize) % 2 ? ((y/trSize) % 2 ? trColorD:trColorL) : ((y/trSize) % 2 ? trColorL:trColorD);
			pBits[(x + y * bmih.biWidth) * 3 + y * dw] = bf * (1.f - a) + b * a * 255;
			pBits[(x + y * bmih.biWidth) * 3 + 1 + y * dw] = bf * (1.f - a) + dx * (float)x * a * 255;
			pBits[(x + y * bmih.biWidth) * 3 + 2 + y * dw] = bf * (1.f - a) + a * r * 255;
		}
	}
}

void cdib::SetRGBA_B(float r, float g, float a){
	float dx = 1. / bmih.biWidth;

	int dw = bmih.biWidth % 4;

	BYTE bf;

	for(int x = 0; x < bmih.biWidth; x ++){
		for(int y = 0; y < bmih.biHeight; y ++){
			bf = (x / trSize) % 2 ? ((y/trSize) % 2 ? trColorD:trColorL) : ((y/trSize) % 2 ? trColorL:trColorD);
			pBits[(x + y * bmih.biWidth) * 3 + y * dw] = bf * (1.f - a) + dx * (float)x * a * 255;
			pBits[(x + y * bmih.biWidth) * 3 + 1 + y * dw] = bf * (1.f - a) + g * a * 255;
			pBits[(x + y * bmih.biWidth) * 3 + 2 + y * dw] = bf * (1.f - a) + a * r * 255;
		}
	}
}

void cdib::SetHSVA_H(float s, float v, float a){
	float dx = 360. / bmih.biWidth;

	int dw = bmih.biWidth % 4;

	BYTE bf;

	float r, g, b;

	r = g = b = 0;

	for(int x = 0; x < bmih.biWidth; x ++){
		for(int y = 0; y < bmih.biHeight; y ++){
			bf = (x / trSize) % 2 ? ((y/trSize) % 2 ? trColorD:trColorL) : ((y/trSize) % 2 ? trColorL:trColorD);
			HSV2RGB(dx * x, s, v, r, g, b);
			pBits[(x + y * bmih.biWidth) * 3 + y * dw] = bf * (1.f - a) + b * a * 255;
			pBits[(x + y * bmih.biWidth) * 3 + 1 + y * dw] = bf * (1.f - a) + g * a * 255;
			pBits[(x + y * bmih.biWidth) * 3 + 2 + y * dw] = bf * (1.f - a) + r * a * 255;
		}
	}
}

void cdib::SetHSVA_S(float h, float v, float a){
	float dx = 1. / bmih.biWidth;

	int dw = bmih.biWidth % 4;

	BYTE bf;

	float r, g, b;

	r = g = b = 0;

	for(int x = 0; x < bmih.biWidth; x ++){
		for(int y = 0; y < bmih.biHeight; y ++){
			bf = (x / trSize) % 2 ? ((y/trSize) % 2 ? trColorD:trColorL) : ((y/trSize) % 2 ? trColorL:trColorD);
			HSV2RGB(h, dx * x, v, r, g, b);
			pBits[(x + y * bmih.biWidth) * 3 + y * dw] = bf * (1.f - a) + b * a * 255;
			pBits[(x + y * bmih.biWidth) * 3 + 1 + y * dw] = bf * (1.f - a) + g * a * 255;
			pBits[(x + y * bmih.biWidth) * 3 + 2 + y * dw] = bf * (1.f - a) + r * a * 255;
		}
	}
}

void cdib::SetHSVA_V(float h, float s, float a){
	float dx = 1. / bmih.biWidth;

	int dw = bmih.biWidth % 4;

	BYTE bf;

	float r, g, b;

	r = g = b = 0;

	for(int x = 0; x < bmih.biWidth; x ++){
		for(int y = 0; y < bmih.biHeight; y ++){
			bf = (x / trSize) % 2 ? ((y/trSize) % 2 ? trColorD:trColorL) : ((y/trSize) % 2 ? trColorL:trColorD);
			HSV2RGB(h, s, dx * x, r, g, b);
			pBits[(x + y * bmih.biWidth) * 3 + y * dw] = bf * (1.f - a) + b * a * 255;
			pBits[(x + y * bmih.biWidth) * 3 + 1 + y * dw] = bf * (1.f - a) + g * a * 255;
			pBits[(x + y * bmih.biWidth) * 3 + 2 + y * dw] = bf * (1.f - a) + r * a * 255;
		}
	}
}

void cdib::SetRGBA_Color(float r, float g, float b, float a){
	int dw = bmih.biWidth % 4;

	BYTE bf;

	for(int x = 0; x < bmih.biWidth; x ++){
		for(int y = 0; y < bmih.biHeight; y ++){
			bf = (x / trSize) % 2 ? ((y/trSize) % 2 ? trColorD:trColorL) : ((y/trSize) % 2 ? trColorL:trColorD);
			pBits[(x + y * bmih.biWidth) * 3 + y * dw] = bf * (1.f - a) + b * a * 255.f;
			pBits[(x + y * bmih.biWidth) * 3 + 1 + y * dw] = bf * (1.f - a) + g * a * 255.f;
			pBits[(x + y * bmih.biWidth) * 3 + 2 + y * dw] = bf * (1.f - a) + r * a * 255.f;
		}
	}
}

cdib::~cdib(){
	DeleteObject(hBitmap);
	this->pBits = NULL;
}

void cdib::BitBltB(HDC &hdc,int x,int y){
	HDC mem = CreateCompatibleDC(hdc);
	SelectObject(mem, hBitmap);
	BitBlt(hdc,x,y,bmih.biWidth, bmih.biHeight,mem, 0,0,SRCCOPY);
	DeleteObject(mem);
}

void cdib::StretchBltB(HDC &hdc, int x, int y, UINT width, UINT height){
	HDC mem = CreateCompatibleDC(hdc);
	SelectObject(mem, hBitmap);
	SetStretchBltMode(mem, COLORONCOLOR);
	StretchBlt(hdc,x,y,width,height,mem, 0,0,bmih.biWidth, bmih.biHeight,SRCCOPY);
	DeleteObject(mem);
}