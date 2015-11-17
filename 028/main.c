#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <syslog.h>
#include <sched.h>
#include <sys/utsname.h>
#include <sys/mount.h>

#define errExit(msg) {perror(msg);exit(EXIT_FAILURE);}

int error_arg(char *name)
{
	printf("Error! options \n");
	printf("Options:\n");
	printf("%s number_mount\n",name);
	exit (0);
}

int main(int arg,char *argv[])
{
	struct utsname uts,uts_parent;
	if(arg != 2)
		error_arg(argv[0]);
	
	int count = atoi(argv[1]);

      	char name_mount[BUFSIZ];	
	//создаем в ./ директории для монтирования
	for(int i=0;i<count;i++)
	{
		sprintf(name_mount,"./my_proc%d",i);
	       	mkdir(name_mount, 0555);
	        //if (mount("proc", name_mount, "proc", 0, NULL) == -1)
	        //	errExit("mount");
        	
		//printf("Mounting %s\n", name_mount);
	}

	for(int j=0;j<count;j++)
	{
		pid_t pid = fork();     //создаем потомка
		if(pid == -1)
		{	
			//если не удалось запустить потомка	
			printf("Error: start \n");
			exit (1);
		}
		else if (pid == 0)
		{
			//если это потомок
			unshare(CLONE_NEWPID);
                	//монтируем
			sprintf(name_mount,"./my_proc%d",j);
			if (mount("proc", name_mount, "proc", 0, NULL) == -1)
				errExit("mount");

			printf("Mounting %s\n", name_mount);

			//pid_t child_pid = getpid();
			//if (waitpid(child_pid, NULL, 0) == -1)
			//            errExit("waitpid");
sleep(10);
printf("child = %d PID=%d \n",j,getpid());
		//sleep(10);
		}
	}
return 0; 
}
