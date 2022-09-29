
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h> 
#include <unistd.h>
#include <pthread.h>


#include "SocketProject.h"




void process(char * message , int len){
	if(len<4) return;
	int code;
	memcpy(&code,message,4);
	printf("\n code : %d  \n",code);

	unsigned char  mess[len-4];
	memset(mess,0,len-4);
	memcpy(mess,message+4,len-4);

	
}


void init(){

	

}




void print_welcome()
{
	printf("===================================================================\n");
	printf("=                                                                 =\n");
	printf("=                      Military   Terminal   Devices              =\n");
	printf("=                                                          beta   =\n");
	printf("===================================================================\n");
	printf("Compiling time: UTC " __TIME__ " " __DATE__ "\n");
	printf("initializing...\n");
}


int main(){
	print_welcome();
	init();
initSocket("192.168.10.106",30002);
     char mess[2048];
	int k=22;
for(int i=0;i<k;i++)
{
memcpy(mess,&i,4);
memset(mess+4,23,112);
printf("\n %d ",i);
sendMess(i, mess, 116);
printf("\n %d ",i);
}
printf("\n sss ");
	closeSocket();
	return 0;
}


