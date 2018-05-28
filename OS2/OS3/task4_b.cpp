#include <stdio.h>
#include <stdlib.h>
#define BUF_SIZE 1024

int read_line(char str[])
{
    int ch, i = 0;
    while ((ch = getchar()) != '\n')
        if (i < BUF_SIZE)
            str[i++] = ch;
    str[i] = '\0';
    return i;
}

int main()
{
	FILE *fp;
	fp = fopen("data","w");
	if(fp==NULL)
	{
		printf("failed to open file\n");
		exit(EXIT_FAILURE);
	}
	char buffer[BUF_SIZE];
	for(int k=0;k<8;k++)
	{
		read_line(buffer);
		int i =0;
		while(buffer[i]!='\0')
		{
			if(buffer[i]>='a' && buffer[i]<='z')
			{
				buffer[i]-=32;
			}
			i++;
		}
		buffer[i]='\n';
		fwrite(buffer,1,i+1,fp);
	}
	fclose(fp);
}
