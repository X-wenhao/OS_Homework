#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main ()
{
    printf("create parent process whose PID=%d\n",getpid());

    pid_t pid=fork();

    if(pid==0)
    {
        printf("create child process whose PID=%d\n",getpid());
    }
}
