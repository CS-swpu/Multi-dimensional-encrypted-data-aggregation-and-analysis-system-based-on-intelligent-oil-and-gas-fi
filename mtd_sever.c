//油田MTD中心端mtd_sever.c文件

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
#include<sys/time.h>

struct timeval start,end;
int timeuse=0;
char * mccIp="127.0.0.1";
char * madIp="127.0.0.1";
int computTime(){
	timeuse=1000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000;
	return timeuse;
}



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
	initSocket(mccIp,30001);
	printf("Socket success");
	/*int length;
	 char datas[2048]; 
	g1ToByte(datas,&length);
	sendMess(datas, length);
	getchar();*/

	char message[2048];
	int len=receive(message ,2048);

	
	process(message,len);
	
	len=receive(message ,2048);
	process(message,len);
	
	len=receive(message ,2048);
	process(message,len);
	
	len=receive(message ,2048);
	process(message,len);
	
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
	printf("连接MCC获取系统参数\n");
	init();
printf("系统初始化完毕\n");

	int k=50;
	
	int len=0;
	char mess[2048];
	element_t us[1024];
	element_t Us[1024];
	mpz_t eks[1024];
	element_t cs[1024];
	element_t sigs[1024][2];
	int tims[1024];

	memset(tims,0,1024);



mpz_t m;
		

	for(int i=0;i<k;i++)
	{
printf("MTD%d 计算签名公私钥\n",i);
		initKeySig(us[i], Us[i]);
		
		int mm=i%11+1;
		printf("MTD%d 加密数据 m：%d\n",i,mm);
		mpz_init_set_si(m,(i%11+1));
               gettimeofday(&start,NULL);
		encryption(cs[i],m);
		gettimeofday(&end,NULL);
printf("MTD%d 加密计算完成 耗时：%d ms\n",i,computTime());
printf("MTD%d 计算签名数据\n",i);
gettimeofday(&start,NULL);
		signature(sigs[i],&tims[i],us[i],cs[i],2,i);
gettimeofday(&end,NULL);
printf("MTD%d 签名计算完成 耗时：%d ms\n",i,computTime());
		

		

	}
printf("MTD 上传密文和签名数据\n");

	for(int i=0;i<k;i++){

               len=element_length_in_bytes(Us[i]);
		memset(mess,0,2048);
		memcpy(mess,&i,4);
		element_to_bytes(mess+4,Us[i]);
		sendMess(1250,mess,len+4);
                //usleep(500);
                
	}


        initSocket(madIp,30002);
	for(int i=0;i<k;i++){
              
                memset(mess,0,2048);
		memcpy(mess,&i,4);
		len=element_to_bytes(mess+4,Us[i]);
		sendMess(1250,mess,len+4);

                len=element_length_in_bytes(cs[i]);
		memset(mess,0,2048);
		memcpy(mess,&i,4);
		element_to_bytes(mess+4,cs[i]);
		sendMess(1262,mess,len+4);


        len=element_length_in_bytes(sigs[i][0]);
        memset(mess,0,2048);
		memcpy(mess,&i,4);
		memcpy(mess+4,&len,4);
		//element_printf("\n sig[0]  %B \n  sig[1]  %B\n",sigs[i][0],sigs[i][1]);
		//  element_printf("\n C %B \n",cs[i]);
		element_to_bytes(mess+8,sigs[i][0]);
		element_to_bytes(mess+8+len,sigs[i][1]);
		int zzz=sendMess(1261,mess,2*len+8);

//usleep(500);
 
	}

	
	
	return 0;
}

