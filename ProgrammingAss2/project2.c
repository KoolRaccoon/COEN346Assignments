/*
 * Project 2: Shell Interface and Multithreading
 * Felix Adrian Lucaciu	27397941
 * Michael Magnabosco 	27189737
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

//80 chars per line, per command, should be enough.
#define MAX_LINE 80
//Size of history
#define MAX_COMMANDS 10

char history[MAX_COMMANDS][MAX_LINE];
char display_history [MAX_COMMANDS][MAX_LINE];

int command_count = 0;

int main(void) {
	//Buffer to hold the command entered 
	char inputBuffer[MAX_LINE]; 
	//1 if a command is followed by &
	int background;             	
	//Command line has max 40 arguments
	char *args[MAX_LINE/2 + 1];	
	//Child process 
	pid_t child;   		
	int status;           
	int shouldrun = 1;
	
	int i, upper;
		
	//Program terminates normally inside setup
    	while (shouldrun) {
		background = 0;
		
		//Get next command
		shouldrun = setup(inputBuffer,args,&background);
		
		if (strncmp(inputBuffer, "exit", 4) == 0)
			return 0;
		else if (strncmp(inputBuffer,"history", 7) == 0) {
			if (command_count < MAX_COMMANDS)
				upper = command_count;
			else 
				upper = MAX_COMMANDS;
			
			for (i = 0; i < upper; i++) {
				printf("%d \t %s\n", i, display_history[i]);
			}
			
			continue;
		}

				
		if (shouldrun) {
			//Creates a duplicate process
			child = fork();
			switch (child) {
				case -1: 
					perror("could not fork the process");
					break; 
				
				case 0: 
					//This is the child process
					status = execvp(args[0],args);
					if (status != 0){
						perror("error in execvp");
						//Terminate this process with error code -2
						exit(-2); 
					}
					break;
				
				default :  
					//Handle parent, wait for child
					if (background == 0) 
						while (child != wait(NULL)) 
							;
			}
		}
    }
	
	return 0;
}

//Function to add command to history
void addtohistory(char inputBuffer[]) {
	int i = 0;
	
	//Adds command to history
	strcpy(history[command_count % MAX_COMMANDS], inputBuffer);
	
	//Builds the display style of history
	while (inputBuffer[i] != '\n' && inputBuffer[i] != '\0') {
		display_history[command_count % MAX_COMMANDS][i] = inputBuffer[i];
		i++;
	}
	display_history[command_count % MAX_COMMANDS][i] = '\0';
	
	++command_count;

	return;
}

//reads command line arguments
int setup(char inputBuffer[], char *args[],int *background) {
	//Number of characters in the command line
    	int length,
	//Index for inputBuffer array	
	i,
	//Starting index of next parameter	
	start,	
	//Index of next parameter in args		
	ct,			
	command_number;	
    
    ct = 0;
	
    	//Read command line
	do {
		printf("osh>");
		fflush(stdout);
		length = read(STDIN_FILENO,inputBuffer,MAX_LINE); 
	}

	while (inputBuffer[0] == '\n'); 
	
    start = -1;
    if (length == 0) 
        exit(0);            
	
	
	//If process is in read,  returns -1	
    	if ( (length < 0) && (errno != EINTR) ) {
		perror("error reading the command");
		exit(-1);           
    	}
	
	//Check if using history	
	if (inputBuffer[0] == '!') {
		if (command_count == 0) {
			printf("No history\n");
			return 1;
		}
		else if (inputBuffer[1] == '!') {
			//Do the previous command
			strcpy(inputBuffer,history[(command_count - 1) % MAX_COMMANDS]);
			length = strlen(inputBuffer) + 1;
		}
		//Do the nth command
		else if (isdigit(inputBuffer[1])) { 
			command_number = atoi(&inputBuffer[1]);
			strcpy(inputBuffer,history[command_number]);
			length = strlen(inputBuffer) + 1;
		}
	}
	
	
	//Adds command to history
	addtohistory(inputBuffer);
	
	//Parses contents of inputBuffer
    	for (i=0;i<length;i++) { 
		//Check every character in the inputBuffer		
        	switch (inputBuffer[i]){
			case ' ':
			case '\t' :               
				if(start != -1){
              		args[ct] = &inputBuffer[start];    
		    		ct++;
				}
				inputBuffer[i] = '\0'; 
				start = -1;
				break;
			case '\n':                 
				if (start != -1){
					args[ct] = &inputBuffer[start];     
		    		ct++;
				}
				inputBuffer[i] = '\0';
				//No more arguments
				args[ct] = NULL; 
				break;		
	    	default :             
				if (start == -1)
		    		start = i;
              	if (inputBuffer[i] == '&') {
	    			*background  = 1;
                   	inputBuffer[i-1] = '\0';
				}
		}
	}
	
	//If background, don't add to args array
	if (*background)
		args[--ct] = NULL;
	//Just in case the input is more than 80 
	args[ct] = NULL; 
	
	return 1;	
}
