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

int flag_usr1, flag_usr2;

void sig_handler1(int s)
{
	flag_usr1++;
	openlog("stat_server", 0, LOG_USER);
	syslog(LOG_NOTICE, "Daemont working sig1 %d",flag_usr1);
	closelog();
//	if(raise(SIGTERM)==-1)
//		printf("Error: SIGTERM \n");
}

void sig_handler2(int s)
{
	flag_usr2++;
        openlog("stat_server", 0, LOG_USER);
        syslog(LOG_NOTICE, "Daemont working sig2 %d",flag_usr2);
        closelog();
}

int main(int arg,char *argv[])
{
	flag_usr1=0;
	flag_usr2=0;

	openlog("stat_server", 0, LOG_USER);
	syslog(LOG_NOTICE, "Parent is starting");
	closelog();
	printf("starting...\n");

	pid_t pid = fork();     //создаем потомка
	if(pid == -1)
	{	//если не удалось запустить потомка	
		printf("Error: start Daemon failed\n");
		exit (1);
	}
	else if(pid == 0)
	{	//если это потомок
		setsid();       //создаем новый сеанс
		chdir("/");     //идем в корень
		//закрываем дескрипторы ввода/вывода/ошибок
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		
		openlog("stat_server", 0, LOG_USER);
		syslog(LOG_NOTICE, "Daemont start working");
		closelog();
		sleep(1);
	
		


		//sig_usr1
		struct sigaction sig_usr1;
		sigemptyset(&sig_usr1.sa_mask);          //обнуляем
		sig_usr1.sa_flags = 0;                   //набор флагов
		sig_usr1.sa_handler = sig_handler1;      //функция-обработчик
		if(sigaction(SIGUSR1,&sig_usr1,NULL)==-1)       //10=SIGUSR1
		{
			printf("Error: sigaction() sig1 \n");
			return 1;
		}
		//sig_usr2
		struct sigaction sig_usr2;
		sigemptyset(&sig_usr2.sa_mask);    
		sig_usr2.sa_flags = 0;             
		sig_usr2.sa_handler = sig_handler2;
		if(sigaction(SIGUSR2,&sig_usr2,NULL)==-1)       //12=SIGUSR2
		{
			printf("Error: sigaction() sig2 \n");
			return 1;
		} 
	
		while(1);
	} 
	else	//parent
		printf("bay....\n");
return 0; 
}
