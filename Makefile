kzshell: kzshell.o parser.o
	 gcc kzshell.o parser.o -o kzShell

kzshell.o: kzshell.c
	   gcc -c kzshell.c

parser.o: parser.c
	  gcc -c parser.c

clean:
	rm -rf *o ksShell
