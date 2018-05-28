#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>

int main()
{
    int pipe1[2];
    int ret = pipe(pipe1);
    if (ret < 0)
    {
        printf("failed to create pipe\n");
        exit(EXIT_FAILURE);
    }
    
    pid_t pid = fork();
    
    int i = 1;
    
    if(pid)
    {
        close(pipe1[0]);
        while(i<101)
        {
            write(pipe1[1],&i,sizeof(i));
            i++;
            usleep(500000);
        }
        close(pipe1[1]);
        
    }else{
        close(pipe1[1]);
        while(1)
        {
            int a=0;
            read(pipe1[0],&a,sizeof(a));
            if(a>100) break;
            printf("%d\n",a);
            usleep(500000);
        }
        close(pipe1[0]);
    }
    
    
}
