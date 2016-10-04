#ifndef FunkcjeSprawdzajace_h
#define FunkcjeSprawdzajace_h

#include <windows.h>
#include <math.h>

#include <string>

class SprFu{
public:
	SprFu(void);
	bool LiczbaPierwsza(int liczba);
	bool PtInCircle(POINT CentralPoint,double Ray,POINT pt);
	bool PtInCircle(int xC,int yC,double Ray,POINT pt);
	bool PtInRay(int xC,int yC,UINT Ray,int dRay,POINT pt);
	static bool IsNumeric(std::string text);
};
#endif