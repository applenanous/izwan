#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int input_check;
	int cmd_type;
	
	
	do {
		printf("Uart Jig ([1]TL1, [2]TLH): ");
		fflush(stdout);
		input_check = scanf("%d", &cmd_type);
		while(getchar() != '\n');
		//fflush(stdin);
		if (input_check == 1 && (cmd_type == 1 || cmd_type == 2))
			printf("OK. uart use %s for log. \n", (cmd_type == 1)?"TL1":"TLH");
		else 
			printf("Invalid input. \n");
	}while(input_check == 0 || (cmd_type != 1 && cmd_type != 2));
	
	
	if (cmd_type == 1)
	{
      //Open minicom in terminal to take log
	  char file_location[1000];
	  char command[1000] = "minicom -b 115200 -D /dev/ttyUSB3 -C ";
	  char* complete_command;
	  
	  printf("File Location or File name: ");
	  scanf("%[^\n]",file_location);
	  complete_command = strcat(command, file_location);
	  system(complete_command);
  }
  else
  {
	   //Open minicom in terminal to take log
	  char file_location[1000];
	  char command[1000] = "minicom -b 115200 -D /dev/ttyUSB0 -C ";
	  char* complete_command;
	  
	  printf("File Location or File name: ");
	  scanf("%[^\n]",file_location);
	  complete_command = strcat(command, file_location);
	  system(complete_command);
  }
}
	
	

	
