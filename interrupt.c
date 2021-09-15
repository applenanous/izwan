
#include <stdio.h>
#include <wiringPi.h>
 

int main()
{
	int DETECT = 27;
	wiringPiSetup(); 
  pinMode(DETECT,INPUT);
  pullUpDnControl(DETECT, PUD_UP);  //Set pulll up
  printf("Pin High\n");
    
    while(1)
    {  
        if (digitalRead(DETECT) == 0) 
        {  
			   //testStatus->state == PAUSE;
          printf ("Test Pause Image Detect\n");
          while(digitalRead(DETECT) == 0)
          delay(100);
        }  
        delay(100);
   }  	
}
