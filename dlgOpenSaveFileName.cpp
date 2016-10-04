#include "dlgOpenSaveFileName.h"

//Konstruktory klasy
dlgOpenSaveFileName::dlgOpenSaveFileName(HINSTANCE hInstance,LPSTR szFilter)
{
	this->szFilter=NULL;
	SetFilter(szFilter);
	szFileName[0]=NULL;
	szTitleName[0]=NULL;
	ofn.lStructSize       = sizeof (OPENFILENAME);
	ofn.hwndOwner         = NULL;
	ofn.hInstance         = hInstance;
	ofn.lpstrCustomFilter = NULL;
	ofn.nFilterIndex      = 0;
	ofn.lpstrFile         = szFileName;
	ofn.nMaxFile          = MAX_PATH;
	ofn.lpstrFileTitle    = szTitleName;
	ofn.nMaxFileTitle     = MAX_PATH;
	ofn.lpstrInitialDir   = NULL;
	ofn.lpstrTitle        = NULL;
	ofn.Flags             = 0;
	ofn.nFileOffset       = 0;
	ofn.nFileExtension    = 0;
	ofn.lpstrDefExt       = TEXT ("dn");
	ofn.lCustData         = 0;
	ofn.lpfnHook          = NULL;
	ofn.lpTemplateName    = NULL;
}
//Destruktor
dlgOpenSaveFileName::~dlgOpenSaveFileName(){
	if(szFilter)
		delete[] szFilter;
}
//Funkcje pobieraj¹ce
LPSTR dlgOpenSaveFileName::GetFileName(){
	return szFileName;
}
LPSTR dlgOpenSaveFileName::GetTitleName(){
	return szTitleName;
}

int dlgOpenSaveFileName::GetIndex(){
	return ofn.nFilterIndex;
}
//Funkcje ustawiaj¹ce
void dlgOpenSaveFileName::SetFilter(LPSTR szFilter){
	if(this->szFilter)
		delete[] this->szFilter;
	if(szFilter){
		this->szFilter=new char[strlen(szFilter)+1];
		strcpy_s(this->szFilter,strlen(szFilter)+1,szFilter);
		ofn.lpstrFilter = szFilter;
		ofn.nMaxCustFilter = DWORD(strlen(szFilter)+1);
	}else{
		ofn.nMaxCustFilter=0;
	}
}
//Funkcje wywo³uj¹ce okno dialogowe
bool dlgOpenSaveFileName::ShowOpenFileName(){
	return GetOpenFileName(&ofn) ? true : false;
}
bool dlgOpenSaveFileName::ShowOpenFileName(HWND hWnd){
	ofn.hwndOwner=hWnd;
	return GetOpenFileName(&ofn) ? true : false;
}
bool dlgOpenSaveFileName::ShowSaveFileName(){
	return GetSaveFileName(&ofn) ? true : false;
}
bool dlgOpenSaveFileName::ShowSaveFileName(HWND hWnd){
	ofn.hwndOwner=hWnd;
	return GetSaveFileName(&ofn) ? true : false;
}