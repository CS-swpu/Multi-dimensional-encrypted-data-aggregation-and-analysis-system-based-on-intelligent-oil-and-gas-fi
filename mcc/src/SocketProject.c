
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

	send(sockfd1,mess,2048,0);
	return 0;

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
		printf("connect failed55:%d",errno);
		return 1;
	} 
	printf("connect success");
	//recv(sockfd1,buf,MAX_DATA,0);//将接收数据打入buf，参数分别是句柄，储存处，最大长度，其他信息（设为0即可）。 
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





















