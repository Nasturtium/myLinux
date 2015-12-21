#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/ioctl.h>

int main(int arg,char **argv)
{

//	if(arg == 1)   //если пользователь ничего не ввел
//	{
//		printf("ERROR! number arg is NULL! \n");
//		exit(1);
//	}

	typedef struct
	{
		int num_open;		//statistic
		int num_release;
		int num_read;
		int num_write;
		int num_seek;
		int num_ioctl;
	} STATISTIC_CHARDEV;	
	STATISTIC_CHARDEV statistic;
	
	char buf[BUFSIZ];
	int size = 0;

	//FILE *file;
	int file;
	file = open("/dev/kbuf",O_RDWR);
	if(file < 0)
	{
		printf("ERROR open file  \n");
		//проверьте, что уст-во есть в списке
		//cat /proc/devices
		exit(1);
	}

	//проверяем функции
	//write
	printf("function write() \n");
        sprintf(buf, "%s", "test\n");
        size = strlen(buf);
        if( write(file, buf, size) < 0)
	{
		printf("ERROR function write() \n");
		exit(1);
	}
	//read
	printf("function read() \n");
	if(read(file,buf,BUFSIZ) < 0)
	{
                printf("ERROR function read() \n");
                exit(1);
	}
	//ioctl
	ioctl(fd,0x456,&statistic);
	printf("function ioctl() for statistic \n");
	printf("num_open = %d \n",statistic.num_open);
	printf("num_read = %d \n",statistic.num_read);
	printf("num_write = %d \n",statistic.num_write);
        printf("num_seek = %d \n",statistic.num_seek);
        printf("num_ioctl = %d \n",statistic.num_ioctl);
        printf("num_release = %d \n",statistic.num_release);

	close(fd);	
	return 0;
}

