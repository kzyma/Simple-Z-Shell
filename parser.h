/*
 * Author: Ken Zyma
 * Date: Spring 2014
 * course: csc552
 * Assignment: Process Programming Assignment: A Unix Shell
 */

#ifndef KZSHELL_PARSER_H
#define KZSHELL_PARSER_H

#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/*
 * parse a character array and 'tokenize' into an argv. If two characters 
 * are separated by a " ", then make a new row for subsequent characters. 
 * If they are separated by a ";", start new column. Return number of
 * tokens found. (-1 is none).
 * parser(input, &output or'tokenized' array, delimeter(s))
 * note that you will need to free ***a_output. This is dynamically malloc'd
 * inside of the function.
 */
int parser(const char *a_input, char ***a_output,const char *delimeter);

/*
 * count and return the number of tokens from string using a delimeter.
 * Leading and trailing delimeter is ignored and handled appropriatly.
 */
int numberOfTokens(const char *a_input,const char*delimeter);

#endif /*KZSHELL_PARSER_H */
