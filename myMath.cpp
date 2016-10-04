#include "myMath.h"

double myMath::GetPI(void){
	return acos(0.)*2;
}

double myMath::RadiansToDegrees(double ktInRadians){
	return ktInRadians*90/acos(0.);
}

double myMath::DegreesToRadians(double ktInDegrees){
	return ktInDegrees*acos(0.)/90;
}

double myMath::Odl3D(double x1,double y1,double z1,double x2,double y2,double z2){
	return pow(pow(x1-x2,2.)+pow(y1-y2,2.)+pow(z1-z2,2.),.5);
}

int myMath::Zaokr(double w){
	double w2=w-(int)w;
	int a=(int)w;
	if(abs(w2)>.5)
		if(w2>0)
			return (int)w+1;
		else
			return (int)w-1;
	else
		return (int)w;
}

double myMath::Zaokr(double w,UINT nrPoPrzecinku){
	w = Zaokr(w * pow(10.,(double)nrPoPrzecinku));
	w = w / pow(10.,(double)nrPoPrzecinku);
	return w;
}

POINT myMath::RotatePoint(int xObracany,int yObracany,int xObrotu,int yObrotu,double ktObrotu){
	POINT p;
	double odl=Odl(xObracany,yObracany,xObrotu,yObrotu);
	double ktPocz=GetAngle(xObrotu,yObrotu,xObracany,yObracany);
	p.x=Zaokr(odl*cos(ktObrotu+ktPocz))+xObrotu;
	p.y=Zaokr(odl*sin(ktObrotu+ktPocz))+yObrotu;
	return p;
}

double myMath::RayOfArc(int x1,int y1,int x2,int y2,int x3,int y3){
	double l1=Odl(x1,y1,x2,y2),l2=Odl(x1,y1,x3,y3),l3=Odl(x2,y2,x3,y3);
	double alfa,beta;
	alfa= acos((pow(l1,2.) + pow(l2,2.) - pow(l3,2.)) / (2 * l1 * l2));
	beta=4*asin(1.)-2*alfa;
	return pow(pow(l3,2.) / (2 - 2 * cos(beta)),0.5);
}

double myMath::RayOfArc(POINT p1,POINT p2,POINT p3){
	double l1=Odl(p1,p2),l2=Odl(p1,p3),l3=Odl(p2,p3);
	double alfa,beta;
	alfa= acos((pow(l1,2.) + pow(l2,2.) - pow(l3,2.)) / (2 * l1 * l2));
	beta=4*asin(1.)-2*alfa;
	return pow(pow(l3,2.) / (2 - 2 * cos(beta)),0.5);
}

POINT myMath::Bezier(double t, POINT firstPoint,POINT lastPoint,double firstAngle,double lastAngle, double firstRay,double lastRay){
	if(t < 0)
		t = 0;
	else if(t > 1)
		t = 1;
	POINT pt;
	pt.x = LONG(pow((1. - t),3.) * double(firstPoint.x)
		+ 3 * t * pow(1 - t,2.) * double(firstPoint.x + firstRay * cos(firstAngle)) 
		+ 3 * pow(t,2.) * (1 - t) * double(lastPoint.x + lastRay * cos(lastAngle))
		+ pow(t, 3.) * lastPoint.x);
	pt.y = LONG(pow((1. - t),3.) * double(firstPoint.y)
		+ 3 * t * pow(1 - t,2.) * double(firstPoint.y + firstRay * sin(firstAngle)) 
		+ 3 * pow(t,2.) * (1 - t) * double(lastPoint.y + lastRay * sin(lastAngle))
		+ pow(t, 3.) * lastPoint.y);
	return pt;
}
POINT myMath::Bezier(double t, POINT firstPoint,POINT lastPoint,POINT first,POINT last){
	if(t < 0)
		t = 0;
	else if(t > 1)
		t = 1;
	POINT pt;
	pt.x = LONG(pow((1. - t),3.) * double(firstPoint.x)
		+ 3 * t * pow(1 - t,2.) * double(first.x) 
		+ 3 * pow(t,2.) * (1 - t) * double(last.x)
		+ pow(t, 3.) * lastPoint.x);
	pt.y = LONG(pow((1. - t),3.) * double(firstPoint.y)
		+ 3 * t * pow(1 - t,2.) * double(first.y) 
		+ 3 * pow(t,2.) * (1 - t) * double(last.y)
		+ pow(t, 3.) * lastPoint.y);
	return pt;
}

//Funkcja licz¹ca odleg³oœæ miêdzy dwiema wspó³¿êdnymi
double myMath::Odl(double x1,double y1,double x2,double y2){
	return pow(pow((double)(x1-x2),2.0)+pow((double)(y1-y2),2.0),.5);
}

double myMath::Odl(POINT p1,POINT p2){
	return Odl(p1.x,p1.y,p2.x,p2.y);
}
//Funkcje licz¹ca k¹t zawarty miêdzy dwoma punktami
double myMath::GetAngle(double x1,double y1,double x2,double y2){
	if(x1==x2 && y1==y2)
		return 0;
	double L=pow(pow((double)(x1-x2),2.)+pow((double)(y1-y2),2.),0.5);
	double PI=asin(1.0)*2;
	if(y2<y1)
		return acos((x1-x2)/L);
	else
		return acos((x1-x2)/-L)+PI;
}

double myMath::GetAngle(POINT p1,POINT p2){
	return GetAngle(p1.x,p1.y,p2.x,p2.y);
}

void StartupFile(LPSTR &startupFile,LPSTR startupFileName){
	char *path=GetCommandLine();
	char *p=strrchr(path, '\\');
	*p=NULL;
	if(path[0]==34)
		path++;
	strcat_s(path,sizeof(path),startupFileName);
	startupFile=new char[strlen(path)+1];
	strcpy(startupFile,path);
}

void StartupPath(LPSTR &startupPath){
	char *path=GetCommandLine();
	char *p=strrchr(path,'\\');
	*p=NULL;
	if(path[0]==34)
		path++;
	startupPath=new char[strlen(path)+1];
	strcpy(startupPath,path);
}