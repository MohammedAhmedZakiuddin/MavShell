# Mav Shell

This shell works similiar to bourne shell (bash), c-shell (csh) or korn shell (ksh). 
This program (msh.c) accepts commands, forks a child process and execute those commands. 
It works like csh or bash, which will run and accept commands until the user exits the shell.
 
Commands to run:
./msh.c or gcc msh.c <br />
a.out <br />

Supports upto 10 command line parameters only. <br />
* ls - list the files or folders inside the directory. <br />
* cd - changes the directory to the provided filename. <br />
* cd .. - brings back one directory. <br />
* pidhistory or showpids = prints the last 15 commands spawned by the user. <br />
* mkdir - creates a new directory. <br />
* history - command prints the last 15 user inputs. <br />
* !n - where n is the number between 0 and 14 which will re-run the nth command. <br />
* Error - Prints command not in hisotry or command not found. <br />
* quit or exit - to end the program. <br />

