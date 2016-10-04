#include "segment.h"

POINT operator +(POINT fp, POINT lp){
	POINT pt;
	pt.x = fp.x + lp.x;
	pt.y = fp.y + lp.y;
	return pt;
}
void operator *=(POINT &pt,double k){
	pt.x *= k;
	pt.y *= k;
}

POINT operator *(POINT pt,double k){
	pt.x *= k;
	pt.y *= k;
	return pt;
}

POINT operator -(POINT fp,POINT lp){
	fp.x -= lp.x;
	fp.y -= lp.y;
	return fp;
}

double operator *(POINT fp, POINT lp){
	return fp.x * lp.x + fp.y * lp.y;
}

POINT GetPointOnLine(POINT fP, POINT lP, POINT rP){
	double u = ((rP - fP) * (lP - fP)) / ((fP - lP) * (fP - lP));
    return fP + (lP - fP) * u;
}

bool operator != (POINT &fp, POINT &lp){
	return fp.x != lp.x || fp.y != lp.y;
}

segment::segment(void)
{
	c_sys = C_SYS_RGB;
}

segment::segment(sColor l, sColor r){
	left = l;
	right = r;
	center.SetRGB((l.r + r.r) / 2.f, (l.g + r.g) / 2.f, (l.b + r.b) / 2.f, (l.a + r.a) / 2.f, (r.pos + l.pos) / 2.f);
	c_sys = C_SYS_RGB;
}

segment::segment(std::string s){
	while(s.size() && s[0] == ' ')
		s.erase(s.begin());
	for(std::string::size_type i = 0; i < s.size(); i++){
		if(s[i]==' '){
			while(s.size() >= i+1 && s[i+1] == ' '){
				s.erase(s.begin() + i + 1);
			}
		}
	}
	float pos1, pos2, pos3, ap, ak, rp, rk, gp, gk, bp, bk;
	pos1 = pos2 = pos3 = ap = ak = rp = rk = gp = gk = bp = bk = 0;
	int v1 = 0;
	c_sys = C_SYS_RGB;
	sscanf(s.c_str(), "%f %f %f %f %f %f %f %f %f %f %f %i %i",&pos1, &pos2, &pos3, &rp, &gp, &bp, &ap, &rk, &gk, &bk, &ak, &v1, &c_sys);
	switch(c_sys)
	{
		case C_SYS_HSV:
			{
				RGB2HSV(rp, gp, bp, rp, gp, bp);
				RGB2HSV(rk, gk, bk, rk, gk, bk);
				if(rp >= rk)
					rk += 360;
				rp /= 360.f;
				rk /= 360.f;
				this->left.SetHSV(rp, gp, bp, ap, pos1);
				this->center.SetHSV((rk + rp) / 2.f, (gk + gp) / 2.f, (bk + bp) / 2.f, (ak + ap) / 2.f, pos2);
				this->right.SetHSV(rk, gk, bk, ak, pos3);
			}
			break;
		case C_SYS_HSV2:
			{
				RGB2HSV(rp, gp, bp, rp, gp, bp);
				RGB2HSV(rk, gk, bk, rk, gk, bk);
				if(rp <= rk)
					rp += 360;
				rp /= 360.f;
				rk /= 360.f;
				this->left.SetHSV(rp, gp, bp, ap, pos1);
				this->center.SetHSV((rk + rp) / 2.f, (gk + gp) / 2.f, (bk + bp) / 2.f, (ak + ap) / 2.f, pos2);
				this->right.SetHSV(rk, gk, bk, ak, pos3);
			}
			break;
		case C_SYS_RGB:
		default:
			{
				this->left.SetRGB(rp, gp, bp, ap, pos1);
				this->center.SetRGB((rk + rp) / 2.f, (gk + gp) / 2.f, (bk + bp) / 2.f, (ak + ap) / 2.f, pos2);
				this->right.SetRGB(rk, gk, bk, ak, pos3);
			}
			break;
	}
	/*MessageBox(NULL,"coœ",s.c_str(),MB_OK);*/
}

POINT segment::SegmentOnScreen(POINT fp,POINT dp){
	return fp + dp * this->right.pos;
}

int segment::SegmentOnScreen(UINT width){
	return int(width * right.pos);
}

void segment::SetLeftColorRGB(float r, float g, float b, float a){
	switch(c_sys){
		case C_SYS_HSV:
			{
				RGB2HSV(r, g, b, r, g, b);
				if(r >= right.r * 360.f)
					r += 360;
				r /= 360.f;
			}
			break;
		case C_SYS_HSV2:
			{
				RGB2HSV(r, g, b, r, g, b);
				if(r <= left.r * 360.f)
					r += 360;
				r /= 360.f;
			}
			break;
	}
	this->left.r =r;
	left.g = g;
	left.b = b;
	left.a = a;
	this->center.SetRGB((right.r + left.r) / 2.f, (right.g + left.g) / 2.f, (right.b + left.b) / 2.f, (right.a + left.a) / 2.f,center.pos);
}

void segment::SetRightColorRGB(float r, float g, float b, float a){
	switch(c_sys){
		case C_SYS_HSV:
			{
				RGB2HSV(r, g, b, r, g, b);
				if(r >= right.r * 360.f)
					r += 360;
				r /= 360.f;
			}
			break;
		case C_SYS_HSV2:
			{
				RGB2HSV(r, g, b, r, g, b);
				if(r <= left.r * 360.f)
					r += 360;
				r /= 360.f;
			}
			break;
	}
	this->right.r = r;
	right.g = g;
	right.b = b;
	right.a = a;
	this->center.SetRGB((right.r + left.r) / 2.f, (right.g + left.g) / 2.f, (right.b + left.b) / 2.f, (right.a + left.a) / 2.f,center.pos);
}

void segment::GetLeftColorRGB(float &r, float &g, float &b, float &a){
	r = left.r;
	g = left.g;
	b = left.b;
	a = left.a;
	switch(c_sys){
		case C_SYS_HSV:
		case C_SYS_HSV2:
			{
				HSV2RGB(r, g, b, r, g, b);
			}
			break;
	}
}

void segment::GetRightColorRGB(float &r, float &g, float &b, float &a){
	r = right.r;
	g = right.g;
	b = right.b;
	a = right.a;
	switch(c_sys){
		case C_SYS_HSV:
		case C_SYS_HSV2:
			{
				HSV2RGB(r, g, b, r, g, b);
			}
			break;
	}
}

void segment::SetRightPos(float pos){
	if(pos < left.pos)
		return ;
	float k = (center.pos - left.pos) / (right.pos - left.pos);
	if(k > 0)
		center.pos = (pos - left.pos) * k + left.pos;
	else
		center.pos = (pos + left.pos) / 2.f;
	right.pos = pos;
}

void segment::SetLeftPos(float pos){
	if(pos > right.pos)
		return ;
	float k = (center.pos - left.pos) / (right.pos - left.pos);
	if(k > 0){
		center.pos = (right.pos - pos) * k + pos;
	}else{
		center.pos = (right.pos + pos) / 2.f;
	}
	left.pos = pos;
}

void segment::SetRightAndLeftPos(float left, float right){
	if(left > right){
		float temp = left;
		left = right;
		right = temp;
	}

	this->left.pos = left;
	this->right.pos = right;
	this->center.pos = (left + right) * 0.5f;
}

void segment::CreateGradientVector(float &pos, float &dk, std::vector<BYTE>::iterator &c, std::vector<float>::iterator &a){
	float dyr = center.r - left.r;
	float dyb = center.b - left.b;
	float dyg = center.g - left.g;
	float dya = center.a - left.a;
	float dx = center.pos - left.pos;
	float dX;

	switch(c_sys)
	{
		case C_SYS_HSV:
		case C_SYS_HSV2:
			{
				float r = 0;
				float g = 0;
				float b = 0;
				if(center.pos - left.pos > 0){
					while(pos >= left.pos && pos <= center.pos && pos <= 1.){
						dX = pos - left.pos;
						HSV2RGB((dX * dyr / dx + left.r) * 360.f, dX * dyg / dx + left.g,dX * dyb / dx + left.b, r, g, b);
						*(c + 2) =	BYTE(r * 255.f);
						*c =		BYTE(b * 255.f);
						*(c + 1) =	BYTE(g * 255.f);
						*a =		dX * dya / dx + left.a;
						c+=3;
						a++;
						pos += dk;
					}
					
					if(pos > 1.){
						c-=3;
						a--;
						HSV2RGB(center.r * 360.f, center.g, center.b, r, g, b);
						*(c + 2) =	BYTE(r * 255.f);
						*c =		BYTE(b * 255.f);
						*(c + 1) =	BYTE(g * 255.f);
						*a =		center.a; 
					}
				}

				if(right.pos - center.pos > 0){
					dyr = right.r - center.r;
					dyb = right.b - center.b;
					dyg = right.g - center.g;
					dya = right.a - center.a;
					dx = right.pos - center.pos;

					while(pos >= center.pos && pos < right.pos + dk  && pos <= 1.){
						dX = (pos > right.pos ? right.pos : pos) - center.pos;
						HSV2RGB((dX * dyr / dx + center.r) * 360.f, dX * dyg / dx + center.g,dX * dyb / dx + center.b, r, g, b);
						*(c + 2) =	BYTE(r * 255.f);
						*c =		BYTE(b * 255.f);
						*(c + 1) =	BYTE(g * 255.f);
						*a =		dX * dya / dx + center.a;
						c +=		3;
						a ++;
						pos += dk;
					}
					if(pos > 1.){
						c-=3;
						a--;
						HSV2RGB(right.r * 360.f, right.g, right.g, r, g, b);
						*(c + 2) =	BYTE(r * 255.f);
						*c =		BYTE(b * 255.f);
						*(c + 1) =	BYTE(g * 255.f);
						*a =		right.a; 
					}
				}
			}
			break;
		case C_SYS_RGB:
		default:
			{
				if(center.pos - left.pos > 0){
					while(pos >= left.pos && pos <= center.pos && pos <= 1.){
						dX = pos - left.pos;
						*(c + 2) =	BYTE((dX * dyr / dx + left.r) * 255.f);
						*c =		BYTE((dX * dyb / dx + left.b) * 255.f);
						*(c + 1) =	BYTE((dX * dyg / dx + left.g) * 255.f);
						*a =		dX * dya / dx + left.a;
						c+=3;
						a++;
						pos += dk;
					}
					
					if(pos > 1.){
						c-=3;
						a--;
						*(c + 2) =	BYTE(center.r * 255.f);
						*c =		BYTE(center.b * 255.f);
						*(c + 1) =	BYTE(center.g * 255.f);
						*a =		center.a; 
					}
				}

				if(right.pos - center.pos > 0){
					dyr = right.r - center.r;
					dyb = right.b - center.b;
					dyg = right.g - center.g;
					dya = right.a - center.a;
					dx = right.pos - center.pos;

					while(pos >= center.pos && pos < right.pos + dk  && pos <= 1.){
						dX = (pos > right.pos ? right.pos : pos) - center.pos;
						*(c + 2) =	BYTE((dX * dyr / dx + center.r) * 255.f);
						*c =		BYTE((dX * dyb / dx + center.b) * 255.f);
						*(c + 1) =	BYTE((dX * dyg / dx + center.g) * 255.f);
						*a =		dX * dya / dx + center.a;
						c += 3;
						a ++;
						pos += dk;
					}
					if(pos > 1.){
						c-=3;
						a--;
						*(c + 2) =	BYTE(right.r * 255.f);
						*c =		BYTE(right.b * 255.f);
						*(c + 1) =	BYTE(right.g * 255.f);
						*a =		right.a; 
					}
				}
			}
			break;
	}
}

segment::~segment(void)
{
}

bool segment::CheckColorPos(segment *segmentBefore){
	if(segmentBefore){
		if(segmentBefore->right.pos != left.pos)
			return false;
	}
	if(left.pos > center.pos || left.pos > right.pos || center.pos > right.pos)
		return 0;
}

std::string segment::SegmentToString(bool last){
	char c[255];
	switch(c_sys){
		case C_SYS_HSV:
		case C_SYS_HSV2:
			{
				float leftr,leftg,leftb, rightr, rightg, rightb;
				leftr = leftg = leftb = rightr = rightg = rightb = 0.f;
				HSV2RGB(left.r * 360.f, left.g, left.b, leftr, leftg, leftb);
				HSV2RGB(right.r * 360.f, right.g, right.b, rightr, rightg, rightb);
				if(last){
					sprintf(c,"%f %f %f %f %f %f %f %f %f %f %f 0 0 %i 0%c",double(left.pos),double(center.pos),double(right.pos), leftr,leftg,leftb,left.a,rightr,rightg,rightb,right.a, c_sys, char(10));
				}else{
					sprintf(c,"%f %f %f %f %f %f %f %f %f %f %f 0 %i%c",double(left.pos),double(center.pos),double(right.pos), leftr,leftg,leftb,left.a,rightr,rightg,rightb,right.a, c_sys, char(10));
				}
				return std::string(c);
			}
			break;
		case C_SYS_RGB:
		default:
			{
				if(last){
					sprintf(c,"%f %f %f %f %f %f %f %f %f %f %f 0 0 0 0%c",double(left.pos),double(center.pos),double(right.pos), left.r,left.g,left.b,left.a,right.r,right.g,right.b,right.a, char(10));
				}else{
					sprintf(c,"%f %f %f %f %f %f %f %f %f %f %f 0 0%c",double(left.pos),double(center.pos),double(right.pos), left.r,left.g,left.b,left.a,right.r,right.g,right.b,right.a, char(10));
				}
				return std::string(c);
			}
	}
}