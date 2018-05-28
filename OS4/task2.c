/*
 * 获取程序中变量的物理地址
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
int main(int argc,char * argv[])
{
    char path[40];
    memset(path,40,0);
    sprintf(path,"%s%d%s","/proc/",getpid(),"/pagemap");
    //printf("%s\n",path);

    int fd = open(path,O_RDONLY);
    int a=0;
	size_t addr =(size_t )&a;
    
    //printf("%lu\n",&a);
	//printf("%lu\n",addr);
	printf("虚拟地址:%p\n",addr);

    size_t pagesize=getpagesize();
	size_t vpageframe = (addr /pagesize) ;
    size_t voffset = addr % pagesize;
    
    printf("虚拟页号:%p\n",vpageframe);
    printf("偏移:%p\n",voffset);

    
    lseek(fd,vpageframe*sizeof(uint64_t),SEEK_SET);
    
    uint64_t info;
    
    read(fd,&info,sizeof(uint64_t));
    
    //char s[64];
    //itoa(info,s,2);
    
    //printf("%p\n",info);
    
    size_t ppageframe = info & ((((uint64_t)1) << 55) - 1);
    
    if((info & (((uint64_t)1) << 63)) == 0)
    {
        printf("page is not present!\n");
        close(fd);
        return 0;
    }
    
    printf("物理页号:%lu\n",ppageframe);
    printf("偏移:%p\n",voffset);
    
    printf("物理地址:%p\n",ppageframe*sizeof(uint64_t)+voffset);
    
    
}

