#include <stdio.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#define TEXT_EXIT 101
typedef struct data{
    int is_written;
    int text;
} DATA;

int main ()
{
   int shm_id = shmget(IPC_PRIVATE,sizeof(DATA),066|IPC_CREAT);
    if(shm_id==-1)
    {
        printf("failed to create shared memory\n");
        exit(EXIT_FAILURE);
    }
    void *shm = shmat(shm_id,NULL,0);
    if (shm == (void*)-1)
    {
        printf("failed to attach shared memory\n");
        exit(EXIT_FAILURE);
    }
    DATA* shared = (DATA *)shm;
    shared -> is_written = 0;
    shared -> text = 0;
    
    printf("%d\n",shared->text);
    
    pid_t pid=fork();
    if (pid)
    {
        while(shared -> text != TEXT_EXIT)
        {
            if(shared->is_written ==0)
            {
                shared->text++;
                shared->is_written=1;
                if(shared->text==TEXT_EXIT)
                {
                    break;
                }
            }else{
                usleep(500000);
            }
       }
    }else{
        while(shared -> text != TEXT_EXIT)
        {
            if(shared->is_written ==1)
            {
                if(shared->text==TEXT_EXIT)
                {
                    break;
                }
                printf("%d\n",shared->text);
                shared->is_written=0;
            }else{
                usleep(500000);
            }
       }
    }
}
