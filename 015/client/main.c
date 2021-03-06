#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>	//socket
#include <sys/socket.h>	//socket
#include <sys/un.h>
#include <netinet/in.h>
#include <unistd.h>

void menu()
{
     printf("\nmenu:\n");
	printf("   'msgNx' - вывод последних сообщений (где х - кол-во сообщений) \n");
	printf("   'exit' - отключение от сервера \n\n");
}


int main(int arg,char **argv)
{
	if(arg != 3)
	{
		fprintf(stderr,"ERROR: number arg is NULL! \n");
		printf("./myclient IP_server port_name \n");
		exit(1);
  	}
  	else
  	{
        
		menu();
		char message[BUFSIZ];
		char message_serv[BUFSIZ];

		//создаем сокет
 		int sock = socket(AF_INET,SOCK_STREAM,0);
		if(sock == -1)
		{
			fprintf(stderr,"error create socket() \n");
			exit(1);
		}
        	
		struct sockaddr_in addr;
		char *hostinfo;
		hostinfo = argv[1];		//ip server
		int my_port = atoi(argv[2]);		//port

		memset(&addr, 0, sizeof(sockaddr_in));		

		//определяем параметры соединения c сервером
		addr.sin_family = AF_INET;			//домены internet
		addr.sin_port = htons(my_port);			//порт
//		addr.sin_addr.s_addr = inet_addr(hostinfo);	//
		addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);	//для проверки на одной машине
		int cnt = 0;
		//установка соединения
		while(connect(sock,(struct sockaddr*)&addr,sizeof(addr))<0)
		{
			if(cnt == 0)
				printf("\nподключение к серверу . .\n");
			cnt=1;
		}

printf(" + Подключен к серверу \n");

		while(1)
		{
			memset(&message,0,sizeof(message));

			printf(" -> ");
			scanf("%s",message);

			//отправляем сообщение на сервер
			send(sock,message,sizeof(message),0);
			//если сообщение - exit
                        if(strcmp(message,"exit") == 0)
                        {
                                printf("отключение от сервера . . . \n");
                                close(sock);
				sleep(1);
				printf("- Отключен \n");
                                exit(1);
                        }

			int bytes = recv(sock,message_serv,sizeof(message_serv),0);
			printf("%s",message_serv);
		}


	}	
return 0; 
}
