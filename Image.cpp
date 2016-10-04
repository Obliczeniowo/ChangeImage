#include "Image.h"

Image::Image(HDC hdc,int width,int height){
	HDC mem=CreateCompatibleDC(hdc);
	hImage = CreateCompatibleBitmap(hdc,abs(width),abs(height));
	DeleteDC(mem);
}
Image::Image(int width,int height){
	HDC hdc = GetDC(NULL);
	HDC mem = CreateCompatibleDC(hdc);
	hImage = CreateCompatibleBitmap(mem,width,height);
	DeleteDC(mem);
	DeleteDC(hdc);
}
