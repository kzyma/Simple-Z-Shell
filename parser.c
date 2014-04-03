/*
 * Author: Ken Zyma
 * Date: Spring 2014
 * course: csc552
 * Assignment: Process Programming Assignment: A Unix Shell
 */

#include "parser.h"

//making a 'bool type' makes the code a lot cleaner to read...
typedef enum { false, true } bool;

//see parser.h
int parser(const char *a_input, char ***a_output,const char *delimeter){
  /*1) find number of tokens in a_input, so that we can allocate memory 
   * on the heap for a_output.
   *2) fill an 'argv' style array with the tokens from a_input.
   */
  //index in for loop...allows for compilation without c99 mode
  int i;

  //count # of tokens
  int tokCount = numberOfTokens(a_input,delimeter);

  //alloc space for our 'argv'
  *a_output = malloc((tokCount+1)*sizeof(char *));

  //get each token and place into 'argv'
  int argvIndex = 0;
  int tokenStart=0;
  int tokenEnd=0;
  bool readingToken = false;
  for(i=0;i<strlen(a_input);i++){
    if((a_input[i]!=*delimeter)&&(readingToken==false)){
      tokenStart = i;
      readingToken = true;
    }else if((a_input[i]==*delimeter)&&(readingToken==true)){
      tokenEnd = i-1;
      //if we found a token end, lets copy that into 'argv'.
      int tokenSize = tokenEnd - tokenStart+1;
      char *buffer = malloc((tokenSize+1)*sizeof(char *));
      strncpy(buffer,&a_input[tokenStart],tokenSize);
      buffer[tokenSize+1] = 0;
      *(*a_output+(argvIndex)) = buffer;
      argvIndex++;
      readingToken = false;
    }else if(a_input[i]==*delimeter){
      readingToken = false;
    }else{
      readingToken = true;
    }
  }
  //if loop exits reading a token, it will miss the last one, need to manually add this.
  if(readingToken==true){
    tokenEnd = strlen(a_input)-1;
    int tokenSize = tokenEnd - tokenStart+1;
    char *buffer = malloc((tokenSize+1)*sizeof(char *));
    strncpy(buffer,&a_input[tokenStart],tokenSize);
    buffer[tokenSize+1] = 0;
    *(*a_output+(argvIndex)) = buffer;
  }
  
  //need to append NULL to end.
  *(*a_output+(tokCount)) = NULL;
  return tokCount;
}

//see parser.h
int numberOfTokens(const char *a_input,const char*delimeter){
  int tokCount = 0;
  bool readingToken = false;
  //index in for loop...allows for compilation without c99 mode
  int i;
  for(i=0;i<strlen(a_input);i++){
    if (((a_input[i]!=*delimeter)||(strcmp(&a_input[i]," ")==0)) && (readingToken==false)){
      tokCount++;
      readingToken = true;
    }else if(a_input[i]==*delimeter){
      readingToken = false;
    }else{
      readingToken = true;
    }
  }
  return tokCount;
}
