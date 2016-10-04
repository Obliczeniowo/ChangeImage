#ifndef SEGMENT_H
#define SEGMENT_H

#include <windows.h>
#include <vector>
#include <string>

#include "sColor.h"

POINT operator	+	(POINT fp, POINT lp);
void operator	*=	(POINT &p, double k);
POINT operator	*	(POINT p, double k);
POINT operator	-	(POINT fp, POINT lp);
double operator	*	(POINT fp, POINT lp);
bool operator	!=	(POINT &lp, POINT &rp);

POINT GetPointOnLine(POINT fp, POINT lp, POINT rp);

#define C_SYS_RGB	0
#define C_SYS_HSV	1
#define C_SYS_HSV2	2

class segment
{
public:
	sColor left;
	sColor right;
	sColor center;
	int c_sys;

	segment(void);
	segment(std::string s);
	inline segment(const segment &segm):
		left(segm.left),
		right(segm.right),
		center(segm.center),
		c_sys(segm.c_sys){};

	segment(sColor l, sColor r);
	~segment(void);

	bool CheckColorPos(segment *segmentBefore);

	void CreateGradientVector(float &pos, float &dk, std::vector<BYTE>::iterator &c, std::vector<float>::iterator &a);

	POINT SegmentOnScreen(POINT fp,POINT dp);
	int SegmentOnScreen(UINT width);

	std::string SegmentToString(bool last);

	void SetLeftColorRGB(float r, float g, float b, float a);
	void SetRightColorRGB(float r, float g, float b, float a);
	void GetLeftColorRGB(float &r, float &g, float &b, float &a);
	void GetRightColorRGB(float &r, float &g, float &b, float &a);

	void SetRightPos(float pos);
	void SetLeftPos(float pos);
	void SetRightAndLeftPos(float left, float right);
};

#endif