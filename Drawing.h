#include <windows.h>
#include <math.h>

#include "myMath.h"

#ifndef Drawing_h
#define Drawing_h

enum rop2 : int{
	black =	1,		// Pixel is always 0. 
	notMergePen,	// Pixel is the inverse of the R2_MERGEPEN color.
	maskNotPen,		// Pixel is a combination of the colors common to both the screen and the inverse of the pen.
	notCopyPen,		// Pixel is the inverse of the pen color.
	maskPenNot,		// Pixel is a combination of the colors common to both the pen and the inverse of the screen. 
	not,			// Pixel is the inverse of the screen color. 
	xorPen,			// Pixel is a combination of the colors in the pen and in the screen, but not in both.  
	notMaskPen,		// Pixel is the inverse of the R2_MASKPEN color. 
	maskPen,		// Pixel is a combination of the colors common to both the pen and the screen. 
	notXorPen,		// Pixel is the inverse of the R2_XORPEN color.
	nop,			// Pixel remains unchanged. 
	mergeNotPen,	// Pixel is a combination of the screen color and the inverse of the pen color. 
	copyPen,		// Pixel is the pen color. 
	MergePenNot,	// Pixel is a combination of the pen color and the inverse of the screen color. 	 
	MergePen,		// Pixel is a combination of the pen color and the screen color. 
	white			// Pixel is always 1. 
};

RECT GetRect(int x,int y,UINT szer,UINT wys);

class Drawing : public myMath
{
	HPEN hPen;
	HBRUSH hBrush;
	HFONT hFont;
	LOGFONT lFont;

	void Pion(HDC &hdc,int dx,int dy,COLORREF kolor,double skala);
	void Poziom(HDC &hdc,int dx,int dy,COLORREF kolor,double skala);
public:
	//Konstruktor
	Drawing(void);
	~Drawing();
	//Rysowanie prostok¹ta:
		//z wype³nieniem i ramk¹
	void rectangle(HDC &hdc,RECT rect);
	void rectangle(HDC &hdc,int xLeft,int yTop,int xRight,int yBottom);
	void rectangle(HDC &hdc,int xLeft,int yTop,int xRight,int yBottom,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void rectangle(HDC &hdc,int x,int y,UINT szer,UINT wys);
	void rectangle(HDC &hdc,RECT rect,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void rectangle(HDC &hdc,int x,int y,UINT szer,UINT wys,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
		//z wype³nieniem i bez ramki
	void fillRectangle(HDC &hdc,int x,int y,UINT szer,UINT wys);
	void fillRectangle(HDC &hdc,RECT rect);
	void fillRectangle(HDC &hdc,int x1,int y1,int x2,int y2);
	void fillRectangle(HDC &hdc,int x,int y,UINT szer,UINT wys,COLORREF kBrush);
	void fillRectangle(HDC &hdc,int x1,int y1,int x2,int y2,COLORREF kBrush);
	void fillRectangle(HDC &hdc,RECT rect,COLORREF kBrush);
		//z ramk¹ i bez wype³nienia
	void frameRectangle(HDC &hdc,int x,int y,UINT szer,UINT wys);
	void frameRectangle(HDC &hdc,RECT rect);
	void frameRectangle(HDC &hdc,int x,int y,UINT szer,UINT wys,COLORREF kPen,int Styl,int Gr);
	void frameRectangle(HDC &hdc,RECT rect,COLORREF kPen,int Styl,int Gr);
	//Rysowanie okrêgu:
		//z ramk¹ i wype³nieniem
	void circle(HDC &hdc,int xS,int yS,UINT Ray);
	void circle(HDC &hdc,int xS,int yS,UINT Ray,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void circle(HDC &hdc,POINT CentralPoint,UINT ray);
	//Rysowanie okrêgu:
		//z wype³nieniem i bez ramki
	void fillCircle(HDC &hdc,int xS,int yS,UINT Ray);
	void fillCircle(HDC &hdc,int xS,int yS,UINT Ray,COLORREF kBrush);
		//z ramk¹ i bez wype³nienia
	void frameCircle(HDC &hdc,int xS,int yS,UINT Ray);
	void frameCircle(HDC &hdc,int xS,int yS,UINT Ray,COLORREF kPen,int Styl,int Gr);
	void frameCircle(HDC &hdc,POINT CentralPoint,UINT Ray);
	void frameCircle(HDC &hdc,POINT CentralPoint,UINT Ray,COLORREF kPen,int Styl,int Gr);
	//Rysowanie elipsy:
		//z ramk¹ i wype³nieniem
	void ellipse(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay);
	void ellipse(HDC &hdc,RECT rect);
	void ellipse(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void ellipse(HDC &hdc,RECT rect,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
		//bez ramki i z wype³nieniem
	void fillEllipse(HDC &hdc,RECT rect);
	void fillEllipse(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay);
	void fillEllipse(HDC &hdc,RECT rect,COLORREF kBrush);
	void fillEllipse(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,COLORREF kBrush);
		//z ramk¹ i bez wype³nienia
	void frameEllipse(HDC &hdc,RECT rect);
	void frameEllipse(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay);
	void frameEllipse(HDC &hdc,RECT rect,COLORREF kPen,int Styl,int Gr);
	void frameEllipse(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,COLORREF kPen,int Styl,int Gr);
	//Rysowanie prostok¹ta z zaokr¹glonymi naro¿nikami:
		//z wype³nieniem i ramk¹
	void roundRect(HDC &hdc,RECT rect,UINT Ray);
	void roundRect(HDC &hdc,RECT rect,UINT xRay,UINT yRay);
	void roundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT Ray);
	void roundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT xRay,UINT yRay);
	void roundRect(HDC &hdc,RECT rect,UINT Ray,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void roundRect(HDC &hdc,RECT rect,UINT xRay,UINT yRay,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void roundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT Ray,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void roundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT xRay,UINT yRay,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
		//bez ramki i z wype³nieniem
	void fillRoundRect(HDC &hdc,RECT rect,UINT Ray,COLORREF kBrush);
	void fillRoundRect(HDC &hdc,RECT rect,UINT xRay,UINT yRay,COLORREF kBrush);
	void fillRoundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT Ray,COLORREF kBrush);
	void fillRoundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT xRay,UINT yRay,COLORREF kBrush);
		//z ramk¹ i bez wype³nienia
	void frameRoundRect(HDC &hdc,RECT rect,UINT Ray,COLORREF kPen,int Styl,int Gr);
	void frameRoundRect(HDC &hdc,RECT rect,UINT xRay,UINT yRay,COLORREF kPen,int Styl,int Gr);
	void frameRoundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT Ray,COLORREF kPen,int Styl,int Gr);
	void frameRoundRect(HDC &hdc,int x,int y,UINT szer,UINT wys,UINT xRay,UINT yRay,COLORREF kPen,int Styl,int Gr);
	//Rysowanie linii
	void line(HDC &hdc,POINT p1,POINT p2);
	void line(HDC &hdc,int x1,int y1,int x2,int y2);
	void line(HDC &hdc,int X,int Y,int kt,UINT R);
	void line(HDC &hdc,int X,int Y,double kt,UINT R);
	void line(HDC &hdc,POINT p1,POINT p2,COLORREF kPen,int Styl,int Gr);
	void line(HDC &hdc,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void line(HDC &hdc,int X,int Y,int kt,UINT R,COLORREF kPen,int Styl,int Gr);
	void line(HDC &hdc,int X,int Y,double kt,UINT R,COLORREF kPen,int Styl,int Gr);
	void lineR(HDC &hdc,int x1,int y1,int x2,int y2,int xObrotu,int yObrotu,double kt);
	void MoveTo(HDC &hdc,int x,int y);
	void MoveTo(HDC &hdc,POINT pt);
	//Rysowanie ³uków
	void circleArc(HDC &hdc,int xS,int yS,UINT Ray,int x1,int y1,int x2,int y2);
	void circleArc(HDC &hdc,int xS,int yS,UINT Ray,int kt1,int kt2);
	void circleArc(HDC &hdc,int xS,int yS,UINT Ray,double kt1,double kt2);
	void circleArc(HDC &hdc,int xS,int yS,UINT Ray,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void circleArc(HDC &hdc,int xS,int yS,UINT Ray,int kt1,int kt2,COLORREF kPen,int Styl,int Gr);
	void circleArc(HDC &hdc,int xS,int yS,UINT Ray,double kt1,double kt2,COLORREF kPen,int Styl,int Gr);
	//Rysowanie ³uków eliptycznych
	void ellipseArc(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int x1,int y1,int x2,int y2);
	void ellipseArc(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int kt1,int kt2);
	void ellipseArc(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,double kt1,double kt2);
	void ellipseArc(HDC &hdc,RECT rect,int x1,int y1,int x2,int y2);
	void ellipseArc(HDC &hdc,RECT rect,int kt1,int kt2);
	void ellipseArc(HDC &hdc,RECT rect,double kt1,double kt2);
	void ellipseArc(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void ellipseArc(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int kt1,int kt2,COLORREF kPen,int Styl,int Gr);
	void ellipseArc(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,double kt1,double kt2,COLORREF kPen,int Styl,int Gr);
	void ellipseArc(HDC &hdc,RECT rect,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void ellipseArc(HDC &hdc,RECT rect,int kt1,int kt2,COLORREF kPen,int Styl,int Gr);
	void ellipseArc(HDC &hdc,RECT rect,double kt1,double kt2,COLORREF kPen,int Styl,int Gr);
	// Ko³o œciête ciêciw¹
	void circleChord(HDC &hdc,int xS,int yS,UINT Ray,int x1,int y1,int x2,int y2);
	void circleChord(HDC &hdc,int xS,int yS,UINT Ray,int kt1,int kt2);
	void circleChord(HDC &hdc,int xS,int yS,UINT Ray,double kt1,double kt2);
	void circleChord(HDC &hdc,int xS,int yS,UINT Ray,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void circleChord(HDC &hdc,int xS,int yS,UINT Ray,int kt1,int kt2,COLORREF kPen,int Styl,int Gr);
	void circleChord(HDC &hdc,int xS,int yS,UINT Ray,double kt1,double kt2,COLORREF kPen,int Styl,int Gr);
	// Elipsa œciêta ciêciw¹
	void ellipseChord(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int x1,int y1,int x2,int y2);
	void ellipseChord(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int kt1,int kt2);
	void ellipseChord(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,double kt1,double kt2);
	void ellipseChord(HDC &hdc,RECT rect,int x1,int y1,int x2,int y2);
	void ellipseChord(HDC &hdc,RECT rect,int kt1,int kt2);
	void ellipseChord(HDC &hdc,RECT rect,double kt1,double kt2);
	void ellipseChord(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void ellipseChord(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int kt1,int kt2,COLORREF kPen,int Styl,int Gr);
	void ellipseChord(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,double kt1,double kt2,COLORREF kPen,int Styl,int Gr);
	void ellipseChord(HDC &hdc,RECT rect,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void ellipseChord(HDC &hdc,RECT rect,int kt1,int kt2,COLORREF kPen,int Styl,int Gr);
	void ellipseChord(HDC &hdc,RECT rect,double kt1,double kt2,COLORREF kPen,int Styl,int Gr);
	// Wycinek ko³a
	void circlePie(HDC &hdc,int xS,int yS,UINT Ray,int x1,int y1,int x2,int y2);
	void circlePie(HDC &hdc,int xS,int yS,UINT Ray,int kt1,int kt2);
	void circlePie(HDC &hdc,int xS,int yS,UINT Ray,double kt1,double kt2);
	void circlePie(HDC &hdc,int xS,int yS,UINT Ray,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void circlePie(HDC &hdc,int xS,int yS,UINT Ray,int kt1,int kt2,COLORREF kPen,int Styl,int Gr);
	void circlePie(HDC &hdc,int xS,int yS,UINT Ray,double kt1,double kt2,COLORREF kPen,int Styl,int Gr);
	// Wycinek elipsy
	void ellipsePie(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int x1,int y1,int x2,int y2);
	void ellipsePie(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int kt1,int kt2);
	void ellipsePie(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,double kt1,double kt2);
	void ellipsePie(HDC &hdc,RECT rect,int x1,int y1,int x2,int y2);
	void ellipsePie(HDC &hdc,RECT rect,int kt1,int kt2);
	void ellipsePie(HDC &hdc,RECT rect,double kt1,double kt2);
	void ellipsePie(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void ellipsePie(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,int kt1,int kt2,COLORREF kPen,int Styl,int Gr);
	void ellipsePie(HDC &hdc,int xS,int yS,UINT xRay,UINT yRay,double kt1,double kt2,COLORREF kPen,int Styl,int Gr);
	void ellipsePie(HDC &hdc,RECT rect,int x1,int y1,int x2,int y2,COLORREF kPen,int Styl,int Gr);
	void ellipsePie(HDC &hdc,RECT rect,int kt1,int kt2,COLORREF kPen,int Styl,int Gr);
	void ellipsePie(HDC &hdc,RECT rect,double kt1,double kt2,COLORREF kPen,int Styl,int Gr);
	void polyline(HDC &hdc,CONST POINT* Punkty,int ilP);
	void polyline(HDC &hdc,CONST POINT* Punkty,int ilP,COLORREF kPen,int Styl,int Gr);
	void polygon(HDC &hdc,CONST POINT* Punkty,int ilP);
	void polygon(HDC &hdc,CONST POINT* Punkty,int ilP,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void framePolygon(HDC &hdc,CONST POINT* Punkty,int ilP);
	void framePolygon(HDC &hdc,CONST POINT* Punkty,int ilP,COLORREF kPen,int Styl,int Gr);
	void fillPolygon(HDC &hdc,CONST POINT* Punkty,int ilP,COLORREF kBrush);
	void fillPolygon(HDC &hdc,CONST POINT* Punkty,int ilP);
	void polyPolygon(HDC &hdc,CONST POINT* Punkty,CONST INT *ilP,int ilW);
	void polyPolygon(HDC &hdc,CONST POINT* Punkty,CONST INT *ilP,int ilW,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void wielForemny(HDC &hdc,int xS,int yS,UINT ilBokow,UINT R,int ktPrzes);
	void wielForemny(HDC &hdc,int xS,int yS,UINT ilBokow,UINT R,double ktPrzes);
	void wielForemny(HDC &hdc,int xS,int yS,UINT ilBokow,UINT R,int ktPrzes,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void wielForemny(HDC &hdc,int xS,int yS,UINT ilBokow,UINT R,double ktPrzes,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void Wektor(HDC &hdc,int X,int Y,double Kat,int dlWektora,bool Arrow);
	void Wektor(HDC &hdc,int x1,int y1,int x2,int y2,bool Arrow);
	void Wektor(HDC &hdc,int X,int Y,double Kat,int dlWektora,bool Arrow,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void Wektor(HDC &hdc,int x1,int y1,int x2,int y2,bool Arrow,COLORREF kPen,int Styl,int Gr,COLORREF kBrush);
	void frameWektor(HDC &hdc,int x1,int y1,int x2,int y2,bool Arrow);
	void frameWektor(HDC &hdc,int x1,int y1,int x2,int y2,bool Arrow,COLORREF kPen,int Styl,int Gr);
	// Wyœwietlanie tekstu ms-help://MS.VSCC.v90/MS.MSDNQTR.v90.en/gdi/fontext_9r77.htm
	// A. TextOut
	BOOL textOut(HDC &hdc,int x,int y,LPCSTR tekst,int dlTekstu);
	BOOL textOut(HDC &hdc,int x,int y,int liczba,int ilCyfr);
	BOOL textOut(HDC &hdc,int x,int y,double liczba,int ilCyfr);
	BOOL textOut(HDC &hdc,int x,int y,LPCSTR tekst,int dlTekstu,COLORREF kTekstu,int bkMode);
	BOOL textOut(HDC &hdc,int x,int y,int liczba,int ilCyfr,COLORREF kTekstu,int bkMode);
	BOOL textOut(HDC &hdc,int x,int y,double liczba,int ilCyfr,COLORREF kTekstu,int bkMode);
	// B. DrawText
	BOOL drawText(HDC &hdc,LPCSTR tekst,int dlTekstu,RECT *rect,UINT uFormat);
	BOOL drawText(HDC &hdc,LPCSTR tekst,int dlTekstu,int x,int y,UINT szer,UINT wys,UINT uFormat);
	BOOL drawText(HDC &hdc,int liczba,int ilCyfr,int x,int y,UINT szer,UINT wys,UINT uFormat);
	BOOL drawText(HDC &hdc,double liczba,int ilCyfr,int x,int y,UINT szer,UINT wys,UINT uFormat);
	// C. PolyTextOut
	BOOL polyTextOut(HDC &hdc,CONST POLYTEXT *pptxt,int cStrings);
	// D. ExtTextOut
	BOOL extTextOut(HDC &hdc,int X,int Y,UINT fuOptions,CONST RECT* lprc,LPCTSTR lpString,UINT cbCount,CONST INT* lpDx);
	// E. TabbedTextOut
	LONG TabbedTextOutC(HDC &hdc,int X,int Y,LPCTSTR lpString,int nCount,int nTabPositions,CONST LPINT lpnTabStopPositions,int nTabOrigin);

	void CyfryWysEl(HBITMAP &hBitmap,UINT nrLiczby/*0-11 (0 - zero, 1 - jeden .... 10 - -*/,COLORREF kTla,COLORREF On,COLORREF Off,double skala);
	// Operacje na bitmapach
		// Rysowanie bitmapy na kontekœcie urz¹dzenia
	void bitBltBmpOnHdc(HDC &hdc,HBITMAP hBmp,int x,int y,UINT szer,UINT wys,int ropStyle);
	void bitBltBmpOnHdc(HDC &hdc,HBITMAP hBmp,int x,int y,int ropStyle);
	void bitBltBmpOnHdc(HDC &hdc,HBITMAP hBmp,int x,int y,UINT szer,UINT wys,int xPocz,int yPocz,int ropStyle);
	void stretchBltBmpOnHdc(HDC &hdc,HBITMAP hBmp,int x,int y,UINT szer,UINT wys,int ropStyle);
	void stretchBltBmpOnHdc(HDC &hdc,HBITMAP hBmp,int x,int y,UINT szer,UINT wys,int bmpX,int bmpY,int bmpWidth,int BmpHeight,int ropStyle);
	void MaskBmp(HDC hdc,int x,int y,int szer,int wys,HBITMAP Image,HBITMAP Mask);
	void MaskBmp(HDC hdc,int x,int y,int szer,int wys,int xPocz,int yPocz,HBITMAP Image,HBITMAP Mask);
	void StretchMaskBmp(HDC hdc,int x,int y,int szer,int wys,HBITMAP Image,HBITMAP Mask);
		// Tworzenie maski bitmapy
	HBITMAP CreateMask(HBITMAP hBmp,COLORREF kMaski);
	// Funkcje ustawiaj¹ce
		// Wype³nienie
	void SetBrush(HDC &hdc,COLORREF kBrush);
	void SetBrush(HDC &hdc,HBRUSH _hBrush);
	void SetBrush(HDC &hdc,UINT lbStyle,COLORREF lbColor,LONG lbHatch);
	void SetBrushIndirectC(HDC &hdc,LOGBRUSH &lg);
	// Pêdzel
	void SetPen(HDC &hdc,COLORREF kPen,int Styl,int Gr);
	void SetPen(HDC &hdc,HPEN _hPen);
		// Inne
	static int SetRop2Style(HDC &hdc,int ropStyle);
	static int GetRop2Style(HDC &hdc);
	static int SetGraphicsModeC(HDC &hdc,int iMode);
	static int GetGraphicsModeC(HDC &hdc);
	static COLORREF SetTextColorC(HDC &hdc,COLORREF color);
	static COLORREF GetTextColorC(HDC &hdc);
	static int SetBkModeC(HDC &hdc,int bkMode);
	static UINT SetTextAlignC(HDC &hdc,UINT fMode);
	static UINT GetTextAlignC(HDC &hdc);
	static BOOL SetTextJustificationC(HDC &hdc,int nBreakExtra,int nBreakCount);
	static int GetTextFaceC(HDC &hdc,int nCount,LPTSTR lpFaceName);
	static BOOL SetWorldTransformC(HDC &hdc,CONST XFORM *lpXform);
	static BOOL GetWorldTransformC(HDC &hdc,LPXFORM lpXform);
	static HGDIOBJ GetCurrentObjectC(HDC &hdc,UINT uObjectType);
	
	void SelectFont(HDC &hdc);
	void SetFontSize(LONG FontSize);
	void SetFontItalic(bool italic);
	void SetFontUnderline(bool underline);
	void SetFontAngle(double angle,bool radiansDegrees);

	static BOOL ModifyWorldTransformC(HDC &hdc,CONST XFORM *lpXform,DWORD iMode);
	// Funkcje obs³ugi bitmap
	PBITMAPINFO CreateBitmapInfoStruct(HWND hwnd,HBITMAP hBmp);
	void CreateBMPFile(HWND hwnd, LPTSTR pszFile, PBITMAPINFO pbi,HBITMAP hBMP, HDC hDC);
	HBITMAP GetBitmapFile(PTSTR szFileName);
	// Funkcje rysuj¹ce ramki
	void frame(HDC &hdc,RECT fr);
	void frame(HDC &hdc,int xLeft,int yTop,int xRight,int yBottom);
};
#endif