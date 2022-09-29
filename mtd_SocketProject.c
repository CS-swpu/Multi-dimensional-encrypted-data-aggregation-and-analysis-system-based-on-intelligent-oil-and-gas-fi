//油田MTD中心端mtd_SocketProject.c文件



#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include <arpa/inet.h> 
#include <unistd.h>
#include <pthread.h>

#define MAX_DATA 100//接收到的数据最大程度 
int sockfd;/*cocket句柄和接受到连接后的句柄 */


char receiveMess[2048];
int receiveEnd;
int  receive(char * message,int len){
int tags=1;

char mess[2048];
	//memset(mess,1,2048);
	int length=0;
while(tags){
	
	int readLen=recv(sockfd,mess,2048,0);
if(readLen<=0)return -1;
        int freeLen=2048-receiveEnd;        
        if(readLen<=freeLen)
	{
	 memcpy(receiveMess+receiveEnd,mess,readLen);
         receiveEnd=readLen+receiveEnd;
        if(receiveEnd==2048){
         receiveEnd=0;
       memcpy(&length,receiveMess,4);  
       memcpy(message,receiveMess+4,length);
       tags=0;
  }

	}
	else
	{
      int cover=readLen-freeLen;
      memcpy(receiveMess+receiveEnd,mess,freeLen);
       memcpy(&length,receiveMess,4);  
       memcpy(message,receiveMess+4,length);
	memcpy(receiveMess,mess+freeLen,cover);
	receiveEnd=cover;
tags=0;

	}
        

	
 }
 
	return length;
}

int sendMess(int code,char* message, int len){
	if(len>2048)return -1;
	char mess[2048];
	len=len+4;
	memset(mess,0,2048);
	memcpy(mess,&len,4);
	memcpy(mess+4,&code,4);
	memcpy(mess+8,message,len);
	

	return send(sockfd,mess,2048,0);
	 


}	 

int closeSocket(){
	return close(sockfd);
}

int  initSocket(char * ip,int port){

if(sockfd!=-1)closeSocket();
	struct sockaddr_in dest_addr;/*目标地址信息*/
	char buf[MAX_DATA];//储存接收数据 
	memset(buf,0,MAX_DATA);
	sockfd=socket(AF_INET,SOCK_STREAM,0);/*建立socket*/
	if(sockfd==-1){
		printf("socket failed:%d",errno);
		return 1;

	}


	//参数意义见上面服务器端 
	dest_addr.sin_family=AF_INET;
	dest_addr.sin_port=htons(port);
	dest_addr.sin_addr.s_addr=inet_addr(ip);
	bzero(&(dest_addr.sin_zero),8);

	if(connect(sockfd,(struct sockaddr*)&dest_addr,sizeof(struct sockaddr))==-1){//连接方法，传入句柄，目标地址和大小 
		printf("connect failed55:%d",errno);
		return 1;
	} 
	printf("connect success");
	//recv(sockfd,buf,MAX_DATA,0);//将接收数据打入buf，参数分别是句柄，储存处，最大长度，其他信息（设为0即可）。 
	//printf("Received:%s",buf);
	/*pthread_t pt;
	 int ret_thread=pthread_create(&pt,NULL,(void *)&receive,NULL);
	 //pthread_join(pt,NULL);//wait thread end
	 if(ret_thread != 0)
	 {

		 printf("create thread faile");
		 return 1;

}


*/

	return 0;

}

 void main(){


	 int sockfd,new_fd;/*socket句柄和建立连接后的句柄
	  struct sockaddr_in my_addr;/*本方地址信息结构体，下面有具体的属性赋值
	   struct sockaddr_in their_addr;/*对方地址信息
		int sin_size;
		*  *  * 
		sockfd=socket(AF_INET,SOCK_STREAM,0);//建立socket 
		if(sockfd==-1){
			printf("socket failed:%d",errno);
			return -1;
			}
			my_addr.sin_family=AF_INET;/*该属性表示接收本机或其他机器传输
			 my_addr.sin_port=htons(3000);/*端口号
			  my_addr.sin_addr.s_addr=htonl(INADDR_ANY);/*IP，括号内容表示本机IP
			   bzero(&(my_addr.sin_zero),8);/*将其他属性置0
				if(bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr))<0){//绑定地址结构体和socket
					printf("bind error");
					return -1;
					}
					listen(sockfd,1000);//开启监听 ，第二个参数是最大监听数 
					while(1){
						sin_size=sizeof(struct sockaddr_in);
						new_fd=accept(sockfd,(struct sockaddr*)&their_addr,&sin_size);//在这里阻塞知道接收到消息，参数分别是socket句柄，接收到的地址信息以及大小 
						if(new_fd==-1){
							printf("receive failed");
							} else{
								printf("receive success");
								send(new_fd,"Hello World!",12,0);//发送内容，参数分别是连接句柄，内容，大小，其他信息（设为0即可） 
								}
								}

								//return 0;

								}
