#include <stdio.h> 
#include <stdlib.h>
//#include <unistd.h>
#include <pthread.h>  
#include <wiringPi.h> 	// Include WiringPi library!
//#include "getch.c"
#include "scanKey.c"
#include "sendIR.c"

#define RUN 1
#define PAUSE 0
#define STOP -1
#define YES 1
#define NO 0
#define RCOFF 15000	// milliseconds
#define RCON 30000	// milliseconds
#define timeOFF 5000	//milliseconds
#define timeON 100 //milisecond
#define POWER_OFF 175
#define POWER_ON 174
#define POWER_Toggle 149

const int relayPin = 17; 	// Set GPIO pin 17
const int IRPin = 22; // Set GPIO pin 22
void seqA_AC (int test_cycle, status *testStatus);
int cmd_type;

int main (void)
{
	int input_check, test_cycle = 0;
	status testStatus = {NO, RUN};
	pthread_t thread_scanKey;
    wiringPiSetupGpio(); 			// Initialize wiringPi
    pinMode(relayPin, OUTPUT); 		// Set pin as output  
    pinMode(IRPin, OUTPUT); 
    
    printf("Custom Test is running! Press CTRL+C to quit. \n\n");
    do {
		printf("Number of cycles: ");
		fflush(stdout);
		input_check = scanf("%d", &test_cycle);
		while(getchar() != '\n');
		//fflush(stdin);
		if (input_check == 1 && test_cycle > 0)
			printf("OK. Test cycle: %d \n", test_cycle);
		else if (input_check == 1 && test_cycle <= 0)
			printf("Number entered is invalid. \n");
		else 
			printf("Wrong input. \n");
	}while(input_check == 0 || test_cycle <= 0);	// do-while loop to check valid total cycles that need to be performed
	
	do {
		printf("Command type ([1]On-Off, [2]Toggle): ");
		fflush(stdout);
		input_check = scanf("%d", &cmd_type);
		while(getchar() != '\n');
		//fflush(stdin);
		if (input_check == 1 && (cmd_type == 1 || cmd_type == 2))
			printf("OK. RC use %s type command. \n", (cmd_type == 1)?"On-Off":"Toggle");
		else 
			printf("Invalid input. \n");
	}while(input_check == 0 || (cmd_type != 1 && cmd_type != 2));
	
	testStatus.runTest = YES;
	testStatus.state = RUN;
	pthread_create(&thread_scanKey, NULL, (void*) scan_key, &testStatus);
	printf("Test will start. Use SPACE or P keys to pause test. \n");
	//printf("To stop test, press Q. \n");
	seqA_AC(test_cycle, &testStatus);
	
	testStatus.runTest = NO;
    printf("\nTest is %s. Press any key to exit..\n", (testStatus.state == STOP)?"stopped":"completed");
    pthread_join(thread_scanKey, NULL);
    getch();
}

void seqA_AC (int test_cycle, status *testStatus)
{
	for (int n=1; n<=test_cycle; n++)
    {
		printf("\nCycle %d starts! \n", n);
		
		digitalWrite(relayPin, HIGH); 		// Turn ON
		printf("\rRound 0: \t\t\tON = 60s \tState: ON  \t");
		fflush(stdout);
		wait_ms(60000, testStatus);
		if (testStatus->state == STOP)
			return;

		for (int i=1; i<=100; i++)	
		{

           sendIR_cmd(IRPin, POWER_OFF);   //RC OFF
		   printf("\rRound 0: \t\t\tOFF = 5min \tState: RC OFF  \t");
		   fflush(stdout);
		   wait_ms(300000, testStatus);
		   if (testStatus->state == STOP)
			return;
			
	      // digitalWrite(relayPin, LOW); 		// Turn OFF
		  // printf("\rRound 0: \t\t\tOFF = 10s \tState: AC OFF  \t");
		   //fflush(stdout);
		   //wait_ms(10000, testStatus);
		   //if (testStatus->state == STOP)
			//return;
			
			//digitalWrite(relayPin, HIGH); 		// Turn ON
		   //printf("\rRound 0: \t\t\tON = 30s \tState: AC ON  \t");
		   //fflush(stdout);
		   //wait_ms(30000, testStatus);
		   //if (testStatus->state == STOP)
			//return;
		
		   sendIR_cmd(IRPin, POWER_Toggle);    //RC ON
		   printf("\rRound 0: \t\t\tON = 5min \tState: RC ON  \t");
		   fflush(stdout);
		   wait_ms(300000, testStatus);
		   //system("./camera.sh");
		   if (testStatus->state == STOP)
			return;
		}

		digitalWrite(relayPin, HIGH);
		printf("\nPost test: \t\t\tState: ON\n");
		wait_ms(9.5*1000, testStatus);
		//delay(9.5*1000);
		if (testStatus->state == STOP)
			return;
		printf("Cycle %d is finished! \n", n);
	}
	return;
}
