#include "FunkcjeSprawdzajace.h"

bool SprFu::PtInRay(int xC,int yC,UINT Ray,int dRay,POINT pt){
	if(dRay<=0)
		dRay=1;
	return (PtInCircle(xC,yC,Ray+dRay/2,pt) && !PtInCircle(xC,yC,Ray-dRay/2,pt))?true:false;
}

bool SprFu::PtInCircle(POINT CentralPoint,double Ray,POINT pt){
	UINT l=(UINT)pow(pow((double)(CentralPoint.x-pt.x),2.)+pow((double)(CentralPoint.y-pt.y),2.),0.5);
	if(l>Ray)
		return false;
	return true;
}

bool SprFu::PtInCircle(int xC,int yC,double Ray,POINT pt){
	double l=pow(pow((double)(xC-pt.x),2.)+pow((double)(yC-pt.y),2.),0.5);
	if(l>Ray)
		return false;
	return true;
}

bool SprFu::LiczbaPierwsza(int liczba){
	liczba=abs(liczba);
	if(!(liczba>1)){
		return false;
	}
	if(liczba<4){
		return true;
	}
	int i=2;
	while(i<=liczba/i){
		if(!(liczba%i)){
			return false;
		}
		i++;
	}
	return true;
}

SprFu::SprFu(){
}

bool SprFu::IsNumeric(std::string num){
	bool eExist = false;
	bool dotExist = false;
	for(std::string::iterator i = num.begin(); i < num.end(); i++){
		switch((int)*i){
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				break;
			case '.':
				if(!dotExist){
					dotExist = true;
				}else{
					return false;
				}
				break;
			case 'e':
				if(i == num.end() - 1 || eExist){	// e mo¿e wyst¹piæ tylko raz i w odleg³oœci conajmniej 1-go znaku od koñca
					return false;
				}else if(num.end() - 2 >= i){	// minus mo¿e wyst¹piæ po e tylko w odleg³oœci conajmniej 2-ch znaków od koñca np 1e-2 - poprawny 1e- - b³êdny
					if(*(i + 1) == '-')
						i++;
				}
				eExist = true;
				break;
			case '-':	// Gdy minus na pocz¹tku
				if(i != num.begin())
					return false;
				break;
			default:
				return false;
		}
	}
	return true;
}