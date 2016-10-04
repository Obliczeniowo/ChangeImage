#pragma once

#ifndef MYIMAGE_H
#define MYIMAGE_H

#include <Windows.h>
#include <atlimage.h>
#include <gdiplus.h>
#include <string>
#include <vector>
#include <fstream>

#include "Drawing.h"

#include "FunkcjeSprawdzajace.h"
#include "gradient.h"

#define BTG_MIDDLE	0
#define BTG_RED		1
#define BTG_GREEN	2
#define	BTG_BLUE	3

#define BTM_LINE	1
#define BTM_POINT	2
#define BTM_CIRCLE	3

#define GetBColor(x,y) table[x * 3 + y * image.GetPitch()]
#define GetGColor(x,y) table[x * 3 + y * image.GetPitch() + 1]
#define GetRColor(x,y) table[x * 3 + y * image.GetPitch() + 2]

#pragma region Jednostki
enum unit{
	pixel,
	milimiter,
	inch
};
#pragma endregion

struct grPoint{
	POINT left;
	POINT right;

	grPoint(POINT left, POINT right);
	grPoint();

	int PtIn(POINT mp, int ray);
};

class myImage
{
protected:
	int		x;
	int		y;
	double	scale;
	double	vResolution;
	double	hResolution;
public:
	ATL::CImage	image;
	ATL::CImage image2;

	gradient gr;

	myImage(const char* imageName);
	int		X();
	int		Y();
	void	X(int x);
	void	Y(int y);
	double	Scale();
	void	Scale(double scale);
	void	Save(LPSTR pathAndName);
	void	bmpGrayCircleFilter(double rMax,double rMin,double xF,double yF);
	void	bmpGrayEllipseFilter(double aMax,double bMax,double aMin,double bMin,double xPos,double yPos);
	void	bmpToGray(int idType);
	int		bmpWidth();
	int		bmpHeight();
	int		Height();
	int		Width();
	int		nBmpWidth();
	int		nBmpHeight();
	HBITMAP	Image();
	double	xResolution(enum unit sunit);
	double	yResolution(enum unit sunit);
	void	Draw(HDC &hdc,RECT WndSize,POINT mp,POINT lmp,UINT mk,bool drawPoints,UINT mode);
	int		GetBPP();

	POINT ImageToWindow(int xp, int yp, RECT &WndSize);
	void WindowToImage(POINT &pt, RECT &WndSize);

	void CreateGradientFile();
	void CreateGr(POINT fp, POINT lp, UINT ind, RECT WndSize);
	void CreateGr(std::vector<grPoint> &grPtTable);
	void SetGradientColor(grPoint gp,int index);
	void NormalizePoint(POINT &pt);
	void AddPointToGradient(float r, float g, float b, float a, float pos);

	void SetGradientColors(POINT fp,POINT dp);
	void SetGradientColors(std::vector<grPoint> &grPtTable);
};
#endif