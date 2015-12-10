#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int arg,char **argv)	//arg - кол-во аргументов
{				//argv - ук на аргумент
//	if(arg == 1)   //если пользователь ничего не ввел
//	{
//		printf("ERROR! number arg is NULL! \n");
//		exit(1);
//	}


	FILE *fd;
	fd = fopen("","a+");
	if(fd == 0)
		printf("ERROR open file  \n");
	else
	{
		fd = open("/dev/kbuf",..);
//		ioctl(fd,1231,buf);

	}

  

	return 0;
}

