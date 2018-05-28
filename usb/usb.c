/*
 * to get information of usb 
 */

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include<sys/types.h> 
#include<sys/stat.h>
#include<fcntl.h>
int main(int arg, char **argv) {

    ssize_t len;
    char buf[256], *p;
    char path[256];
    char buf3[256];
    int i;
    //获取USB的信息所在的目录
    len = readlink("/sys/block/sda", buf, 256);
    buf[len] = 0;
    sprintf(path, "%s%s", "/sys/block/", buf);
    //printf("%s\n",path);
    printf("path: %s\n", path);
    
    for (i=0; i<6; i++) {
        p = strrchr(path, '/');
        *p = 0;
    }
    
    //读取序列号
    memset(buf,0,256);
    strcpy(buf3,path);
    strcat(buf3, "/serial");
    int f = open(buf3, O_RDONLY);
    len = read(f, buf, 256);
    printf("serial: %s", buf);
    
    memset(buf3,0,256);
    memset(buf,0,256);
    strcpy(buf3,path);
    strcat(buf3, "/idProduct");
    f = open(buf3, O_RDONLY);
    len = read(f, buf, 256);
    printf("idProduct: %s", buf);
    
    memset(buf3,0,256);
    memset(buf,0,256);
    strcpy(buf3,path);
    strcat(buf3, "/idVendor");
    f = open(buf3, O_RDONLY);
    len = read(f, buf, 256);
    printf("idVendor: %s", buf);
    
    
    

}



