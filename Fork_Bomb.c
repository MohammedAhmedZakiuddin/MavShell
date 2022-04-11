#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    while(1)
    {
        pid_t pid = fork();

        if(pid == -1)
        {
            perror("Fork failed\n");
        }
    }
    return 0;
}   