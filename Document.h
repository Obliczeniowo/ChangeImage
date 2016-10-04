#ifndef DOCUMENT_H
#define DOCUMENT_H

#define WINVER 0x0500
#define _WIN32_WINNT 0x0500   // for Mouse Wheel support
#include <windows.h>
#include <math.h>
#include <vector>
#include <atlimage.h>
#include <atlwin.h>
#include <fstream>
#include <io.h>
#include <shlobj.h>

#include "resource.h"

#include "Drawing.h"

#include "myControls.h"

#include "dlgOpenSaveFileName.h"

#include "Text.h"
#include "myImage.h"

#define GR_IND 10

#define RULER_HEIGHT	17

#pragma region Obs³uga kontrolki vRuler (vertical ruler - pionowa linijka - linia³)

LRESULT CALLBACK vRulerProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);

class vRulerDataClass{
	HBITMAP	hBmp;
	HFONT	font;
	double	vResolution;
	int		y;
	int		height;
	RECT	WndSize;
	double	scale;
	enum unit	sUnit;
	void	WmPaintScale(HDC mem,Drawing *DRAW);
public:
	vRulerDataClass(HWND hWnd,enum unit sUnit);
	~vRulerDataClass();
	// W³aœciwoœci
	double	VResolution();
	void	VResolution(double vResolution);
	int		Y();
	void	Y(int y);
	int		Height();
	void	Height(int height);
	double	Scale();
	void	Scale(double scale);
	enum unit	SUnit();
	void		SUnit(enum unit sUnit,double vResolution);
	// Komunikaty
	void	WmPaint(HDC hdc,Drawing *DRAW);
	void	WmSize(HWND hWnd);
};

class vRulerClass{
	HWND hVRuler;
public:
	vRulerClass();
	~vRulerClass();
	bool	InitControl(HWND hParent);
	bool	CreateVRulerWindow(HWND hParent,int xPos,int yPos,int wndWidth,int wndHeight,int id,double vResolution,enum unit sUnit,int y,int height);
	void	DestroyVRulerWindow();
	void	SetRulerSize(int width,int height);
	vRulerDataClass*	RClass();
	void	Invalidate();
	enum unit	SUnit();
	void		SUnit(enum unit sUnit,double vResolution);
};

#pragma endregion

#pragma region Obs³uga kontrolki hRuler (horizontal ruler - pozioma linijka - linia³)

LRESULT CALLBACK hRulerProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);

class hRulerDataClass{
	HBITMAP	hBmp;
	HFONT	font;
	double	hResolution;
	int		x;
	int		width;
	RECT	WndSize;
	double	scale;
	enum unit	sUnit;
	void	WmPaintScale(HDC mem,Drawing *DRAW);
public:
	hRulerDataClass(HWND hWnd,enum unit sUnit);
	~hRulerDataClass();
	// W³aœciwoœci
	double	HResolution();
	void	HResolution(double hResolution);
	int		X();
	void	X(int x);
	int		Width();
	void	Width(int width);
	double	Scale();
	void	Scale(double scale);
	enum unit	SUnit();
	void		SUnit(enum unit sUnit,double hResolution);
	// Komunikaty
	void	WmPaint(HDC hdc,Drawing *DRAW);
	void	WmSize(HWND hWnd);
};

class hRulerClass{
	HWND hHRuler;
public:
	hRulerClass();
	~hRulerClass();
	bool	InitControl(HWND hParent);
	bool	CreateHRulerWindow(HWND hParent,int xPos,int yPos,int wndWidth,int wndHeight,int id,double hResolution,enum unit sUnit,int x,int width);
	void	DestroyHRulerWindow();
	void	SetRulerSize(int width,int height);
	enum unit	SUnit();
	void		SUnit(enum unit sUnit,double hResolution);
	hRulerDataClass*	RClass();
	void	Invalidate();
};

#pragma endregion

#pragma region Obs³uga kontrolki ViewImage

#define VI_SETIMAGEDATA		60001

LRESULT CALLBACK ViewImageProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);

bool InitViewImageClass(HINSTANCE hInstance);

class ImageData{
private:
	BITMAP	bmp;
public:
	HBITMAP	image;
	BYTE	*orginalByte;
	BYTE	*copyByte;
	ImageData(HBITMAP OrginalImage);
	~ImageData();
	// W³aœciwoœci
	int		Height();
	int		Width();
	int		WidthBytes();
	int		BitsPixel();
	// Metody
	void	CopyBitsFromOrginal();
	void	CopyBitsToOrginal();
};

class ViewImageData
{
public:
	enum viDrawingStyle{
		NoStyle		= 0,
		DrawPoint	= 1,
		DrawCircle	= 2,
		DrawEllipse	= 3
	};

	HBITMAP	hBmp;
	RECT	wndSize;
	double xScale;
	double yScale;
	enum viDrawingStyle	drawingStyle;
	int hRay;
	int vRay;
	POINT ptOnBitmap;
	POINT ptOnScreen;
	ImageData *imData;
	ViewImageData(HWND hWnd);
	~ViewImageData();
	// W³aœciwoœci
	void	ImData(ImageData *imData);
	enum viDrawingStyle	DrawingStyle();
	void	DrawingStyle(ViewImageData::viDrawingStyle drawingStyle);
	void	PtOnScreen(POINT point);
	void	PtOnBitmap(POINT pt);
	POINT	PtOnBitmap();
	// Komunikaty
	void	WmSize(HWND hWnd);
	void	WmPaint(HDC hdc,Drawing *DRAW);
	bool	WmLButtonDown(POINT MousePos);
	void	CopyBitsFromOrginal();
	void	CopyBitsToOrginal();
};

#pragma endregion

LRESULT CALLBACK ImageWindowProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);

LRESULT CALLBACK mdiChildProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);

LRESULT CALLBACK WindowGrPoints(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);

LRESULT CALLBACK WndScProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#define ID_SC_LEFT_COLOR	5000
#define ID_SC_RIGHT_COLOR	5001
#define ID_SC_CHAIN			5002
#define ID_SC_RED			5003
#define ID_SC_GREEN			5004
#define ID_SC_BLUE			5005
#define ID_SC_ALPHA			5006
#define ID_SC_HUE			5007
#define ID_SC_SATURATION	5008
#define ID_SC_VALUE			5009
#define ID_SC_HEX			5010

class Document
{
	void InitWindowPos(HWND hWnd, std::string fileName, HINSTANCE hInstance);
public:

	class scWindows{
	public:
		scWindows();

		int scInd;

		POINT wndPos;
		UINT width;
		UINT height;

		sColor left;
		sColor right;

		cdib cleft;
		cdib cright;

		cdib gred;
		cdib ggreen;
		cdib gblue;
		cdib galpha;
		cdib ghue;
		cdib gsaturation;
		cdib gvalue;

		HBITMAP hChainClose;
		HBITMAP hChainOpen;

		HWND scWnd;
		HWND scRightColor;
		HWND scLeftColor;
		HWND scChainButton;

		HWND scHex;

		NumericControl scRed;
		NumericControl scGreen;
		NumericControl scBlue;
		NumericControl scAlpha;
		NumericControl scHue;
		NumericControl scSaturation;
		NumericControl scValue;

		void Create(POINT pt, Document* doc);
		void Destroy();
		void SetControlsValue(float r, float g, float b, float a);
	};

	scWindows scWnd;

	void CreateScWnd();
	void RefreshGradient();

	RECT wndImSize;
	UINT ind;			// liczba punktów

	UINT mode;

	int selPt;
	int lr;

	std::vector<grPoint> grPtTable;

	NumericControl nc;

	HWND imWindow;
	HWND grPtWindow;

	HWND btWindow;
	HWND btSave;
	HWND btHand;		// ID 5006
	HWND btLineMode;	// ID 5007
	HWND btCircleMode;	// ID 5009
	HWND btPointMode;	// ID 5008

	bool btHandClicked;
	bool isPtOnLine;
	ULONG ulScrollLines;
	int iDeltaPerLine;

	POINT	grPtMousePos;
	UINT	grPtMouseKey;
	bool	grPtMouseIn;
	int		grPtSel;
	int		grSgSel;

	POINT lmp;
	POINT fmp;
	POINT rp;

	vRulerClass vrc;
	hRulerClass hrc;

	myImage *im;

	HBITMAP ImageWindowBitmap;
	HBITMAP grPtBitmap;

	Document(HWND hWnd);
	Document(HWND hWnd,const char* docName);
	~Document();
	void WmSize(HWND hWnd);
	void WmMouseWheel(HWND hWnd,WPARAM wParam,LPARAM lParam);
	void WmLButtonDown(POINT mp);
	void WmLButtonUp(POINT mp, int mk);
	void WmDblLeftClk(int mk);
	void WmMouseMove(POINT mp, UINT MK);
	void Save(HWND hWnd);

	// dla grPtWindow
	void grPtWmPaint(HDC hdc);
	void grPtWmMouseMove(int x, int y, UINT MK);
	void grPtWmMouseLeave();
	void grPtWmLButtonDown();
	void grPtWmLButtonUp();
	void grPtWmRButtonDown();

	void Draw(HDC hdc, UINT MK);

	void DrawGradient();

	void CreateGradientByCircle(); // Tworzenie gradientu po okrêgu

	static void CreateGradientPngFile(HWND hWnd);
	static void CreateGradientPngFile(HWND hWnd, std::vector<std::string> &tPath);
	static void CreateGradientSvgFile(HWND hWnd);
	static void CreateGradientSvgFile(HWND hWnd, std::vector<std::string> &tPath);
	static void ExportGradientFromSvgToGgr(HWND hWnd);
	static void ExportGradientFromGgrToCss(HWND hWnd);
	static void ExportGradientFromGgrToCss(HWND hWnd, std::vector<std::string> &tPath);

	void SetInd();
	void SetInd(UINT ind);

	void SetColorsFromBitmap();
};

#endif
