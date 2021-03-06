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
//	syslog(LOG_NOTICE, "Daemont working sig1 %d",flag_usr1);
}

void sig_handler2(int s)
{
	flag_usr2++;
//      syslog(LOG_NOTICE, "Daemont working sig2 %d",flag_usr2);
}

int main(int arg,char *argv[])
{
	flag_usr1=0;
	flag_usr2=0;

	syslog(LOG_NOTICE, "Parent is starting");
	printf("starting...\n");

	pid_t pid = fork();     //создаем потомка
	if(pid == -1)
	{	//если не удалось запустить потомка	
		printf("Error: start Daemon failed\n");
		exit (1);
	}
	else if(pid == 0)
	{
		//если это потомок
		setsid();       //создаем новый сеанс
		chdir("/");     //идем в корень
		//закрываем дескрипторы ввода/вывода/ошибок
		close(STDIN_FILENO);
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
		
		syslog(LOG_NOTICE, "Daemont start working");
		sleep(1);

                //sig_usr1
                struct sigaction sig_usr1;
                sigemptyset(&sig_usr1.sa_mask);          //обнуляем
                sig_usr1.sa_flags = 0;                   //набор флагов
                sig_usr1.sa_handler = sig_handler1;      //функция-обработчик
                if(sigaction(SIGUSR1,&sig_usr1,NULL)==-1)       //10=SIGUSR1   kill -l
                       syslog(LOG_NOTICE,"Daemont error: sigaction() sig1");
                                       
		//sig_usr2
                struct sigaction sig_usr2;
                sigemptyset(&sig_usr2.sa_mask);
                sig_usr2.sa_flags = 0;
                sig_usr2.sa_handler = sig_handler2;
                if(sigaction(SIGUSR2,&sig_usr2,NULL)==-1)       //12=SIGUSR2   kill -l
                        syslog(LOG_NOTICE,"Daemont error: sigaction() sig2");
                                
		char message[5];//BUFSIZ];	
		int sock = socket(AF_INET,SOCK_STREAM,0);
		if(sock == -1)
		{
			syslog(LOG_NOTICE,"Daemont error: create socket");
			exit(1);
		}
        	
		struct sockaddr_in serv_addr;
		int my_port = 8888;				//port
		memset(&serv_addr, 0, sizeof(sockaddr_in));		
		//определяем параметры соединения
		serv_addr.sin_family = AF_INET;			//internet
		serv_addr.sin_port = htons(my_port);		//порт
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);	//все
		//связываемся с сетевым устрйством
		if(bind(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
		{
			close(sock);
			exit(1);
		}
		listen(sock,5);		//очередь входных подключений
		syslog(LOG_NOTICE, "Daemon listen");

		while(1)
		{
			//запрос на соединение
			int sock_accept = accept(sock,NULL,NULL);	//новый сокет для работы
			if(sock_accept < 0)
				syslog(LOG_NOTICE,"Daemon error sock_accept");
			
			syslog(LOG_NOTICE,"Daemon sock conection!!!!");			
			while(1)
			{
 				memset(&message,0,BUFSIZ);
				//ожидаем сообщение от клиента
				int bytes = recv(sock_accept,message,sizeof(message),0);
				if(bytes<=0)
					break;
				//syslog(LOG_NOTICE, "Daemon message: %s",message);
				syslog(LOG_NOTICE,"waiting command: stat, usr1, usr2, exit");
				
				if(strstr(message,"stat"))
					syslog(LOG_NOTICE,"Daemon flag_usr1 = %d & flag_usr2 = %d",flag_usr1,flag_usr2);
				if(strstr(message,"usr1"))
				{
					kill(getpid(),SIGUSR1);
					syslog(LOG_NOTICE,"Daemon flag_usr1 = %d",flag_usr1);
				}
				if(strstr(message,"usr2"))
                                {
                                        kill(getpid(),SIGUSR2);
                                        syslog(LOG_NOTICE,"Daemon flag_usr2 = %d",flag_usr1);
                                }
                                if(strstr(message,"exit"))
                                {
                                        syslog(LOG_NOTICE,"Daemon exit. Socket close");
					close(sock);
                                        kill(getpid(),SIGTERM);
                                }
	 			//else
				//	syslog(LOG_NOTICE,"Daemon: command not found"); 
			}
		}

	} 
	else	//parent
		printf("bay....\n");
return 0; 
}
