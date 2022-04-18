# Mav Shell

This shell works similiar to bourne shell (bash), c-shell (csh) or korn shell (ksh) 
msh.c accepts commands, forks a child process and execute those commands.
The shell, like csh or bash, will run and accept commands until the user exits the shell.
Supports upto 10 command line parameters only. 

Commands to run:
./msh.c or gcc msh.c & a.out <br />
ls - list the files or folders inside the directory.
cd - changes the directory to the provided filename.
cd .. - brings back one directory.
pidhistory or showpids = prints the last 15 commands spawned by the user.
mkdir - creates a new directory.
history - command prints the last 15 user inputs. 
!n - where n is the number between 0 and 14 which will re-run the nth command.
Error = Prints command not in hisotry or command not found.
quit or exit - to end the program.
