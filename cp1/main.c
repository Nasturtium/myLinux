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

#define HTML_text	200
#define HTML_image	201
#define HTML_error	404

bool start_dmn = false;

void sig_handler(int s)
{
	printf("bye...\n");
	if(start_dmn)
	{
		openlog("stat_server", 0, LOG_USER);
		syslog(LOG_NOTICE, "Daemont end working");
		closelog();
	}	
	if(raise(SIGTERM)==-1)
		printf("Error: SIGTERM \n");
}

int error_arg()
{
	printf("Error! options \n");
	printf("Options:\n");
	printf("        -pXXXX port number\n");
	printf("        -d daemon\n\n");
	printf("Usage:\n./static_server -pXXXX\n");
	printf("./static_server -pXXXX -d\n\n");
	exit (0);
}
	
int main(int arg,char *argv[])
{
	int  rez = 0;					//getopt()
	//bool start_dmn = false;
	int  my_port = 0;

	while((rez = getopt(arg,argv,"p:d"))!=-1){	//перебираем все опции пока не будет -1
		switch(rez){
			case 'p':
			        my_port = atoi(optarg);
				break;
			case 'd':
				start_dmn = true;
				break;
			default:
				error_arg();
		}
	}

	if(!arg || !my_port)
		error_arg();
	
	if(start_dmn == true)
	{
        struct sigaction sig_act;
         sigemptyset(&sig_act.sa_mask);          //обнуляем
         sig_act.sa_flags = 0;                   //набор флагов
         sig_act.sa_handler = sig_handler;       //функция-обработчик
 
         if(sigaction(SIGINT,&sig_act,NULL)==-1){
                 printf("Error: sigaction() \n");
                 return 1;
         }





		pid_t pid = fork();     //создаем потомка
		if(pid == -1){  //если не удалось запустить потомка	
			printf("Error: start Daemon failed\n");
			exit (1);
		}
		else if (pid == 0){     //если это потомок
			setsid();       //создаем новый сеанс
			chdir("/");     //идем в корень
			//закрываем дескрипторы ввода/вывода/ошибок
			close(STDIN_FILENO);
			close(STDOUT_FILENO);
			close(STDERR_FILENO);
			//return stat_dmn;

			openlog("stat_server", 0, LOG_USER);
			syslog(LOG_NOTICE, "Daemont start working");
			closelog();
		}
	}

return 0; 
}
