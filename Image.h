#include <Windows.h>
#include <math.h>

#ifndef IMAGE_H
#define IMAGE_H
class Image
{
	HBITMAP hImage;
public:
	Image(HDC hdc,int width,int height);
	Image(int width,int height);
};
#endif
