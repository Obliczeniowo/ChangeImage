#include "myImage.h"

#include "resource.h"
#include "dlgOpenSaveFileName.h"

extern Drawing *RYS;
extern HINSTANCE hInst;

//Konstruktory
myImage::myImage(const char* imageName)
{
	scale = 0.5;
	x = y = 0;
	image.Load(imageName);
	//this->CreateGrayScaleFrame(75,75,75,75);
	WCHAR *bufor = new WCHAR[strlen(imageName) + 1];
	MultiByteToWideChar(CP_ACP,MB_ERR_INVALID_CHARS,imageName,(int)strlen(imageName) + 1,bufor,(int)strlen(imageName) + 1);
	Gdiplus::Image im(bufor);
	if(image.GetBPP() < 24){
		image.Save("im.jpg",Gdiplus::ImageFormatJPEG);
		image.Load("im.jpg");
		DeleteFile("im.jpg");		
	}
	if(image.GetBPP() == 32){
		//ATL::CImage image2;
		image2.Create(image.GetWidth(), image.GetHeight(),24,0);
		//image.Create(image.GetWidth(), image.GetHeight(),24,0);
		BYTE *b2 = (BYTE*) image2.GetBits();
		BYTE *b1 = (BYTE*) image.GetBits();
		BYTE b = 0;
		double k;
		int o = 10;
		for(int x = 0; x < image.GetWidth(); x++){
			for(int y = 0; y < image.GetHeight(); y++){
				b = (x / o) % 2 ? ((y / o) % 2 ? trColorD:trColorL) : ((y / o) % 2 ? trColorL:trColorD);
				k = (double)b1[x * 4 + y * image.GetPitch() + 3] / 255.;
				b2[x * 3 + y * image2.GetPitch()] =		BYTE(b1[x * 4 + y * image.GetPitch()] * k + b * (1. - k));
				b2[x * 3 + y * image2.GetPitch() + 1] = BYTE(b1[x * 4 + y * image.GetPitch() + 1] * k + b * (1. - k));
				b2[x * 3 + y * image2.GetPitch() + 2] = BYTE(b1[x * 4 + y * image.GetPitch() + 2] * k + b * (1. - k));

			}
		}
	}
	vResolution = (double)im.GetVerticalResolution();
	hResolution = (double)im.GetHorizontalResolution();
}
//Koniec konstruktorów
//Funkcje ustawiaj¹ce
void myImage::bmpGrayCircleFilter(double rMax,double rMin,double xPos,double yPos){
	if(image.GetBPP() == 24){
		BYTE *byte = (BYTE*)image.GetBits();
		for(int y=0;y<image.GetHeight();y++){
			for(int x=0;x<image.GetWidth();x++){
				BYTE r = byte[y*image.GetPitch()+x*3];
				BYTE g = byte[y*image.GetPitch()+x*3+1];
				BYTE b = byte[y*image.GetPitch()+x*3+2];
				double sr	= (double)(r + g + b) / 3;
				double dist = pow(pow((double)(x - xPos),2.0) + pow((double)(y - yPos),2.0),0.5);
				if(dist > rMin){
					double k	= (dist - rMin) / (rMax - rMin);
					if(k < 1){
						byte[y*image.GetPitch()+x*3]	= BYTE(r - (r - sr) * k);// GetPitch() - zwraca iloœæ bitów pomiêdzy adresem pocz¹tkowym pierwszej linii  a adresem pocz¹tkowym linii nastêpnej
						byte[y*image.GetPitch()+x*3+1]	= BYTE(g - (g - sr) * k);
						byte[y*image.GetPitch()+x*3+2]	= BYTE(b - (b - sr) * k);
					}else{
						byte[y*image.GetPitch()+x*3]	= BYTE(sr);
						byte[y*image.GetPitch()+x*3+1]	= BYTE(sr);
						byte[y*image.GetPitch()+x*3+2]	= BYTE(sr);
					}
				}
			}
		}
	}
}
void myImage::bmpGrayEllipseFilter(double aMax,double bMax,double aMin,double bMin,double xPos,double yPos){
	if(image.GetBPP()==24){
		BYTE *byte = (BYTE*)image.GetBits();
		for(int y=0;y<image.GetHeight();y++){
			for(int x=0;x<image.GetWidth();x++){
				BYTE r		= byte[y*image.GetPitch()+x*3];
				BYTE g		= byte[y*image.GetPitch()+x*3+1];
				BYTE b		= byte[y*image.GetPitch()+x*3+2];
				double sr	= (double)(r + g + b) / 3;
				double k	= aMax / bMax;
				double aR	= pow(pow((double)(x - xPos),2.0) * k + pow((double)(y - yPos) / k,2.0),0.5);
				if(aR > aMin){
					double kg	= (aR - aMin) / (aMax - aMin);
					if(kg < 1){
						byte[y*image.GetPitch()+x*3]	= BYTE(r - (r - sr) * kg);
						byte[y*image.GetPitch()+x*3+1]	= BYTE(g - (g - sr) * kg);
						byte[y*image.GetPitch()+x*3+2]	= BYTE(b - (b - sr) * kg);
					}else{
						byte[y*image.GetPitch()+x*3]	= (BYTE)sr;
						byte[y*image.GetPitch()+x*3+1]	= (BYTE)sr;
						byte[y*image.GetPitch()+x*3+2]	= (BYTE)sr;
					}
				}
			}
		}
	}
}
void myImage::bmpToGray(int idType){
	if(image.GetBPP() == 24){
		switch(idType){
			case BTG_MIDDLE:
				{
					BYTE* byte = (BYTE*)image.GetBits();
					for(int x = 0; x<image.GetWidth();x++){
						for(int y = 0; y<image.GetHeight();y++){
							double middle = (double)(byte[y*image.GetPitch() + x*3] + byte[y*image.GetPitch() + x*3 +1] + byte[y*image.GetPitch() + x*3 + 2]) / 3;
							byte[y * image.GetPitch() + x * 3]		= (BYTE)middle;
							byte[y * image.GetPitch() + x * 3 + 1]	= (BYTE)middle;
							byte[y * image.GetPitch() + x * 3 + 2]	= (BYTE)middle;
						}
					}
				}
				break;
			case BTG_RED:
				{
					BYTE* byte = (BYTE*)image.GetBits();
					for(int x = 0; x<image.GetWidth();x++){
						for(int y = 0; y<image.GetHeight();y++){
							byte[y * image.GetPitch() + x * 3 + 1]	= byte[y * image.GetPitch() + x * 3];
							byte[y * image.GetPitch() + x * 3 + 2]	= byte[y * image.GetPitch() + x * 3];
						}
					}
				}
				break;
			case BTG_GREEN:
				{
					BYTE* byte = (BYTE*)image.GetBits();
					for(int x = 0; x<image.GetWidth();x++){
						for(int y = 0; y<image.GetHeight();y++){
							byte[y * image.GetPitch() + x * 3]	= byte[y * image.GetPitch() + x * 3 + 1];
							byte[y * image.GetPitch() + x * 3 + 2]	= byte[y * image.GetPitch() + x * 3 + 1];
						}
					}
				}
				break;
			case BTG_BLUE:
				{
					BYTE* byte = (BYTE*)image.GetBits();
					for(int x = 0; x<image.GetWidth();x++){
						for(int y = 0; y<image.GetHeight();y++){
							byte[y * image.GetPitch() + x * 3]	= byte[y * image.GetPitch() + x * 3 + 2];
							byte[y * image.GetPitch() + x * 3 + 1]	= byte[y * image.GetPitch() + x * 3 + 2];
						}
					}
				}
				break;
		}
	}
}
int myImage::bmpHeight(){
	return int(image.GetHeight() * scale);
}
int myImage::bmpWidth(){
	return int(image.GetWidth() * scale);
}
int myImage::Height(){
	return image.GetHeight();
}
int myImage::Width(){
	return image.GetWidth();
}
int myImage::nBmpHeight(){
	return image.GetHeight();
}
int myImage::nBmpWidth(){
	return image.GetWidth();
}
HBITMAP	myImage::Image(){
	return (HBITMAP)this->image;
}
void myImage::Save(LPSTR pathAndName){
	image.Save(pathAndName);
}
void myImage::Draw(HDC &hdc,RECT WndSize,POINT mp,POINT lmp,UINT mk,bool drawPoints,UINT mode){
	SetStretchBltMode(hdc, COLORONCOLOR);
	if(image.GetBPP() == 24){
		if(WndSize.right < bmpWidth() && WndSize.bottom < bmpHeight()){
			image.Draw(hdc,0,0,WndSize.right,WndSize.bottom,x,y,int(WndSize.right / scale),int(WndSize.bottom / scale));
		}else if(WndSize.right < bmpWidth()){
			image.Draw(hdc,0,y,WndSize.right,WndSize.bottom,x,0,int(WndSize.right / scale),int(WndSize.bottom / scale));
		}else if(WndSize.bottom < bmpHeight()){
			image.Draw(hdc,x,0,WndSize.right,WndSize.bottom,0,y,int(WndSize.right / scale),int(WndSize.bottom / scale));
		}else{
			image.Draw(hdc,x,y,WndSize.right,WndSize.bottom,0,0,int(WndSize.right / scale),int(WndSize.bottom / scale));
		}
	}else if(image.GetBPP() == 32){
		if(WndSize.right < bmpWidth() && WndSize.bottom < bmpHeight()){
			image2.Draw(hdc,0,0,WndSize.right,WndSize.bottom,x,y,int(WndSize.right / scale),int(WndSize.bottom / scale));
		}else if(WndSize.right < bmpWidth()){
			image2.Draw(hdc,0,y,WndSize.right,WndSize.bottom,x,0,int(WndSize.right / scale),int(WndSize.bottom / scale));
		}else if(WndSize.bottom < bmpHeight()){
			image2.Draw(hdc,x,0,WndSize.right,WndSize.bottom,0,y,int(WndSize.right / scale),int(WndSize.bottom / scale));
		}else{
			image2.Draw(hdc,x,y,WndSize.right,WndSize.bottom,0,0,int(WndSize.right / scale),int(WndSize.bottom / scale));
		}
	}

	switch(mode){
		case BTM_LINE:
			{
				mp = ImageToWindow(mp.x,mp.y,WndSize);
				lmp = ImageToWindow(lmp.x,lmp.y,WndSize);
				HBRUSH hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
				HPEN hp = CreatePen(PS_SOLID,1,RGB(255,255,255));
				SelectObject(hdc,hp);
				SetROP2(hdc,R2_NOT);
				SelectObject(hdc,hbr);
				MoveToEx(hdc,mp.x,mp.y,NULL);
				LineTo(hdc,lmp.x,lmp.y);
				if( drawPoints)
					gr.DrawGradientPt(hdc,lmp,mp);
				DeleteObject(hbr);
				DeleteObject(hp);
			}
			break;
		case BTM_POINT:
			{
			}
			break;
	}
}

POINT myImage::ImageToWindow(int xp,int yp, RECT &WndSize){
	if(WndSize.right < bmpWidth() && WndSize.bottom < bmpHeight()){
		xp -= x;
		yp -= y;
		xp *= scale;
		yp *= scale;
	}else if(WndSize.right < bmpWidth()){
		xp -= x;
		xp *= scale;
		yp *= scale;
		yp += y;
	}else if(WndSize.bottom < bmpHeight()){
		yp -= y;
		xp *= scale;
		yp *= scale;
		xp += x;
	}else{

		xp *= scale;
		yp *= scale;
		xp += x;
		yp += y;
	}
	POINT pt;
	pt.x = xp;
	pt.y = yp;
	return pt;
}

void myImage::WindowToImage(POINT &pt,RECT &WndSize){
	if(WndSize.right < bmpWidth() && WndSize.bottom < bmpHeight()){
		pt.x /= scale;
		pt.y /= scale;
		pt.x += x;
		pt.y += y;
	}else if(WndSize.right < bmpWidth()){
		pt.y -= y;
		pt.x /= scale;
		pt.y /= scale;
		pt.x += x;
	}else if(WndSize.bottom < bmpHeight()){
		pt.x -= x;
		pt.x /= scale;
		pt.y /= scale;
		pt.y += y;
	}else{
		pt.x -= x;
		pt.y -= y;
		pt.x /= scale;
		pt.y /= scale;
	}
}

int NrOfItems(std::string str){
	int index = 0;
	int nrOf = 0;
	while(index != std::string::npos){
		nrOf ++;
		index = (int) str.find_first_of(';',index ? index + 1:index);
	}
	return nrOf;
}

void myImage::NormalizePoint(POINT &pt){
	pt.x = min(max(pt.x, 0), image.GetWidth()-1);
	pt.y = min(max(pt.y, 0), image.GetHeight()-1);
}

void myImage::AddPointToGradient(float r, float g, float b, float a, float pos){
	this->gr.AddPoint( r, g, b, a, pos);
}

void myImage::SetGradientColor(grPoint grPt,int index){
	BYTE *table =  (BYTE*)this->image.GetBits();
	
	if(image.GetBPP() == 24){
		if(index == 0){
			gr.tSegment[0].SetLeftColorRGB(float(table[grPt.left.x * 3 + grPt.left.y * image.GetPitch() + 2])/255,
				  float(table[grPt.left.x * 3 + grPt.left.y * image.GetPitch() + 1])/255,
				  float(table[grPt.left.x * 3 + grPt.left.y * image.GetPitch()])/255,
				  1.f);
		}else if(index == gr.tSegment.size()){
			gr.tSegment[gr.tSegment.size() - 1].SetRightColorRGB(float(table[grPt.right.x * 3 + grPt.right.y * image.GetPitch() + 2])/255,
										float(table[grPt.right.x * 3 + grPt.right.y * image.GetPitch() + 1])/255,
										float(table[grPt.right.x * 3 + grPt.right.y * image.GetPitch()])/255,
										1.f);
		}else{
			gr.tSegment[index].SetLeftColorRGB(float(table[grPt.right.x * 3 + grPt.right.y * image.GetPitch() + 2])/255,
												  float(table[grPt.right.x * 3 + grPt.right.y * image.GetPitch() + 1])/255,
												  float(table[grPt.right.x * 3 + grPt.right.y * image.GetPitch()])/255,
												  1.f);
			gr.tSegment[index - 1].SetRightColorRGB(float(table[grPt.left.x * 3 + grPt.left.y * image.GetPitch() + 2])/255,
											  float(table[grPt.left.x * 3 + grPt.left.y * image.GetPitch() + 1])/255,
											  float(table[grPt.left.x * 3 + grPt.left.y * image.GetPitch()])/255,
											  1.f);
		}

	}else if(image.GetBPP() == 32){
		if(index == 0){
			gr.tSegment[0].SetLeftColorRGB(float(table[grPt.left.x * 4 + grPt.left.y * image.GetPitch() + 2])/255,
				  float(table[grPt.left.x * 4 + grPt.left.y * image.GetPitch() + 1])/255,
				  float(table[grPt.left.x * 4 + grPt.left.y * image.GetPitch()])/255,
				  float(table[grPt.left.x * 4 + grPt.left.y * image.GetPitch() + 3])/255);
		}else if(index == gr.tSegment.size()){
			gr.tSegment[gr.tSegment.size() - 1].SetRightColorRGB(float(table[grPt.right.x * 4 + grPt.right.y * image.GetPitch() + 2])/255,
										float(table[grPt.right.x * 4 + grPt.right.y * image.GetPitch() + 1])/255,
										float(table[grPt.right.x * 4 + grPt.right.y * image.GetPitch()])/255,
										float(table[grPt.right.x * 4 + grPt.right.y * image.GetPitch() + 3])/255);
		}else{

			gr.tSegment[index].SetLeftColorRGB(float(table[grPt.right.x * 4 + grPt.right.y * image.GetPitch() + 2])/255.f,
												  float(table[grPt.right.x * 4 + grPt.right.y * image.GetPitch() + 1])/255.f,
												  float(table[grPt.right.x * 4 + grPt.right.y * image.GetPitch()])/255.f,
												  float(table[grPt.right.x * 4 + grPt.right.y * image.GetPitch() + 3])/255.f);

			gr.tSegment[index - 1].SetRightColorRGB(float(table[grPt.left.x * 4 + grPt.left.y * image.GetPitch() + 2])/255.f,
											  float(table[grPt.left.x * 4 + grPt.left.y * image.GetPitch() + 1])/255.f,
											  float(table[grPt.left.x * 4 + grPt.left.y * image.GetPitch()])/255.f,
											  float(table[grPt.left.x * 4 + grPt.left.y * image.GetPitch() + 3])/255.f);
		}
	}
	gr.CreateGradientVector();
}

void myImage::SetGradientColors(std::vector<grPoint> &grPtTable){
	if(gr.tSegment.size() != grPtTable.size() - 1)
		return ;
	BYTE *table =  (BYTE*)this->image.GetBits();
	if(grPtTable.size() >= 2){
		if(image.GetBPP() == 24){
			gr.tSegment[0].SetLeftColorRGB(float(table[grPtTable[0].right.x * 3 + grPtTable[0].right.y * image.GetPitch() + 2])/255.f,
				float(table[grPtTable[0].right.x * 3 + grPtTable[0].right.y * image.GetPitch() + 1])/255.f,
				float(table[grPtTable[0].right.x * 3 + grPtTable[0].right.y * image.GetPitch()])/255.f, 1.f);
			for(std::vector<grPoint>::iterator i = grPtTable.begin() + 1; i < grPtTable.end() - 1; i++){
				gr.tSegment[int(i - grPtTable.begin()) - 1].SetRightColorRGB(float(table[i->left.x * 3 + i->left.y * image.GetPitch() + 2])/255.f,
					float(table[i->left.x * 3 + i->left.y * image.GetPitch() + 1])/255.f,
					float(table[i->left.x * 3 + i->left.y * image.GetPitch()])/255.f, 1.f);
				gr.tSegment[int(i - grPtTable.begin())].SetLeftColorRGB(float(table[i->right.x * 3 + i->right.y * image.GetPitch() + 2])/255.f,
					float(table[i->right.x * 3 + i->right.y * image.GetPitch() + 1])/255.f,
					float(table[i->right.x * 3 + i->right.y * image.GetPitch()])/255.f,1.f);
			}
			(gr.tSegment.end() -1)->SetRightColorRGB(float(table[grPtTable[grPtTable.size() - 1].left.x * 3 + grPtTable[grPtTable.size() - 1].left.y * image.GetPitch() + 2])/255.f,
				float(table[grPtTable[grPtTable.size() - 1].left.x * 3 + grPtTable[grPtTable.size() - 1].left.y * image.GetPitch() + 1])/255.f,
				float(table[grPtTable[grPtTable.size() - 1].left.x * 3 + grPtTable[grPtTable.size() - 1].left.y * image.GetPitch()])/255.f, 1.f);
		}else if(image.GetBPP() == 32){
			float a;
			gr.tSegment[0].SetLeftColorRGB(float(table[grPtTable[0].right.x * 4 + grPtTable[0].right.y * image.GetPitch() + 2])/255.f,
				float(table[grPtTable[0].right.x * 4 + grPtTable[0].right.y * image.GetPitch() + 1])/255.f,
				float(table[grPtTable[0].right.x * 4 + grPtTable[0].right.y * image.GetPitch()])/255.f, 
				float(table[grPtTable[0].right.x * 4 + grPtTable[0].right.y * image.GetPitch() + 3])/255.f);
			for(std::vector<grPoint>::iterator i = grPtTable.begin() + 1; i < grPtTable.end() - 1; i++){
				gr.tSegment[int(i - grPtTable.begin())-1].SetRightColorRGB(float(table[i->left.x * 4 + i->left.y * image.GetPitch() + 2])/255.f,
					float(table[i->left.x * 4 + i->left.y * image.GetPitch() + 1])/255.f,
					float(table[i->left.x * 4 + i->left.y * image.GetPitch()])/255.f,
					float(table[i->left.x * 4 + i->left.y * image.GetPitch() + 3]) / 255.f);
				gr.tSegment[int(i - grPtTable.begin())].SetLeftColorRGB(float(table[i->right.x * 4 + i->right.y * image.GetPitch() + 2])/255.f,
					float(table[i->right.x * 4 + i->right.y * image.GetPitch() + 1])/255.f,
					float(table[i->right.x * 4 + i->right.y * image.GetPitch()])/255.f,
					float(table[i->right.x * 4 + i->right.y * image.GetPitch() + 3]) / 255.f);
			}
			(gr.tSegment.end() -1)->SetRightColorRGB(float(table[grPtTable[grPtTable.size() - 1].left.x * 4 + grPtTable[grPtTable.size() - 1].left.y * image.GetPitch() + 2])/255.f,
				float(table[grPtTable[grPtTable.size() - 1].left.x * 4 + grPtTable[grPtTable.size() - 1].left.y * image.GetPitch() + 1])/255.f,
				float(table[grPtTable[grPtTable.size() - 1].left.x * 4 + grPtTable[grPtTable.size() - 1].left.y * image.GetPitch()])/255.f, 
				float(table[grPtTable[grPtTable.size() - 1].left.x * 4 + grPtTable[grPtTable.size() - 1].left.y * image.GetPitch() + 3])/255.f);
		}
		gr.CreateGradientVector();
	}
}

void myImage::CreateGr(std::vector<grPoint> &grPtTable){
	std::vector<sColor> sct;
	BYTE *table =  (BYTE*)this->image.GetBits();
	float r;
	float g;
	float b;
	float pos;
	float dx = 1.f / (grPtTable.size() - 1);
	if(grPtTable.size() >= 2){
		if(image.GetBPP() == 24){
			r = float(table[grPtTable[0].right.x * 3 + grPtTable[0].right.y * image.GetPitch() + 2])/255;
			g = float(table[grPtTable[0].right.x * 3 + grPtTable[0].right.y * image.GetPitch() + 1])/255;
			b = float(table[grPtTable[0].right.x * 3 + grPtTable[0].right.y * image.GetPitch()])/255;
			sct.push_back(sColor(r, g, b, 1., 0.));
			for(std::vector<grPoint>::iterator i = grPtTable.begin() + 1; i < grPtTable.end() - 1; i++){
				r = float(table[i->left.x * 3 + i->left.y * image.GetPitch() + 2])/255;
				g = float(table[i->left.x * 3 + i->left.y * image.GetPitch() + 1])/255;
				b = float(table[i->left.x * 3 + i->left.y * image.GetPitch()])/255;
				pos = dx * float(i - grPtTable.begin());
				sct.push_back(sColor(r,g,b,1.,pos));
				r = float(table[i->right.x * 3 + i->right.y * image.GetPitch() + 2])/255;
				g = float(table[i->right.x * 3 + i->right.y * image.GetPitch() + 1])/255;
				b = float(table[i->right.x * 3 + i->right.y * image.GetPitch()])/255;
				sct.push_back(sColor(r,g,b,1.,pos));
			}
			r = float(table[grPtTable[grPtTable.size() - 1].left.x * 3 + grPtTable[grPtTable.size() - 1].left.y * image.GetPitch() + 2])/255;
			g = float(table[grPtTable[grPtTable.size() - 1].left.x * 3 + grPtTable[grPtTable.size() - 1].left.y * image.GetPitch() + 1])/255;
			b = float(table[grPtTable[grPtTable.size() - 1].left.x * 3 + grPtTable[grPtTable.size() - 1].left.y * image.GetPitch()])/255;
			sct.push_back(sColor(r, g, b, 1., 1.));
		}else if(image.GetBPP() == 32){
			float a;
			r = float(table[grPtTable[0].right.x * 4 + grPtTable[0].right.y * image.GetPitch() + 2])/255;
			g = float(table[grPtTable[0].right.x * 4 + grPtTable[0].right.y * image.GetPitch() + 1])/255;
			b = float(table[grPtTable[0].right.x * 4 + grPtTable[0].right.y * image.GetPitch()])/255;
			a = float(table[grPtTable[0].right.x * 4 + grPtTable[0].right.y * image.GetPitch() + 3])/255;
			sct.push_back(sColor(r, g, b, a, 0.));
			for(std::vector<grPoint>::iterator i = grPtTable.begin() + 1; i < grPtTable.end() - 1; i++){
				r = float(table[i->left.x * 4 + i->left.y * image.GetPitch() + 2]) / 255;
				g = float(table[i->left.x * 4 + i->left.y * image.GetPitch() + 1]) / 255;
				b = float(table[i->left.x * 4 + i->left.y * image.GetPitch()]) / 255;
				a = float(table[i->left.x * 4 + i->left.y * image.GetPitch() + 3]) / 255;
				pos = dx * float(i - grPtTable.begin());
				sct.push_back(sColor(r,g,b,a,pos));
				r = float(table[i->right.x * 4 + i->right.y * image.GetPitch() + 2]) / 255;
				g = float(table[i->right.x * 4 + i->right.y * image.GetPitch() + 1]) / 255;
				b = float(table[i->right.x * 4 + i->right.y * image.GetPitch()]) / 255;
				a = float(table[i->right.x * 4 + i->right.y * image.GetPitch() + 3]) / 255;
				sct.push_back(sColor(r,g,b,a,pos));
			}
			r = float(table[grPtTable[grPtTable.size() - 1].left.x * 4 + grPtTable[grPtTable.size() - 1].left.y * image.GetPitch() + 2])/255;
			g = float(table[grPtTable[grPtTable.size() - 1].left.x * 4 + grPtTable[grPtTable.size() - 1].left.y * image.GetPitch() + 1])/255;
			b = float(table[grPtTable[grPtTable.size() - 1].left.x * 4 + grPtTable[grPtTable.size() - 1].left.y * image.GetPitch()])/255;
			a = float(table[grPtTable[grPtTable.size() - 1].left.x * 4 + grPtTable[grPtTable.size() - 1].left.y * image.GetPitch() + 3])/255;
			sct.push_back(sColor(r, g, b, a, 1.));
		}
		gr.tSegment.clear();
		for(int i = 0; i < (int)sct.size() / 2; i++){
			gr.tSegment.push_back(segment(sct[i * 2], sct[i * 2 + 1]));
		}
		gr.CreateGradientVector();
	}
}

void myImage::CreateGr(POINT fp,POINT lp,UINT ind, RECT WndSize){

	fp.x = min(max(fp.x, 0),image.GetWidth()-1);
	fp.y = min(max(fp.y, 0),image.GetHeight()-1);
	lp.x = min(max(lp.x, 0),image.GetWidth()-1);
	lp.y = min(max(lp.y, 0),image.GetHeight()-1);
	float dx = float(fp.x - lp.x);//mp.x - lmp.x;
	float dy = float(fp.y - lp.y);

	std::vector<float> v;

	BYTE *table = (BYTE*)this->image.GetBits();

	if(image.GetBPP() == 24){ 

		v.push_back(float(table[lp.x * 3 + lp.y * image.GetPitch() + 2]) / 255);
		v.push_back(float(table[lp.x * 3 + lp.y * image.GetPitch() + 1]) / 255);
		v.push_back(float(table[lp.x * 3 + lp.y * image.GetPitch()]) / 255);
		v.push_back(1.f);
		
		for(UINT i = 1; i < ind + 1; i++){
			int x = int(lp.x + dx / ind * i);
			int y = int(lp.y + dy / ind * i);

			v.push_back(float(table[x * 3 + y * image.GetPitch() + 2]) / 255);
			v.push_back(float(table[x * 3 + y * image.GetPitch() + 1]) / 255);
			v.push_back(float(table[x * 3 + y * image.GetPitch()]) / 255);
			v.push_back(1.f);

		}

		gr.SetGradient(v);
	}else if(image.GetBPP() == 32){

		v.push_back(float(table[lp.x * 4 + lp.y * image.GetPitch() + 2])/255);
		v.push_back(float(table[lp.x * 4 + lp.y * image.GetPitch() + 1])/255);
		v.push_back(float(table[lp.x * 4 + lp.y * image.GetPitch()])/255);
		v.push_back(float(table[lp.x * 4 + lp.y * image.GetPitch() + 3])/255);
		
		for(UINT i = 1; i < ind + 1; i++){
			int x = int(lp.x + dx / ind * i);
			int y = int(lp.y + dy / ind * i);

			v.push_back(float(table[x * 4 + y * image.GetPitch() + 2])/255);
			v.push_back(float(table[x * 4 + y * image.GetPitch() + 1])/255);
			v.push_back(float(table[x * 4 + y * image.GetPitch()])/255);
			v.push_back(float(table[x * 4 + y * image.GetPitch() + 3])/255);

		}

		gr.SetGradient(v);
	}
}

void myImage::SetGradientColors(POINT fp, POINT dp){
	if(image.GetBPP() == 24){
		gr.SetGradientColors(fp, dp, (BYTE*) this->image.GetBits(),image.GetPitch());
	}else if(image.GetBPP() == 32){
		gr.SetGradientColors32(fp, dp, (BYTE*) this->image.GetBits(), image.GetPitch());
	}
}

void myImage::CreateGradientFile(){
	if(image.GetBPP() == 24 || image.GetBPP() == 32){ 

		dlgOpenSaveFileName o(hInst, "pliki *.ggr\0*.ggr\0plik *.svg\0*.svg\0plik *.css\0*.css\0");
		if(o.ShowSaveFileName()){
			switch(o.GetIndex()){
				case 1:
					{
						std::fstream file(o.GetFileName(),std::ios_base::out);

						if(!file.fail()){
							std::string buffor("GIMP Gradient");
							buffor += char(10u);
							buffor += "Name: ";
							buffor += o.GetTitleName();
							buffor.erase(buffor.size() - 4, buffor.size() - 1);
							buffor += char(10u);
							char b2[200];
							sprintf(b2,"%i",gr.tSegment.size());
							buffor += b2;
							buffor += char(10u);
							buffor += gr.GradientToString();
							file.write(buffor.c_str(),buffor.size());
						}

						file.close();
					}
					break;
				case 2:
					{
						std::fstream file2(o.GetFileName(),std:: ios_base::out);

						std::string s = gr.GetSvgGradientString(o.GetTitleName());
						file2.write(s.c_str(), s.size());

						file2.close();
					}
				case 3:
					{
						std::fstream file(o.GetFileName(),std:: ios_base::out);

						std::string s = gr.GetCssGradientString();
						file.write(s.c_str(), s.size());

						file.close();
					}
			}

		}
	}
}

int myImage::GetBPP(){
	return this->image.GetBPP();
}
int myImage::X(){
	return x;
}
void myImage::X(int x){
	this->x = x;
}
int myImage::Y(){
	return y;
}
void myImage::Y(int y){
	this->y = y;
}
double myImage::Scale(){
	return scale;
}
void myImage::Scale(double scale){
	if(scale >= 0.1 && scale <= 10){
		this->scale = scale;
	}
}
double myImage::xResolution(enum unit sunit){
	switch(sunit){
		case pixel:
			return 1.0;
		case milimiter:
			return hResolution / 25.4;
		case inch:
			return hResolution;
	}
	return 1.0;
}
double myImage::yResolution(enum unit sunit){
	switch(sunit){
		case pixel:
			return 1.0;
		case milimiter:
			return vResolution / 25.4;
		case inch:
			return vResolution;
	}
	return 1.0;
}