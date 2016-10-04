#ifndef GRADIENT_H
#define GRADIENT_H

#include "segment.h"
#include "dlgOpenSaveFileName.h"
#include "cdibClass.h"

#include <fstream>
#include <math.h>

#include "myMath.h"

#define HEIGHT	30

#define WIDTH_1 (WIDTH - 1)

#define H_SIZE 4

std::string RemSign(std::string s, const char* sign);

class gradient
{
	UINT width;
public:
	void CreateGradientVector();
	void SetWidth(UINT width);

	std::string gname;
	std::vector<segment> tSegment;

	std::vector<BYTE> gTable;
	std::vector<float> gATable;

	cdib transparent;
	cdib gr;

	gradient(void);

	void operator = (const gradient &grad);

	inline gradient(const gradient &grad): 
		width(grad.width), 
		gTable(grad.gTable),
		gATable(grad.gATable),
		gname(grad.gname),
		transparent(grad.transparent),
		gr(grad.gr){};
	~gradient(void);

	void LoadFile(HINSTANCE hInst);
	void LoadFile(std::string path);

	void GetPointColor(int index, float &rl, float &gl, float &bl, float &al, float &rr, float &gr, float &br, float &ar);

	void SetGradient(std::vector<float> &v);

	void DrawGradientPt(HDC &hdc, POINT fp, POINT lp);
	void DrawGrPt(HDC &hdc, UINT width, UINT height,int grPtInd,int grSgInd);

	void AddPoint(float r, float g, float b, float a, float pos);
	void SetGradientColors(POINT fp, POINT dp, BYTE* table, int pitch);
	void SetGradientColors32(POINT fp, POINT dp, BYTE* table, int pitch);

	int MouseInPt(POINT mp,POINT fp, POINT dp, double scale);

	void FindGrPtAndSegmentIndex(int &grPtInd, int &grSgInd, int x,int width,int ray);

	void DeletePoint(int selPt);

	std::string GradientToString();
	std::string GetSvgGradientString(std::string idS);
	std::string GetSvgGradientDefinition(std::string idS);
	std::string GetSvgGradientObject(std::string idS, int idR, int x, int y, int height, int width);
	std::string GetHtmlGradientDef(std::string path);
	std::string GetCssGradientString();

	void GradientFromXml(c_xml &xml, int index);

	void MoveSegment(float dx, int index);
};

#endif