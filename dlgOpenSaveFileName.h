#ifndef DLGOPENSAVEFILENAME_H
#define DLGOPENSAVEFILENAME_H

#include <windows.h>

class dlgOpenSaveFileName
{
	OPENFILENAME ofn;
	LPSTR szFilter;
	char szFileName[MAX_PATH];
	char szTitleName[MAX_PATH];
public:
	//Konstruktory klasy
	dlgOpenSaveFileName(HINSTANCE hInstance,LPSTR szFilter);
	//Destruktor klasy
	~dlgOpenSaveFileName();
	//Funkcje pobierające
	LPSTR GetFileName();
	LPSTR GetTitleName();
	//Funkcje ustawiające
	void SetFilter(LPSTR szFilter);
	//Funkcje wywołujące okno dialogowe
	bool ShowOpenFileName();
	bool ShowOpenFileName(HWND hWnd);
	bool ShowSaveFileName();
	bool ShowSaveFileName(HWND hWnd);

	int GetIndex();
};
#endif
