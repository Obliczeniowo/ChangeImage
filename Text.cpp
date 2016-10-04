#include "Text.h"

//Konstruktory klasy
Text::Text()
{
	text=NULL;
	destroy=true;
}
Text::Text(LPSTR text,bool destroy){
	this->text=NULL;
	SetText(text);
	this->destroy=destroy;
}
//Destruktor klasy
Text::~Text(void)
{
	if(text && destroy)
		delete[] text;
}
//Funkcje ustawiaj¹ce
void Text::SetText(LPSTR text){
	if(this->text)
		delete[] this->text;
	this->text=new char[strlen(text)+1];
	strcpy_s(this->text,sizeof(char)*(strlen(text)+1),text);
}
void Text::SetText(double number,int nrOfSigns){
	if(this->text)
		delete[] this->text;
	if(nrOfSigns<3)
		nrOfSigns=3;
	text=new char[nrOfSigns+10];
	_gcvt_s(text,sizeof(char)*(nrOfSigns+10),number,nrOfSigns);
	if(text[strlen(text)-1]=='.'){
		text[strlen(text)-1]=NULL;
	}
}
void Text::SetText(int number,int system){
	if(this->text)
		delete[] this->text;
	if(system<2)
		system=2;
	else if(system>16){
		system=16;
	}
	text=new char[15];
	ltoa(number,text,system);
}
void Text::SetStartupPath(){
	LPSTR tempStr=GetCommandLine();
	if(tempStr[0]=='"')
		tempStr++;
	char str[MAX_PATH];
	strcpy(str,tempStr);
	char *p=strrchr(str,'\\');
	if(p)
		*(p+1)=NULL;
	SetText(str);
}
void Text::ShowMessage(HWND hWnd,LPSTR title,int style){
	MessageBox(hWnd,text,title,style);
}
//Operacje na tekœcie
void Text::AddTextToEnd(LPSTR text){
	if(this->text){
		LPSTR p= new char[strlen(text)+GetLenght()+1];
		sprintf_s(p,sizeof(char)*(strlen(text)+GetLenght()+1),"%s%s",this->text,text);
		SetText(p);
		delete[] p;
	}else{
		SetText(text);
	}
}
void Text::AddTextToEnd(Text &myText){
	AddTextToEnd(myText.GetText());
}
void Text::AddTextToBeginning(LPSTR text){
	if(this->text){
		LPSTR p= new char[strlen(text)+GetLenght()+1];
		sprintf_s(p,sizeof(char)*(strlen(text)+GetLenght()+1),"%s%s",text,this->text);
		SetText(p);
		delete[] p;
	}else{
		SetText(text);
	}
}
void Text::AddTextToBeginning(Text &myText){
	AddTextToBeginning(myText.GetText());
}
void Text::Right(int nrOfSigns){
	if(text && nrOfSigns){
		if(nrOfSigns>=GetLenght()){
			delete[] text;
			text=NULL;
		}else{
			LPSTR p=new char[GetLenght()-nrOfSigns+1];
			text[GetLenght()-nrOfSigns]=NULL;
			strcpy(p,text);
			SetText(p);
			delete[] p;
		}
	}
}
void Text::Left(int nrOfSigns){
	if(text && nrOfSigns){
		if(nrOfSigns>=GetLenght()){
			delete[] text;
			text=NULL;
		}else{
			LPSTR p=new char[GetLenght()-nrOfSigns+1];
			char *tempPt=(text+nrOfSigns);
			strcpy(p,tempPt);
			SetText(p);
			delete[] p;
		}
	}
}
void Text::CutText(int nrOfFirstSign,int nrOfSigns){
	Left(nrOfFirstSign);
	Right(GetLenght()-nrOfSigns);
}
//Funkcje sprawdzj¹ce
bool Text::IsEqual(LPSTR text){
	return !strcmp(text,this->text);
}
bool Text::IsEqual(Text myText){
	return IsEqual(GetText());
}
//Funkcje pobieraj¹ce
int Text::GetLenght(){
	if(text){
		return (int)strlen(text);
	}
	return -1;
}
LPSTR Text::GetText(){
	if(!text){
		return 0;
	}else{
		LPSTR myText=text;
		return myText;
	}
}