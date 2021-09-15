/*Description: This file runs Instantaneous AC Test 2a.
Last updated: 03-Aug-2019
Updated by: Wen How
Updated items: descriptions
*/

#include <stdio.h> 
#include <stdlib.h>
//#include <unistd.h>
#include <pthread.h>  
#include <wiringPi.h> 	// Include WiringPi library!
//#include "getch.c"
#include "scanKey.c"

#define RUN 1
#define PAUSE 0
#define STOP -1
#define YES 1
#define NO 0
#define timeOFF_start 40
#define timeOFF_step 	20		// milliseconds
#define timeOFF_max		480	// milliseconds

const int relayPin = 17; 	// Set GPIO pin 17
void instantaneous_AC_test2a (int test_cycle, status *testStatus);

int timeON = 12;

int main (void)
{
	int input_check, test_cycle = 0;
	status testStatus = {NO, RUN};
	pthread_t thread_scanKey;
    wiringPiSetupGpio(); 			// Initialize wiringPi
    pinMode(relayPin, OUTPUT); 		// Set pin as output   
    
    printf("Instantaneous Test 2a is running! Press CTRL+C to quit. \n\n");
	do {
		printf("Enter TV on time (in seconds): ");
		fflush(stdout);
		input_check = scanf("%d", &timeON);
		while (getchar() != '\n');

		if (input_check == 1 && timeON > 0)
			printf("OK. TV ON time is %d second%s. \n", timeON, timeON > 1 ? "s" : "");
		else
			printf("Invalid input. \n");
	} while (input_check == 0 || timeON < 1);

    do {
		printf("Number of cycles: ");
		fflush(stdout);
		input_check = scanf("%d", &test_cycle);
		while(getchar() != '\n');

		if (input_check == 1 && test_cycle > 0)
			printf("OK. Test cycle: %d \n", test_cycle);
		else
			printf("Number entered is invalid / wrong input.\n");
	}while(input_check == 0 || test_cycle <= 0);	// do-while loop to check valid total cycles that need to be performed
	
	testStatus.runTest = YES;
	testStatus.state = RUN;
	pthread_create(&thread_scanKey, NULL, (void*) scan_key, &testStatus);
	printf("Test will start. Use SPACE or P keys to pause test. \n");
	instantaneous_AC_test2a(test_cycle, &testStatus);
	
	testStatus.runTest = NO;
    printf("\nTest is %s. Press any key to exit..\n", (testStatus.state == STOP)?"stopped":"completed");
    //pthread_join(thread_scanKey, NULL);
    getch();
}

void instantaneous_AC_test2a(int test_cycle, status *testStatus)
{
	for (int n=1; n<=test_cycle; n++)
    {
		printf("\nCycle %d starts! \n", n);
		
		digitalWrite(relayPin, HIGH); 		// Turn ON
		printf("\rRound 0: \t\t\tON = 0s \tState: ON  \t");
		fflush(stdout);
		wait_ms(0, testStatus);

		for (int i = 1; i <= (timeOFF_max - timeOFF_start)/timeOFF_step + 1; i++)	
		{
			digitalWrite(relayPin, HIGH); 	// Turn ON
			printf("\rRound %d: \tOFF = %dms \tON = %ds \tState: ON  \t", i, timeOFF_start + ((i-1)*timeOFF_step), timeON);
			fflush(stdout);

			wait_ms(timeON * 1000, testStatus);

			digitalWrite(relayPin, LOW); 	// Turn OFF
			printf("\rRound %d: \tOFF = %dms \tON = %ds \tState: OFF \t", i, timeOFF_start + ((i - 1)*timeOFF_step), timeON);
			fflush(stdout);

			wait_ms(timeOFF_start + ((i - 1)*timeOFF_step), testStatus);
		}

		digitalWrite(relayPin, HIGH);
		printf("\nPost test: \t\t\tState: ON\n");
		wait_ms(0, testStatus);

		printf("Cycle %d is finished! \n", n);
	}
	return;
}
