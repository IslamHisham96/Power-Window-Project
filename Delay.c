#include "delay.h"

void delayUs(long t){
	  long i , j;
	   i = 0;
	   j = 0;
	  while(i<t){
       while(j<3){
				 j++;
			 //Do Nothing
			 }
			 j=0;
			 i++;
	  }
}
//Function to delay t milliseconds
void delayMs(long t){
	  long i , j;
	   i = 0;
	   j = 0;
	  while(i<t){
       while(j<3180){
				 j++;
			 //Do Nothing
			 }
			 j=0;
			 i++;
	  }
}
