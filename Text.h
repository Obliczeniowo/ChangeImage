#ifndef TEXT_H
#define TEXT_H

#include <windows.h>
#include <stdio.h>

class Text
{
	LPSTR text;
	bool destroy;
public:
	//Konstruktory klasy
	Text(void);
	Text(LPSTR text,bool destroy);
	//Destruktor klasy
	~Text(void);
	//Funkcje ustawiaj¹ce
	void SetText(LPSTR text);
	void SetText(double number,int nrOfSigns);
	void SetText(int number,int system);
	void SetStartupPath();
	//Operacje na tekœcie
	void AddTextToEnd(LPSTR text);
	void AddTextToEnd(Text &myText);
	void AddTextToBeginning(LPSTR text);
	void AddTextToBeginning(Text &myText);
	void Right(int nrOfSigns);
	void Left(int nrOfSigns);
	void CutText(int nrOfFirstSign,int nrOfSigns);
	void ShowMessage(HWND hWnd,LPSTR title,int style);
	//Funkcje sprawdzaj¹ce
	bool IsEqual(LPSTR text);
	bool IsEqual(Text myText);
	//Funkcje pobieraj¹ce
	int GetLenght();
	LPSTR GetText();
};
#endif