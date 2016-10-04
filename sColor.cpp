#include "sColor.h"

//sColor::sColor(void)
//{
//	r = g = b = pos = 0.;
//}

//sColor::sColor(double r, double g, double b, double a, double pos){
//	this->r = r;
//	this->g = g;
//	this->b = b;
//	this->pos = pos;
//}

int HexSingleValue(char c){
    switch(int(c)){
        case '0':
             return 0;
        case '1':
             return 1;
        case '2':
             return 2;
        case '3':
             return 3;
        case '4':
             return 4;
        case '5':
             return 5;
        case '6':
             return 6;
        case '7':
             return 7;
        case '8':
             return 8;
        case '9':
             return 9;
        case 'a':
        case 'A':
             return 10;
        case 'b':
        case 'B':
             return 11;
        case 'c':
        case 'C':
             return 12;
        case 'd':
        case 'D':
             return 13;
        case 'e':
        case 'E':
             return 14;
        case 'f':
		case 'F':
             return 15;
        default:
             return -1;
    }
}

int CharHexToInt(std::string str){
    int dig = str.size();
    int value = 0;
	int v = 0;
    for(int i = 0; i < dig; i++){
		v = HexSingleValue(str[i]);
		if(v == -1)
			return -1;
        value += v*pow(16.,double(dig -i-1));
    }
    return value;
}

char GetCharHex(int c){
    switch(c){
        case 0:
             return '0';
        case 1:
             return '1';
        case 2:
             return '2';
        case 3:
             return '3';
        case 4:
             return '4';
        case 5:
             return '5';
        case 6:
             return '6';
        case 7:
             return '7';
        case 8:
             return '8';
        case 9:
             return '9';
        case 10:
             return 'a';
        case 11:
             return 'b';
		case 12:
             return 'c';
        case 13:
             return 'd';
        case 14:
             return 'e';
        case 15:
             return 'f';
        default:
             return '-';
    }
}

std::string IntToCharHex(int v){
	int r = 0;
	std::string vStrHex;
	do{
		r = v % 16;
		v = (v - r) / 16;
		vStrHex.insert(vStrHex.begin(), GetCharHex(r));
	}while(v != 0);
	return vStrHex;
}


void HSV2RGB(float h, float s, float v, float &r, float &g, float &b)
{
int i;
h = fmod(h, 360.f);
float f, p, q, t;
if(v == 0){
	r = 0;
	g = 0;
	b = 0;
}else{
		h /= 60;
		i = int(floor(h));
		f = h - i;
		p = v * (1 - s);
		q = v * (1 - (s * f));
		t = v * (1 - (s * (1 - f)));
		if (i == 0){
			r = v;
			g = t;
			b = p;
		}else if (i == 1) {
			r = q;
			g = v;
			b = p;
		}else if (i == 2){
			r = p;
			g = v;
			b = t;
		}else if (i == 3){
			r = p;
			g = q;
			b = v;
		}else if (i == 4){
			r = t;
			g = p;
			b = v;
		}else if (i == 5){
			r = v;
			g = p;
			b = q;
		}
	}
}

void RGB2HSV(float &h, float &s, float &v, float r, float g, float b){
        float x, f, i;

        x = min(min(r, g), b);
        v = max(max(r, g), b);
        if (x == v){
            h = s = 0;
        }
        else {
            f = (r == x) ? g - b : ((g == x) ? b - r : r - g);
            i = (r == x) ? 3.f : ((g == x) ? 5.f : 1.f);
            h = fmod((i - f/(v - x)) * 60.f, 360.f);
            s = ((v - x) / v);
        }
}

val::val(std::string vText){
	if(vText.size()){
		std::string::size_type index = vText.find_first_of('=',0);
		if(index != std::string::npos){
			name.insert(0,vText,0,index);
			value.insert(0,vText,index + 2, vText.size() - index - 3);
		}
	}
}

int c_xml::FindValueIndex(std::string valueName){
	for(std::vector<val>::iterator i = tVal.begin(); i < tVal.end(); i++){
		if(!valueName.compare(i->name)){
			return int(i - tVal.begin());
		}
	}
	return -1;
}

void c_xml::EraseRekordIfNameNotMatch(std::string name){
	for(int i = 0; i < xmlT.size(); i++){
		if(name.compare(xmlT[i].vName)){
			xmlT.erase(xmlT.begin() + i);
			i--;
		}
	}
}

void c_xml::EraseRekordIfDontHaveChild(){
	for(int i = 0; i < xmlT.size(); i++){
		if(!xmlT[i].xmlT.size()){
			xmlT.erase(xmlT.begin() + i);
			i--;
		}
	}
}

c_xml::c_xml(std::vector<std::string> &t){
	vName.insert(0,t[0],1,t[0].size()-1);
	t.erase(t.begin());
	if(t.size()){
		bool w = true;
		while(t.size() && w){
			if(t[0][t[0].size()-1] == '>'){
				w = false;
				t[0].erase(t[0].end()-1);
			}
			if(t[0][t[0].size()-1] == '/'){
				t[0].erase(t[0].end()-1);
				tVal.push_back(val(t[0]));
				t.erase(t.begin());
			}else{
				tVal.push_back(val(t[0]));
				t.erase(t.begin());
				if(!w){
					std::string s = "</";
					s += vName;
					s += ">";
					while(t.size() && s.compare(t[0])){
						xmlT.push_back(c_xml(t));
					}
					t.erase(t.begin());
				}

			}

		}
	}
}

sColor::~sColor(void)
{
}

sColor::sColor(c_xml stopPoint){
	if(!stopPoint.vName.compare(std::string("stop")) && stopPoint.tVal.size() == 3){
		int iStyle = stopPoint.FindValueIndex(std::string("style"));
		int iOffset = stopPoint.FindValueIndex(std::string("offset"));
		std::string::size_type st = stopPoint.tVal[iStyle].value.find("rgb(",0);
		if(st == std::string::npos){
			std::string s;
			s.insert(0,stopPoint.tVal[iStyle].value,12,2);
			r = float(CharHexToInt(s.c_str())) / 255.f;
			s.clear();
			s.insert(0,stopPoint.tVal[iStyle].value,14,2);
			g = float(CharHexToInt(s.c_str())) / 255.f;
			s.clear();
			s.insert(0,stopPoint.tVal[iStyle].value,16,2);
			b = float(CharHexToInt(s.c_str())) / 255.f;
			s.clear();
			s.insert(0,stopPoint.tVal[iStyle].value,32,stopPoint.tVal[0].value.size() - 32);
			if(s[s.size()-1] == ';')
				s.erase(s.end() - 1);
			a = atof(s.c_str());
			pos = atof(stopPoint.tVal[iOffset].value.c_str());
		}else{
			std::string::size_type st2 = stopPoint.tVal[iStyle].value.find(')',0);
			st2;
			st += 4;
			std::string s;
			s.insert(0,stopPoint.tVal[iStyle].value,st, st2-st);
			int r = 0, g = 0, b = 0;
			sscanf(s.c_str(),"%i,%i,%i",&r,&g,&b);
			r = (float) r / 255.f;			
			g = (float) g / 255.f;
			b = (float) b / 255.f;

			st = stopPoint.tVal[iStyle].value.find("stop-opacity:",0);
			if(st != std::string::npos){
				st += 13;
				s.clear();
				s.insert(0, stopPoint.tVal[iStyle].value, st,stopPoint.tVal[iStyle].value.size() - st);
				if(s[s.size() -1] == '%')
					s.erase(s.end() - 1);
				a = atof(s.c_str());
			}
			s.clear();
			s = stopPoint.tVal[iOffset].value;
			if(s[s.size() -1] == '%')
				s.erase(s.end() - 1);
			pos = atol(s.c_str()) / 100.f;
		}
	}
}

void sColor::SetRGB(float r,float g, float b, float a,float pos){
	this->r = min(max(r,0.f),1.f);
	this->g = min(max(g,0.f),1.f);
	this->b = min(max(b,0.f),1.f);
	this->a = min(max(a,0.f),1.f);
	this->pos = min(max(pos,0.f),1.f);
}

void sColor::SetHSV(float r,float g, float b, float a,float pos){
	this->r = r;
	this->g = min(max(g,0.f),1.f);
	this->b = min(max(b,0.f),1.f);
	this->a = min(max(a,0.f),1.f);
	this->pos = min(max(pos,0.f),1.f);
}

bool sColor::operator ==(sColor cs){
	return bool(cs.a == a && cs.r == r && cs.g == g && cs.b == b);
}

std::string sColor::ChangeColorToHexStringRGBA(){
	std::string red;
	std::string green;
	std::string blue;
	std::string alpha;

	red = IntToCharHex(int(r * 255.f));
	if(red.size() == 1)
		red.insert(red.begin(), '0');
	green = IntToCharHex(int(g * 255.f));
	if(green.size() == 1)
		green.insert(green.begin(), '0');
	blue = IntToCharHex(int(b * 255.f));
	if(blue.size() == 1)
		blue.insert(blue.begin(), '0');
	alpha = IntToCharHex(int(a * 255.f));
	if(alpha.size() == 1)
		alpha.insert(alpha.begin(), '0');

	std::string s = red;
	s += green;
	s += blue;
	s += alpha;
	
	return s;
}

std::string sColor::ChangeColorToRGBAString(){
	int red = int(r * 255.f);
	int green = int(g * 255.f);
	int blue = int(b * 255.f);
	float alpha = a;

	std::string s;

	char c[100];
	sprintf(c,"RGBA(%i,%i,%i,%.2f)",red,green,blue,alpha);

	s += c;
	
	return s;
}

std::string sColor::ChangeColorToHexStringRGB(){
	std::string red;
	std::string green;
	std::string blue;

	red = IntToCharHex(int(r * 255.f));
	if(red.size() == 1)
		red.insert(red.begin(), '0');
	green = IntToCharHex(int(g * 255.f));
	if(green.size() == 1)
		green.insert(green.begin(), '0');
	blue = IntToCharHex(int(b * 255.f));
	if(blue.size() == 1)
		blue.insert(blue.begin(), '0');

	std::string s = red;
	s += green;
	s += blue;
	
	return s;
}

std::string sColor::GetSvgString(int &id){
	std::string s;

	s = "<stop";
	s += char(10);
	s += "style=\"stop-color:#";
	s += ChangeColorToHexStringRGB();
	s += ";stop-opacity:";

	char c[100];
	sprintf(c,"%f",a);

	s += c;

	s += ";\"";
	s += char(10);
	s += "offset=\"";

	sprintf(c, "%f", pos);

	s += c;
	s += "\"";
	s += char(10);
	s += "id=\"stop";
	
	sprintf(c, "%i",id);

	s += c;
	
	s += "\"/>";
	s += char(10);

	id ++;

	return s;
}

bool sColor::SetColorFromHexString(std::string hStr){
	if(hStr.size() != 8u){
		return false;
	}
	std::string red;
	red.insert(0, hStr, 0, 2);
	std::string green;
	green.insert(0, hStr, 2, 2);
	std::string blue;
	blue.insert(0, hStr, 4, 2);
	std::string alpha;
	alpha.insert(0, hStr, 6, 2);

	int tr = CharHexToInt(red);
	int tg = CharHexToInt(green);
	int tb = CharHexToInt(blue);
	int ta = CharHexToInt(alpha);

	if(tr == -1 || tg == -1 || tb == -1 || ta == -1)
		return false;

	r = float(tr) / 255.f;
	g = float(tg) / 255.f;
	b = float(tb) / 255.f;
	a = float(ta) / 255.f;

	return true;
}