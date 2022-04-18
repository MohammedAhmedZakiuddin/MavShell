#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n"      // We want to split our command line up into tokens
                                // so we need to define what delimits our tokens.
                                // In this case  white space
                                // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255    // MAXIMUM COMMAND LINE SIZE

// REQUIREMENT 7: MAV SHELL SHALL SUPPORT 10 COMMAND LINE PARAMETERS
#define MAX_NUM_ARGUMENTS 10     // MAV SHELL ONLY SUPPORTS 10 ARGUMENTS

#define MAX_HISTORY_SIZE 15    // MAXIMUM HISTORY SIZE OF 15 IS TO STORE COMMANDS IN HISTORY 

int main()
{
    int i;
    int status;
    int size = 0;
    int count = 0;
    int pidcount = 0;
    int history_pos = -1;
    int history_size = 15;


    pid_t pid; // PID REPRESENT PROCESS ID'S
    pid_t pid_history[MAX_HISTORY_SIZE]; // STORES UPTO 15 PROCESS IDS.
    char history[MAX_HISTORY_SIZE][MAX_COMMAND_SIZE]; // 2-D ARRAY TO SAVE LAST 15 COMMANDS OF MAX_COMMAND_SIZE OF 255.

    char * command_string = (char*) malloc( MAX_COMMAND_SIZE );

    for(i = 0; i < 15;i++)
    {
        memset(history[i], '\0', MAX_COMMAND_SIZE);
    }

    for(i = 0; i < 15; i++)
    {
        pid_history[i] = -1;
    }   

    // REQUIREMENT 1: PROGRAM WILL PRINT OUT THE MSH PROMPT
    while( 1 )
    {
        // PRINTS OUT THE MSH PROMPT
        printf ("msh> ");

        // Read the command from the commandline.  The
        // maximum command that will be read is MAX_COMMAND_SIZE
        // This while command will wait here until the user
        // inputs something since fgets returns NULL when there
        // is no input
        while( !fgets (command_string, MAX_COMMAND_SIZE, stdin) );

        /* Parse input */
        char *token[MAX_NUM_ARGUMENTS];

        int token_count = 0;

        strncpy(history[history_pos], command_string ,MAX_COMMAND_SIZE);
        
        // REQUIREMENT 12: SHELL SUPPORTS THE HISTORY COMMAND (!n).
        if(command_string[0] == '!')
        {
            int index = atoi(&command_string[1]);
            strncpy(command_string, history[index], MAX_COMMAND_SIZE);
        }
                                                           
        // POINTER TO POINT TO BE TOKEN
        // PARSED BY STRSEP
        char *argument_ptr;
                                                            
        char *working_string  = strdup( command_string );

        // we are going to move the working_string pointer so
        // keep track of its original value so we can deallocate
        // the correct amount at the end
        char *head_ptr = working_string;

        // TOKENIZE THE INPUT STRINGS WITH WHITESPACE USED AS THE DELIMETER
        while ( ( (argument_ptr = strsep(&working_string, WHITESPACE ) ) != NULL) &&
              (token_count < MAX_NUM_ARGUMENTS))
        {
            token[token_count] = strndup( argument_ptr, MAX_COMMAND_SIZE );
            if( strlen( token[token_count] ) == 0 )
            {
                token[token_count] = NULL;
            }
            token_count++;
        }

        // REQUIREMENT 4 & 6: PRINTS THE MSH PROMPT AFTER EACH COMMAND.
        if (token[0] == '\0')
        {
            continue;
        }

        // STRCPY COPIES USER INPUT INTO HISTORY ARRAY 
        strcpy(history[size++], command_string);

        if(size < MAX_HISTORY_SIZE)
        {
            count = size;
        }

        // STRCMP CHECKS WITH TOKEN[0] IN POSITION WITH THE USER COMMAND "HISTORY" AND PRINTS THE SAVED HISTORY ARRAY.
        if(!strcmp(token[0],"history"))
        {
            for(i = 0; i < count; i++)
            {
                printf("%d: %s", i, history[i]);
            }

            for(i = 0;i < 15; i++)
            {
                pid_history[i] = pid_history[i + 1];
            }

            continue;
        }

        // IF THE USER INPUTS EXCEED 15, IT RESETS TO 0 IN ORDER TO PRINT THE LAST 15 COMMANDS FOR HISTORY.
        if(history_size > 15) history_size = 0;
        
        // REQUIREMENT 11: SHELL SUPPORTS THE PIDHISTORY COMMAND TO LIST THE PIDS OF THE LAST 15 PROCESS.
        // STRCMP CHECKS WITH TOKEN[0] IN POSITION WITH THE USER COMMAND "PIDHISOTRY" AND PRINTS THE PIDHISTORY ARRAY.
        if(!strcmp(token[0],"pidhistory"))
        {
            for(i = 0; i < 15; i++)
            {
                if(pid_history[i] != -1)
                {
                    printf("%d: %d\n",i, pid_history[i]);
                }
                pidcount++;
            }
            continue;
        }

        // IF THE USER INPUTS EXCEED 15, IT RESETS TO 0 IN ORDER TO PRINT THE LAST 15 COMMANDS FOR PIDHISTORY.
        if(pidcount > 15) pidcount = 0;

        // CHANGES DIRECTORY THROUGH "CD" COMMAND
        int directory;
        
        // REQUIREMENT 10: SHELL SUPPORTS THE CD COMMAND TO CHANGE DIRECTORIES.
        // CHECKS FOR SHELL SUPPORT FOR CD COMMAND
        if(!strcmp (token[0], "cd"))
        {
            // CHDIR FUNCTION IS USED TO CHANGE DIRECTORY.
            directory = chdir(token[1]);
            if (directory == -1)
            {
                printf("No such directory exists %s.\n", token[1]);
                continue;
            }
            else
            {
                continue;
            }
        }

        // REQUIREMENT 5: EXIT OR QUITS THE PROGRAM
        if(!strcmp(token[0], "exit") || !strcmp(token[0], "quit"))
        {
            exit(0); // Terminates the current process.
        }

        // FORK() CREATES A NEW PROCESS
        // REQUIREMENT 9: MAV SHELL IMPLEMENTED USING FORK(), WAIT() AND EXECVP().
        pid = fork();

        // PRINTS THE HISTORY COMMAND AT POSTION 0 FOR (!n).
        if (history_pos != -1)
        {
            strcpy(token[0], history[history_pos]);
            strcat(token[0], "\0");

            // MUST RESET TO -1 TO AVOID INCRMENTING.
            history_pos = -1;
        }

        char cmd[MAX_COMMAND_SIZE];
        memset(&cmd, '\0', MAX_COMMAND_SIZE);

        // PARENT PROCESS
        if(pid != 0)
        {
            // INCREMENTS COUNTER FOR THE PID HISTORY. 
            pid_history[pidcount++] = pid;
            if(pidcount > 15) pidcount = 0;
            waitpid(pid, &status, 0);  // PARENT WAITS FOR CHILD PROCESS TO COMPLETE.
        }

        else
        {
            // "EXECVP()" GIVES CONTROLS TO THE CURRENT PROCESS
            if(pid == 0) // CHILD PROCESS
            {
                execvp(token[0],&token[0]);
                {
                    // IF -1 THE COMMAND FAILED
                    // REQUIREMENT 2: COMMAND NOT FOUND
                    while(-1)
                    {
                        printf("%s: command not found\n",*token);
                        return 0;
                    }
                }
            }
            exit(0);
        }

        free( head_ptr );

    }
    return 0;
}

