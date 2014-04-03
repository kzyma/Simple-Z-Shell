/*
 * Author: Ken Zyma
 * Date: Spring 2014
 * course: csc552
 * Assignment: Process Programming Assignment: A Unix Shell
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include "parser.h"

/*
 * File kzshell.c contains the source to compile and run the kzshell. File
 * parser.h must be found in the same directory or found by path variable.
 */

void kzPrompt();
void welcomeMessage();
int interactiveMode();
int batchMode();
int processCommand(char *commands);

#define maxBuffer 255
#define quitCommand "quit"

int main(int argc, char *argv[]){

  //process to interactive mode or batch mode
  if(argc==1){
    //interactive mode
    interactiveMode();
  }else if(argc==2){
    //batch mode, assume that argv[1] is a filename to read.
    //fopen provides buffering i/o and gives the ability to use stdio functions vs.
    //using open(...)
    FILE *input = fopen( argv[1], "r" );
    if (input == NULL)
      {
	printf( "File %s failed to open. %s\n",argv[1],strerror(errno));
	return 1;
      }
    batchMode(input);
    fclose(input);
  }else{
    //display usage
    printf( "usage: %s [batchfile]\n", argv[0]);
  }
  return 0;
}

/*
 * interactiveMode() starts a kzshell in interactive mode. This mode
 * reads and executes commands via the prompt until [quit] is read.
 * *note* max size of any one LINE (command or series of commands
 * separated by [;] is 255.
 */
int interactiveMode(){
  char input[maxBuffer];
  char **args;
  char **commands;

  //display welcome message
  welcomeMessage();

  //main interactive loop
  for( ; ; ){

    kzPrompt();
    gets(input);

    //check for exit status
    if(strcmp(input,quitCommand)==0)
      return 0;

    //separate input into commands and process each (if only one command
    //put it in a argv type array at commands[0]).
    int numOfCommands = parser(input,&commands,";");
    int i;
    for (i=0;i<numOfCommands;i++){
      //fork and execute
      int retVal = processCommand(commands[i]);
      //fork failed at 1.
      if(retVal == 1){printf("Fork Failed. %s\n",strerror(errno));}
    }
  }
  return 0;
}

/*
 * batchMode() reads and exectutes commands until EOF.
 * *note* max size of any one LINE (command or series of commands
 * separated by [;] is 255.
 */
int batchMode(FILE *inputFile){
  char input[maxBuffer];
  char **commands;
  pid_t childpid = 0;

  //display welcome message
  welcomeMessage();

  //main batch loop, while file not empty...
  while(fgets(input,maxBuffer,inputFile) != NULL){
    //cleanup on input
    int inputSize = strlen(input);
    input[inputSize-1] = 0;

    //print entire line of commands
    printf("%s \n",input);

    //separate input into commands and process each (if only one command
    //put it in a argv type array at commands[0].
    int numOfCommands = parser(input,&commands,";");
    int i;
    for (i=0;i<numOfCommands;i++){
      //print command currently excecuting
      printf("%s \n",commands[i]);
      //fork and execute
      int retVal = processCommand(commands[i]);
      //fork failed at 1.
      if(retVal == 1){printf("Fork Failed. %s\n",strerror(errno));}
    }
  }
  return 0;
}

/*
 * Fork/Exec a single child process to carry out a command.
 * Returns: 0 if sucessfull.
 *          1 if fork failes (check errno).
 *          
 */
int processCommand(char *command){
  char **args;
  pid_t childpid = 0;

  //fork child process to excecute
  childpid = fork();
  
  //check for error from fork
  if(childpid == -1){
    return 1;
  }
  
  /******************** CITATION ****************************
   * Title: Advanced Unix Programming, Program example 11.1
   * Author: Kay A. Robbins and Steven Robbins
   * Date: February 22, 2014
   * Availiability: Published by Prentive Hall PRT
   *
   * Usage: Lines directly below derived from the following:
   *     if((fork==0)&&(makeargv(inbug," ",&chargv)>0))
   *        execvp(chargv[0],chargv)
   ***********************************************************/

  //if this is the child process AND a command was sent to excecute
  if((childpid == 0)&&(parser(command,&args," ") > 0)){
    //note* if filename includes slash character, PATH ignored and
    //specified pathname is excecuted. otherwise search PATH.
    //example... passing "cd" will look in /bin/cd for the new image.
    if((execvp(args[0],args))<0){
      //unsuccessful exec
      printf("Err: %s\n",strerror(errno));
      _Exit(EXIT_FAILURE);
    }
    free(args);
  }else if(childpid == 0){free(args);_Exit(EXIT_SUCCESS);} //if no arguments, exit.
  else{wait(NULL);} //block until child returns

  return 0;
}

/*
 * display the kzPrompt
 * logname>
 */
void kzPrompt(){
  //Do not really want to display an extremely large path name, 
  //so if it's bigger than 20 characters...display the last 20. 
  char entirePathName[maxBuffer];
  int i=0;
  if(getcwd(entirePathName,sizeof(entirePathName)) == NULL){perror("Pathname exceeds buffer.");}

  //Really only want to see path since username (if exists)
  char pathName[maxBuffer];
  char *logName = getenv("LOGNAME");
  char *startPathOut;
  startPathOut = strstr(entirePathName,logName);
  if(startPathOut!=NULL){
    startPathOut = startPathOut + strlen(logName);
    i=0;
    while(*startPathOut != NULL){
      pathName[i]=*startPathOut;
      startPathOut++;
      i++;
    }
    pathName[i]=0;
  }

  if(strlen(pathName)>20){
    //size includes ..., 20 chars of pathname, and null terminator
    char tempPathName[24];
    tempPathName[0]='.';
    tempPathName[1]='.';
    tempPathName[2]='.';
   
    for(i=0;i<20;i++){
      tempPathName[i+3] = pathName[strlen(pathName)-i];
    }
    tempPathName[23]=0;
    memcpy(pathName,tempPathName,strlen(tempPathName));
  }
  printf("%s::%s> ",getenv("LOGNAME"),pathName); 
}

/*
 * display welcome message. Information regarding the shell/version.
 */
void welcomeMessage(){
  //program information
  const char name[] = {"kzShell"};
  const char version[] = {"V.1.0.0"};
  const char author[] = {"Ken Zyma"};
  const char authorContact[] = {"email: zymakenneth@gmail.com"};
  //list of all sucessful compilation enviroments
  const char compilationHistory[] = {"-gcc version:Apple LLVM version 5.0 "
			             "\n \t\t\ttarget: x86_64-apple-darwin13.0.0"};

  printf("**************************************************************** \n");
  printf("* Shell: %s \n",name);
  printf("* Version: %s \n",version);
  printf("* Author: %s \n",author);
  printf("* Compilation history: %s \n",compilationHistory);
  printf("*************************************************************** \n");
  
}
