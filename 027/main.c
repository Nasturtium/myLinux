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

#define errExit(msg) {perror(msg);exit(EXIT_FAILURE);}

int error_arg(char *name)
{
	printf("Error! options \n");
	printf("Options:\n");
	printf("%s name_child \n",name);
	exit (0);
}

int main(int arg,char *argv[])
{
	struct utsname uts,uts_parent;
	if(arg != 2)
		error_arg(argv[0]);

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
		unshare(CLONE_NEWUTS);

    		if (sethostname(argv[1], strlen(argv[1])) == -1)
			errExit("sethostname");//printf("Error: sethostname \n");
			  
		if (uname(&uts) == -1)
        		errExit("uname");
	
		printf("nodename in child:  %s\n", uts.nodename);	
		sleep(100);
	}
	
	//parent
       	if (uname(&uts_parent) == -1)
                errExit("uname");
	printf("nodename in parent:  %s\n", uts_parent.nodename);

return 0; 
}
