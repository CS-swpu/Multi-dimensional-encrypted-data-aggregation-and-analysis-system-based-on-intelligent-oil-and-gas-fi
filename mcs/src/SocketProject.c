
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
int sockfd1;/*cocket句柄和接受到连接后的句柄 */
int  receive(char * message,int len){
	char mess[2048];
	memset(mess,0,2048);
	int length=0;
	int x=recv(sockfd1,mess,2048,0);
	memcpy(&length,mess,4);
	printf("length is %d",length);
	if(length>len)return -1;
	memcpy(message,mess+4,length);

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

	return send(sockfd1,mess,2048,0);
	

}	 

int closeSocket(){
	close(sockfd1);
}

int  initSocket(char * ip,int port){
	struct sockaddr_in dest_addr;/*目标地址信息*/
	char buf[MAX_DATA];//储存接收数据 
	memset(buf,0,MAX_DATA);
	sockfd1=socket(AF_INET,SOCK_STREAM,0);/*建立socket*/
	if(sockfd1==-1){
		printf("socket failed:%d",errno);
		return 1;
	}


	//参数意义见上面服务器端 
	dest_addr.sin_family=AF_INET;
	dest_addr.sin_port=htons(port);
	dest_addr.sin_addr.s_addr=inet_addr(ip);
	bzero(&(dest_addr.sin_zero),8);

	if(connect(sockfd1,(struct sockaddr*)&dest_addr,sizeof(struct sockaddr))==-1){//连接方法，传入句柄，目标地址和大小 
		printf("connect failed:%d",errno);
		return 1;
	} 
	printf("connect success");
	
	
	return 0;

}

























