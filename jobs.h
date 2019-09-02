#include "global.h"

void showjobs(){
// 			Shows jobs	
	printf("%lld \n",bgind);
	for(int r=0;r<bgind;r++){
		printf("%lld %s\n",procaarray[r],characterarray[procaarray[r]]);
	}
}
