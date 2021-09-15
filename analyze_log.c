#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char getlog()
{
	//Open minicom in terminal to take log
	//char file_location[1000];
	//char command[1000] = "minicom -b 115200 -D /dev/ttyUSB3 -C ";
	//char* complete_command;
	
	//printf("File Location or File name: ");
	//scanf("%[^\n]",file_location);
	//complete_command = strcat(command, file_location);
	//system(complete_command);
	
	char temp;
	char word[1000];
	char line[1000000];
	
	
	  
	while(1)
	{
		FILE *fp;
	    fp = fopen("test_log.txt","r");
	    if(fp == NULL) 
	     {
		  exit(1);
	     }
	  
		int count = 0;
		int line_num = 1;
		
		//Get Word From USER
		printf("Please Enter The Word That You Want To Find: ");
		scanf("%c", &temp);
		scanf("%[^\n]", word);
		
		//While Loop To Get And Compare Every Line In The File
		while(fgets(line,999999,fp)!= NULL)
		{
			char* p = strstr(line,word);
			int q = strlen(line);
			int r = strlen(word);
			int i, j, k;
			
			if(p)
			{
				for(i = 0; i < q; i++)
				{
					if(line[i] == word[0])
					{
						k = 0;
						
						for(j = 0; j < r; j++)
						{
							if(line[i + j] == word[j])
							{
								k++;
							}
						}
					
						if(k == r)
						{
							count++;
							//printf("%d ", line_num);
						}
					
					}
					
				}
			}
			
			line_num ++;
		}
	
		//Print Out The Result
		printf("\n\n\rTotal: %d '%s' found!\n\n", count, word);
		fflush(stdout);
		
			//Close The File
	fclose(fp);
	
	return 0;
	}


	
	
}
