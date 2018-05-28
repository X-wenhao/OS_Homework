#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
void *count1(void *ptr)
{
    for (int i=0;i<1000;i++)
    {
        printf("%d \n",i+1);
        usleep(500000);
    }
}

void *count2(void *ptr)
{
    for (int i=1000;i>0;i--)
    {
        printf("%d \n",i);
        usleep(500000);
    }
}

int main ()
{
    pthread_t ntid1,ntid2;
    pthread_create(&ntid1,NULL,count1,"TW1");

    pthread_create(&ntid2,NULL,count2,"TW2");
    pthread_join(ntid1,NULL);
    pthread_join(ntid2,NULL);

}
