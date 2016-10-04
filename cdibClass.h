#ifndef CDLIB_CLASS
#define CDLIB_CLASS

#include <windows.h>

#include "sColor.h"

#define trColorD 102
#define trColorL 153

#define trSize 5

class cdib{
public:
	HBITMAP hBitmap;
	BITMAPINFOHEADER bmih;
	BYTE *pBits;

	void operator = (const cdib &b);

	cdib(UINT width, UINT height,BYTE red, BYTE green, BYTE blue);
	cdib(UINT width, UINT height,BYTE red, BYTE green, BYTE blue, double alpha);
	cdib(UINT width,UINT height);
	cdib(const cdib &b);

	void SetWidth(UINT width, UINT HEIGHT);
	void SetRGBA_R(float g, float b, float a);
	void SetRGBA_G(float r, float b, float a);
	void SetRGBA_B(float r, float g, float a);
	void SetRGBA_A(float r, float g, float b);

	void SetHSVA_H(float s, float v, float a);
	void SetHSVA_S(float h, float v, float a);
	void SetHSVA_V(float h, float s, float a);

	void SetRGBA_Color(float r, float g, float b, float a);

	~cdib();

	void BitBltB(HDC &hdc,int x,int y);
	void StretchBltB(HDC &hdc, int x,int y,UINT width, UINT height);
};

#endif