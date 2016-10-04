#ifndef BROWSER_H
#define BROWSER_H

#include <windows.h>
#include <commctrl.h>
#include <io.h>
#include <shlobj.h>
#include <TIME.H>
#include <vector>
#include <algorithm>

#include "document.h"
#include "resource.h"

#define lpszAppName "Przegl¹darka gradientów (Gradient browser)"
#define GRADIENT_BROWSER_LIST "gradient browser list"

class GrItem{
public:
	bool selected;
	//gradient gr;
	cdib pr;
	std::string path;
	std::string file_name;
	std::string insert_name;

	void operator = (const GrItem &gi);

	inline GrItem(const GrItem &gri):pr(gri.pr),selected(false),path(gri.path),file_name(gri.file_name),insert_name(gri.insert_name){};

	GrItem(std::string grpath, std::string file_name,std::string insert_name, cdib pr);

	void GrDraw(HDC hdc, int x,int &dy, RECT &size);
};


LRESULT CALLBACK BrowserProc(HWND hWnd, UINT uMsg, WPARAM wParam,LPARAM lParam);

LRESULT CALLBACK GradientBrowserProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif