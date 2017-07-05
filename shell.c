/*
* This is a C program to serve as a shell interface that accepts user commands,then executeeach command in a separate process.
* In addition, provide a history feature that allows the user to access the most recently entered commands.
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define MAX_LINE 80 //maximum lenght command

//method that will be used to run the last command in the history
char  *LastCommand(char* array[],int c){
  int num = c-1;
  char* command = array[num];
  return command;
}

//method that will be used to run a specific command from history
char *specCommand(char* array[],int count,int index){
  int i = 10;;
  char* command;
  command= array[index];
  return command;
}

int main(void){
  char *args[MAX_LINE/2+1]; //command line arguments
  char *historyCommands[MAX_LINE/2+1]; //command line history
  char *lastCommand; //storing the las command
  char *storeCommand; //storing the command in history
  int should_run=1; //flag for exiting program
  char input[MAX_LINE];
  int argsCounter = 0;
  pid_t pid;
  int amp=0; //& doesn't exist at the end
  int histCounter = 0;
  char* save_command;
  char* res_command;

  //main loop
  while(should_run){
    
    printf("CSCI3120>");
    fflush(stdout);
    wait (0);
    input[0]= '\0';
    amp=0;
    argsCounter=0;

    //take input from user
    fgets (input, MAX_LINE, stdin);
    strtok(input, "\n");

    //if the input is exit then set should_run to 0 so that it quits the loop
    if(strcmp(input,"exit") == 0){
      should_run = 0;
      break;
    }

    //tokenizing the input using spaces and storing the values in the args array
    char *p = strtok(input, " ");
    while(p != NULL) {
      args[argsCounter] = p;
      p = strtok(NULL, " ");
      argsCounter++;
    }
    args[argsCounter]= NULL;
    
    //checking for the history
    storeCommand = input; 
    if(strcmp(storeCommand,"exit")==0)
      break;
    if((strcmp(storeCommand,"history") != 0 && storeCommand[0] != '!')){
      historyCommands[histCounter] = strdup(storeCommand);
      histCounter++;
    }
    else if(strcmp(storeCommand,"history") == 0){
      //if the history is empty print error else print the history
      if(histCounter == 0){
        printf("No commands in history.\n");
        continue;
      }
      else{
    	  //print history here
    	  int i = 10, count = histCounter;
    	  //printing history when less than 10
    	  if(count <= 10){
          while(count != 0){
            printf("%d %s\n",count,historyCommands[count-1]);
            i++;
    		    if(i == 10)
    		      break;
    		    count--;
    		  }
  	    }
    	  //bigger than 10
    	  else{
    	      while(count != 0){
    		      printf("%d %s\n",count,historyCommands[count-1]);
              i--;
    	        if(i == 0)
    		        break;
              count--;
    	      }
        }
      }
    }
    

    //dealing with !! that's runing the last command
    save_command= input;
    if((strcmp(save_command,"!!") == 0)){
      if (histCounter == 0){
        printf("No commands in history\n");
        continue;
      }
      res_command = strdup(LastCommand(historyCommands,histCounter));
      historyCommands[histCounter++] = strdup(res_command);
      strcpy(input,res_command);
    }
    //dealing with !k that's runing the Kth command
    else if(save_command[0] == '!'){       
    	int index = save_command[1] - '0', i=2, CL=strlen(save_command)-1;
    	char ind=save_command[1];
    	int ne;
    	int val;

      //tokenizing the string to get the full numbers
    	char  *p = input;
    	while (*p) { 
    	  if (isdigit(*p)) { 
    	    val = strtol(p, &p, 10); 
    	    printf("%d\n", val); 
    	  } 
        else { 
    	    p++;
    	  }
    	}
      index=val-1;

      if (histCounter == 0 || index > histCounter){
        printf("No command in history\n");
	      continue;
	    }

      res_command = strdup(specCommand(historyCommands,histCounter,index));
      historyCommands[histCounter++] = strdup(res_command);
      strcpy(input,res_command);
    }

    //checking for & at the end of the line
    if((strcmp(args[argsCounter-1],"&") == 0)){
      amp = 1;
      args[argsCounter-1] = NULL;
    }

    //****Creating Process****//
    if(strcmp(args[0],"exit") == 0){
      break;
    }

    pid = fork();
    //child process pid==0; pid<0 fork failed; else parent process
    if(pid == 0){
      //Start of Child Process 
      if(strcmp(storeCommand,"exit") == 0){
        break;
      }

     if(strcmp(storeCommand,"history") == 0 || strcmp(storeCommand,"!!") == 0 || (storeCommand[0] == '!')){
        execvp(args[0], args);
     }
     if(execvp(args[0], args) == 0){
        execvp(args[0], args);
      }
      else{
	     exit(1);
      }

    } 
    else if(pid < 0){
      //fork failed
      if(strcmp(input,"exit") == 0){
        break;
        exit(1);
      }
      printf("Fork Failed");
      exit(1);
      break;
    }
    else{
      //parent process
      if(strcmp(storeCommand,"exit") == 0){
        break;
      }
      
      //if command ends with & then wait
      if(amp == 0){
        wait(0);
      }

    }

  }

  return 0;
}
