#include <stdio.h>
#include <windows.h>
char *a[]={"If by life you were deceived\n", 
"Don't be dismal, don't be wild\n", 
"In the day of grief, be mild\n",
"Merry days will come, believe\n",
"Heart is living in tomorrow\n", 
"Present is dejected here\n",
"In a moment, passes sorrow\n", 
"That which passes will be dear\n"}; 

int main()
{
	for (int i =0 ; i<8;i++)
	{ 
		printf("%s",a[i]);
		//Sleep(1000);
	}
}
