#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>

/*
  Call waitpid to wait on our child, then intentionally SEGFAULT the child
  so we can use the WIFSIGNALED and WTERMSIG to learn how our child died
*/

int main( void )
{
  pid_t child_pid = fork();
  int status;

  if( child_pid == 0 )
  {
    // Sleep for a second
    sleep(1);

    // Intentionally SEGFAULT the child process
    int *p = NULL;
    *p = 1;

    // We won't get here, but let's be optimistic
    exit( EXIT_SUCCESS );
  }

  // Wait for the child to exit
  waitpid( child_pid, &status, 0 );

  // See if the child was terminated by a signal
  if( WIFSIGNALED( status ) )
  {
    // Print the signal that the child terminated with 
    printf("Child returned with status %d\n", WTERMSIG( status ) );
  }

  return 0;
}