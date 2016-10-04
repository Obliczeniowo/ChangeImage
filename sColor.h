#ifndef SCOLOR_H
#define SCOLOR_H

#include <windows.h>
#include <math.h>
#include <string>
#include <vector>

void HSV2RGB(float h, float s, float v, float &r, float &g, float &b);
void RGB2HSV(float &h, float &s, float &v, float r, float g, float b);

class val{
public:
	std::string name;
	std::string value;
	val(std::string vText);
};

class c_xml{
public:
	std::vector<c_xml> xmlT;
	std::string vName;
	std::vector<val> tVal;
	c_xml(std::vector<std::string> &t);

	void EraseRekordIfNameNotMatch(std::string name);
	void EraseRekordIfDontHaveChild();

	int FindValueIndex(std::string valueName);
};

class sColor
{
public:
	float r, g, b, a;	// color definition in proportional scale
	float pos;			// color pos

	inline sColor(void):r(0),g(0),b(0),a(0),pos(0){};
	inline sColor(const sColor &sc) : r(sc.r), g(sc.g), b(sc.b), a(sc.a), pos(sc.pos){};
	sColor(float r,float g, float b, float a,float pos):r(min(max(r,0),1.f)),g(min(max(g,0),1.f)),b(min(max(b,0),1.f)),a(min(max(a,0),1.f)),pos(min(max(pos,0),1.f)){};

	sColor(c_xml stopPoint);

	~sColor(void);

	void SetRGB(float r,float g, float b, float a,float pos);
	void SetHSV(float r,float g, float b, float a,float pos);

	bool operator == (sColor cs);

	std::string ChangeColorToHexStringRGBA();
	std::string ChangeColorToHexStringRGB();
	std::string ChangeColorToRGBAString();

	bool SetColorFromHexString(std::string hStr);

	std::string GetSvgString(int &id);
};

#endif