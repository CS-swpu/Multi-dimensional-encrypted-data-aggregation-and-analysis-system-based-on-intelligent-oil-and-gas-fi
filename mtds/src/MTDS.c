#include "pbc.h"
#include <gmp.h>
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
#include "cryptog.h"
#include "signature.h"
#include "SocketProject.h"




void process(char * message , int len){
	if(len<4) return;
	int code;
	memcpy(&code,message,4);
	printf("\n code : %d  \n",code);

	unsigned char  mess[len-4];
	memset(mess,0,len-4);
	memcpy(mess,message+4,len-4);

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
		case 1240: printf("\n code is 30 \n");
		break;
		case 1250: printf("\n code is 40 \n");
		break;
		case 1260: printf("\n code is 50 \n");
		break;
		default: printf("\n code is default \n");
		break;
	}

}


void init(){

	initParamSig();
	initParam();
	initKey();
	initSocket("192.168.10.106",30001);
	printf("Socket success");
	/*int length;
	 char datas[2048]; 
	g1ToByte(datas,&length);
	sendMess(datas, length);
	getchar();*/

	char message[2048];
	int len=receive(message ,2048);

	printf("Socket1 success1 \n");
	process(message,len);
	printf("Socket1 success1 \n");
	len=receive(message ,2048);
	process(message,len);
	printf("Socket1 success1 \n");
	len=receive(message ,2048);
	process(message,len);
	printf("Socket1 success1 \n");
	len=receive(message ,2048);
	process(message,len);
	printf("Socket1 success1 \n");
	len=receive(message ,2048);
	process(message,len);
		

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

	int k=20;
	
	int len=0;
	char mess[2048];
	element_t us[1024];
	element_t Us[1024];
	mpz_t eks[1024];
	element_t cs[1024];
	element_t sigs[1024][2];
	int tims[1024];

	memset(tims,0,1024);





	for(int i=0;i<k;i++)
	{
		initKeySig(us[i], Us[i]);
		// initRegist(eks[i],k,i);
		
		mpz_t m;
		mpz_init_set_si(m,(1));
		encryption(cs[i],m);
		// int ms[2];
		//decryption(ms,cs[i]);
		
		//printf("m %d \n m2  %d\n",ms[0],ms[1]);
		signature(sigs[i],&tims[i],us[i],cs[i],2,i);

		

		

	}


	for(int i=0;i<k;i++){

               len=element_length_in_bytes(Us[i]);
		memset(mess,0,2048);
		memcpy(mess,&i,4);
		element_to_bytes(mess+4,Us[i]);
		sendMess(1250,mess,len+4);
                usleep(500);
                
	}


        initSocket("192.168.10.106",30002);
	for(int i=0;i<k;i++){
              
                memset(mess,0,2048);
		memcpy(mess,&i,4);
		len=element_to_bytes(mess+4,Us[i]);
		sendMess(1250,mess,len+4);
printf("\n uslen   %d    i %d\n ",len,i);
//usleep(500);
                len=element_length_in_bytes(cs[i]);
		memset(mess,0,2048);
		memcpy(mess,&i,4);
		element_to_bytes(mess+4,cs[i]);
		sendMess(1262,mess,len+4);
printf("\n cslen   %d    i %d\n ",len,i);
//usleep(500);
        len=element_length_in_bytes(sigs[i][0]);
        memset(mess,0,2048);
		memcpy(mess,&i,4);
		memcpy(mess+4,&len,4);
		//element_printf("\n sig[0]  %B \n  sig[1]  %B\n",sigs[i][0],sigs[i][1]);
		//  element_printf("\n C %B \n",cs[i]);
		element_to_bytes(mess+8,sigs[i][0]);
		element_to_bytes(mess+8+len,sigs[i][1]);
		int zzz=sendMess(1261,mess,2*len+8);
printf("\n sigslen   %d    i %d\n ",len,i);
//usleep(500);
 
	}

	
	while(1);
	return 0;
}


