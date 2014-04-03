############################# INTRO ################################
  Author: Ken Zyma
  Date: Spring 2014
  Course: csc552
  Assignment: Process Programming Assignment: A Unix Shell

  usage:
  kzshell [batchfile]

  Kzshell is a simple shell that forks and executes any existing
Unix command or user-developed programs. This shell may be run in
two modes: 1) Interactive and 2) Batch.
  1)Interactive Mode: allows user to enter commands at a prompt and
executes those commands.
  2)Batch Mode: pass a batch file from the command line into kzshell
and it will run batch mode. This mode reads from a file )until end of
file), processing each command encountered. Batch mode will first
display the line read from a file, and next the individual commands as
it executes them.

######################### COMPILATION ###############################

Running GNU’s make utility will compile and link to an executable 
named kzShell. You may also manually compile and link:
	gcc -c kzshell.c parser.c
	gcc kzshell.o parser.o -o kzShell

########################## HOW TO RUN ###############################

Run the executable kzShell with no arguments for interactive mode
or with the batch file to run batch mode.
	ex: ./kzShell
	ex2: ./kzShell test.bat


######################## DESIGN OVERVIEW ############################

ksShell follows relatively straightforward implementation, a command is
read by prompt or file, and executed by a child process using fork-exec.
The parser is the only piece that may differ quite a bit from other 
simple shell implementations. For this I wanted to ‘tokenize’ each command
myself, instead of using strtok() or better strtok_r(), building an argv
style array of char pointers (similar to the textbook example).
The algorithm is as follows:

Paser(input,output,delimeter)
1) count number of tokens in input
2) allocate memory for our ‘argv’ using the number of tokens
3) Read each token and place into argv
	3.1)for(each char in input)
		if (char is not delimiter and your not reading a token)
			start of token found
			were now reading a token
		else if(char is delimiter and we have been reading a token)
			end of token found
			add this token to the output array
		else if(char is delimiter, but were not reading a token)
			do nothing
		else
			were still reading the token, just continue.

Simple design but effective. For more information on design decitions
at a lower code level the source code is commented heavily. 

footnote:
Ideas on how to implement this project at high level design were derived from
the work in “Unix Systems Programming” by Kay A. Robbins and Steven Robbins.
Although most of the ideas expressed in this text are common knowledge, I 
annotated a citation if I felt it was nesessary. I think it’s safe to consider
‘common knowledge’ as any information that can be found by 5 or more 
reference tools. See link below for more details:

http://uark.libguides.com/content.php?pid=155080&sid=1780817

######################## OTHER (BUGS/ECT) ############################

Ambiguities in the specification:
None

Known Bugs:
1) “quit” failure
	Could not replicate this error, so if the following happens PLEASE
	try to document and report the commands sent before this happened to
	zymakenneth@gmail.com. ‘quit’ command failed to exit shell, had 
	to be executed multiple times. I would assume this is an issue 
	with the child process correctly exiting and returning id to parent.
