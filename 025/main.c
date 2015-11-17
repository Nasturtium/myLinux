#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>	//socket
#include <sys/socket.h>	//socket
#include <sys/un.h>
#include <netinet/in.h>
#include <pthread.h>

#define errExit(msg) {perror(msg);exit(EXIT_FAILURE);}

int sock;
int localport, remoteport;	//local - сушаем, remote - отправляем

/*
void * func_thread(void *arg)
{
	struct sockaddr_in addr;
	char buf[BUFSIZ];

	memset(&addr, 0, sizeof(addr));
	//определяем параметры соединения
	addr.sin_family = AF_INET;
	addr.sin_port = htons(localport);		//отправляем через этот порт
	addr.sin_addr.s_addr=htonl(INADDR_LOOPBACK);    //127.0.0.1

	while(1)
	{
		if(scanf("%s",buf) > 0)
		{
			//printf("послыаем buf = %s port = %d \n",buf,localport);		
               		int send = sendto(sock,buf,strlen(buf),0,(struct sockaddr *)&addr,sizeof(addr));
			if(send == -1)
				fprintf(stderr,"ERROR: sendto() thread \n");
		}
	}
}
*/
int main(int arg,char **argv)
{
//	thread_t thread;
	char buf[BUFSIZ];

	struct sockaddr_in addr;

	/*
	if(arg == 1)   //если пользователь не ввел аргументы
	{
		fprintf(stderr,"ERROR: number arg is NULL! \n");
		printf("1 copy: ./udpchat name_1 name_2 \n");
		printf("2 copy: ./udpchat name_2 name_1 \n");
		//name is - localport/remoteport
		exit(1);
  	}
	*/
//		localport = atoi(argv[1]);
//		remoteport = atoi(argv[2]);

//printf("in = %d out = %d \n",localport,remoteport);

		//создаем сокет
 		sock = socket(AF_INET,SOCK_RAW,IPPROTO_ICMP);
		if(sock == -1)
			perror("socket");
		        	
		memset(&addr, 0, sizeof(sockaddr_in));		
		//определяем параметры соединения
		addr.sin_family = AF_INET;
		addr.sin_port = htons(remoteport);		//слушаем порт
		addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);	//inet_addr("127.0.0.1")
		//назначаем адрес
		int b = bind(sock,(struct sockaddr*)&addr,sizeof(addr));
		if(b == -1)
		{
			fprintf(stderr,"ERROR: bind() \n");
			exit(1);
		}
/*
		//запускаем потоковую фукцию
		if(pthread_create(&thread,NULL,&func_thread,NULL) != 0)    //&my_fifo
        	{
			fprintf(stderr,"ERROR: pthread_create() ! \n");
			exit(1);
        	}

		while(1)
		{
			//printf("recv >\n");
			memset(&buf,0,sizeof(buf));
			int recv = recvfrom(sock,buf,sizeof(buf),0,NULL,NULL);
			if(recv == -1)
				fprintf(stderr,"ERROR: recvfrom() \n");
			printf("%s\n",buf);
		}
*/		
return 0; 
}
