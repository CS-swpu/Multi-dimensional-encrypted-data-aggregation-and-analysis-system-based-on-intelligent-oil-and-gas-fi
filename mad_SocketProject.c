//油田MAD中心端mad_SocketProject.c文件


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
#define DEST_PORT 30000//目标地址端口号 
#define DEST_IP "10.0.0.95"/*目标地址IP，这里设为本机*/ 
#define MAX_DATA 100//接收到的数据最大程度 
int sockfd=-1;/*cocket句柄和接受到连接后的句柄 */

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
	printf("length   %d",len);

	send(sockfd,mess,2048,0);
	return 0;

}	 

int closeSocket(){
	close(sockfd);
}

int  initSocket(char * ip,int port){
	if(sockfd!=-1)
		close(sockfd);
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
		printf("connect failed:%d",errno);
		return 1;
	} 
	printf("connect success");
	
	return 0;

}
