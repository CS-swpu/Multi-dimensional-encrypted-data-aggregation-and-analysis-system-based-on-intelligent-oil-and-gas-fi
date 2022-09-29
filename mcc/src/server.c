
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
#include "pbc.h"
#include <gmp.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "cryptog.h"
#include "SocketProject.h"
#include "signature.h"

#define DEST_PORT 30000//目标地址端口号 
#define DEST_IP "10.0.0.95"/*目标地址IP，这里设为本机*/ 
#define MAX_DATA 100//接收到的数据最大程度 
 /*cocket句柄和接受到连接后的句柄 */

int fd=-1;

int sockfd;

int k=10;
int ts[1000];
element_t ACs,ht;
element_t  ASig[2];
int z=0;
	element_t Us[1024];//sigpublics
int Usi=0;
	//mpz_t eks[1024];
	element_t cs[1024];//cs
int csi=0;
	element_t sigs[1024][2];//sigs
int sigsi=0;


int  receiveS(int new_fd,char * message,int len){
	char mess[2048];
	memset(mess,1,2048);
        int length=0;
        int x=recv(new_fd,mess,2048,0);
         memcpy(&length,mess,4);
         if(length>len)return -1;
        memcpy(message,mess+4,length);
	
	return length;
 }
 
 int sendMessS(int new_fd,int code,char* message, int len){
if(len>2048)return -1;
  char mess[2048];
memset(mess,1,2048);
len=len+4;
   memcpy(mess,&len,4);
   memcpy(mess+4,&code,4);
   memcpy(mess+8,message,len);
   printf("length   %d",len);

   send(new_fd,mess,2048,0);
	return 0;
	}	 
	
	

void sendChallenge(){
initSocket("127.0.0.1",30003);
char message[2048];
for(int i=0;i<k;i++)
	{
	 ts[i]=1;
     memcpy(message+i*4,&ts[i],4);

}
sendMess(1321,message,k*4);

}




void process(int fdd,char * message , int len){
	int code;
	if(len<4)return ;
	
	memcpy(&code,message,4);
        printf("\n code : %d \n",code);
	unsigned char  mess[len-4];
	memset(mess,0,len-4);
	memcpy(mess,message+4,len-4);
	printf("\n message is %s \n",mess);
	
	int id=0;
        
	int ms[2];
	switch(code){
		case 1100: setg1(mess);
		break;
		case 1200: setCryPubKeypub(mess, 0);
		break;
		case 1210: setCryPubKeypub(mess, 1);
		break;
		case 1220: setCryPubKeypub(mess, 2);
		break;
		case 1230: setCryg(mess);
		break;
		case 1240: 

			decryptionByte( ms,mess);
			printf("\n decryption ms is %d \n",ms[0]);
			break;
		case 1250: 

			memcpy(&id,mess,4); 
			setpublic(&Us[id],mess+4);
			Usi++;

			break;
		case 1261: 
			memcpy(&id,mess,4); 
			setsigs(sigs[id], mess+4);
			sigsi++;
			break;
		case 1262: 

			memcpy(&id,mess,4); 
			setCry(&cs[id], mess+4);
			csi++;
			break;
		case 1300: 
			setACs(&ACs,mess);
			z++;

			//save ACs;
			break;
		case 1310: 
			setASig(ASig,mess);
			z++;

			//save ASig;
			break;
		case 1320:
			//request challenge

			sendChallenge();
			break;
		case 1321:
			//response challenge
			//ht=getHt(mess, 2,k,ACs)[0];
			//sendtoMCC(ht);
			break;
		case 1330:
			//ht
          setHt(&ht,mess);
         fd=fdd;
			z++;

			break;
		
			
			break;
			default: printf("\n\n\n\n\n    code is default    \n\n\n\n");
			break;
	}


}


void thread(void * data){
printf("\n\n\nthread start success \n \n\n\n");

char message[2048];
memset(message,0,2048);
int new_fd=0;
 new_fd=*(int *) data;
int tims[1024];
         memset(tims,0,1024);
while(1){

if(new_fd==fd&&z==3&&Usi==k)
{
printf("\n \n \n z %d \n\n\n ",z);
z=5;
element_printf("\n ht %B \n",ht);
int x= verifyAggSig( ASig,ACs , ts, Us,ht,k);
if(x==0){
	printf("\n verifyAggSig  success \n");


	int ms[2];
 
	decryption(ms,ACs);

	printf("\n m %d \n m2  %d \n",ms[0],ms[1]);

break;
}
else
printf("\n verifyAggSig failed \n");


}

 int len=receiveS(new_fd,message ,2048);
if(len>=0)
process(new_fd,message , len);
else {
printf("\n \n \n end  soketfd  %d   len  %d \n\n\n ",new_fd,len);
break;
}

}


}

void print_welcome()
{
	printf("===================================================================\n");
	printf("=                                                                 =\n");
	printf("=                      Military   Control    Center               =\n");
	printf("=                                                          beta   =\n");
	printf("===================================================================\n");
	printf("Compiling time: UTC " __TIME__ " " __DATE__ "\n");
	printf("initializing...\n");
}

int main(){
print_welcome();
 initParamSig();
initParam();
initKey();
element_t* g= getg();
element_t* pub= getPub();

element_t* g1= getg1();

    struct sockaddr_in my_addr;/*本方地址信息结构体，下面有具体的属性赋值*/
    struct sockaddr_in their_addr;/*对方地址信息*/
    int sin_size;

    sockfd=socket(AF_INET,SOCK_STREAM,0);//建立socket 
    if(sockfd==-1){
        printf("socket failed:%d",errno);
        return -1;
    }
    my_addr.sin_family=AF_INET;/*该属性表示接收本机或其他机器传输*/
    my_addr.sin_port=htons(30001);/*端口号*/
    my_addr.sin_addr.s_addr=htonl(INADDR_ANY);/*IP，括号内容表示本机IP*/
    bzero(&(my_addr.sin_zero),8);/*将其他属性置0*/
    if(bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr))<0){//绑定地址结构体和socket
        printf("bind error");
        return -1;
    }
        listen(sockfd,1000);//开启监听 ，第二个参数是最大监听数 
        while(1){
            sin_size=sizeof(struct sockaddr_in);
           int new_fd=accept(sockfd,(struct sockaddr*)&their_addr,&sin_size);//在这里阻塞知道接收到消息，参数分别是socket句柄，接收到的地址信息以及大小  /*socket句柄和建立连接后的句柄*/
            if(new_fd==-1){
                printf("receive failed");
        } else{
            printf("\n receive success \n");

printf("\n new_fd is %d ",new_fd);
 pthread_t pt;
		int ret_thread=pthread_create(&pt,NULL,(void *)&thread,(void *)&new_fd);
		//pthread_join(pt,NULL);//wait thread end
		if(ret_thread != 0)
		{
			printf("create thread faile");
			return 1;
		}

               else
pthread_detach(pt);

            unsigned char mess[1024];
       /*     element_printf("g %B \n",g[0]);
element_printf("pub1 %B \n",pub[0]);
element_printf("pub2 %B \n",pub[1]);
element_printf("pub3 %B \n",pub[2]);*/
		    memset(mess,0,1024);
		int len=element_length_in_bytes(pub[0]);
		element_to_bytes(mess,pub[0]);
		 sendMessS(new_fd,1200,mess,len);
                
		len=element_length_in_bytes(pub[1]);
		element_to_bytes(mess,pub[1]);
		sendMessS(new_fd,1210,mess,len);
  
		      len=element_length_in_bytes(pub[2]);
		element_to_bytes(mess,pub[2]);
		sendMessS(new_fd,1220,mess,len);

		
 
		      len=element_length_in_bytes(g[0]);

		element_to_bytes(mess,g[0]);

		sendMessS(new_fd,1230,mess,len);


  len=element_length_in_bytes(g1[0]);

		element_to_bytes(mess,g1[0]);

		sendMessS(new_fd,1100,mess,len);



           //recv(new_fd,mess,1024,0);
        //  printf("\n end:  %s \n",mess);
        }
    }

return 0;











}


























