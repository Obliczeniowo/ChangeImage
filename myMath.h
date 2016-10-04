#ifndef myMath_h
#define myMath_h

#include <windows.h>

#define _USE_MATH_DEFINES

#include <math.h>

class myMath{
public:
	static double Odl(double x1,double y1,double x2,double y2);
	static double Odl(POINT p1,POINT p2);
	static double Odl3D(double x1,double y1,double z1,double x2,double y2,double z2);
	static double GetAngle(double x1,double y1,double x2,double y2);
	static double GetAngle(POINT p1,POINT p2);
	static double RayOfArc(POINT p1,POINT p2,POINT p3);
	static double RayOfArc(int x1,int y1,int x2,int y2,int x3,int y3);
	static double DegreesToRadians(double ktInDegrees);
	static double RadiansToDegrees(double ktInRadians);
	static double GetPI(void);
	static int Zaokr(double w);
	static double Zaokr(double w,UINT nrPoPrzecinku);
	static POINT RotatePoint(int xObracany,int yObracany,int xObrotu,int yObrotu,double ktObrotu);
	static POINT Bezier(double t, POINT firstPoint,POINT lastPoint,double firstAngle,double lastAngle, double firstRay, double lastRay);
	static POINT Bezier(double t, POINT firstPoint,POINT lastPoint,POINT first,POINT last);
};

void StartupFile(LPSTR &startupFile,LPSTR startupFileName);

void StartupPath(LPSTR &startupPath);

LPSTR Right(LPSTR text,UINT nr);

LPSTR Left(LPSTR text,UINT nr);

LPSTR WytnijTekst(LPSTR text,UINT nrPocz,UINT szer);

LPSTR ZastTekst(LPSTR text,LPSTR textZ,UINT nrPocz,UINT szer);

LPSTR FindX(LPSTR fu,double x);

int SprNaw(LPSTR fu);

void FindNaw(LPSTR fu,UINT &pocz,UINT &kon);

bool IsNumeric(LPSTR liczba);

double Liczba(LPSTR fu,UINT p);

double LiczFu(LPSTR fu,double x);
#endif