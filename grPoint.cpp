#include "myImage.h"

grPoint::grPoint(POINT left, POINT right){
	this->left = left;
	this->right = right;
}
grPoint::grPoint(){
	left.x = left.y = right.x = right.y = 0;
}

int grPoint::PtIn(POINT mp, int ray){
	if((left - mp) * (left - mp) <= ray){
		if(left != right){
			return 1;
		}
		return 3;
	}else if((right - mp) * (right - mp) <= ray){
		return 2;
	}
	return 0;
}