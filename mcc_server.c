//油田MCC中心端mcc_server.c文件：
#define DEST_PORT 30000//目标地址端口号 
#define DEST_IP "10.0.0.95"/*目标地址IP，这里设为本机*/ 
#define MAX_DATA 100//接收到的数据最大程度 
/*cocket句柄和接受到连接后的句柄 */

int fd=-1;

int sockfd;

int k=50;
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

struct timeval start,end;
int timeuse=0;
int computTime(){
	timeuse=1000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000;
	return timeuse;
}


char receiveSMesss[1024][2048];
int receiveSEnds[1024];
int  receiveS(int new_fd,char * message,int len){
	int tags=1;
	char * receiveSMess=&receiveSMesss[new_fd][0];
	int receiveSEnd= receiveSEnds[new_fd];
	char mess[2048];
	//memset(mess,1,2048);
	int length=0;
	while(tags){

		int readLen=recv(new_fd,mess,2048,0);
		if(readLen<=0)return -1;
		int freeLen=2048-receiveSEnd;        
		if(readLen<=freeLen)
		{
			memcpy(receiveSMess+receiveSEnd,mess,readLen);
			receiveSEnd=readLen+receiveSEnd;
			if(receiveSEnd==2048){
				receiveSEnd=0;
				memcpy(&length,receiveSMess,4);  
				memcpy(message,receiveSMess+4,length);
				tags=0;
			}

		}
		else
		{
			int cover=readLen-freeLen;
			memcpy(receiveSMess+receiveSEnd,mess,freeLen);
			memcpy(&length,receiveSMess,4);  
			memcpy(message,receiveSMess+4,length);
			memcpy(receiveSMess,mess+freeLen,cover);
			receiveSEnd=cover;
			tags=0;

		}
		receiveSEnds[new_fd]=receiveSEnd;


	}

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
	
	unsigned char  mess[len-4];
	memset(mess,0,len-4);
	memcpy(mess,message+4,len-4);
	

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
			default: printf("    code is default    \n");
			break;
	}


}


void thread(void * data){
	printf("thread start success \n");

	char message[2048];
	memset(message,0,2048);
	int new_fd=0;
	new_fd=*(int *) data;
	int tims[1024];
	memset(tims,0,1024);
	while(1){

		if(new_fd==fd&&z==3&&Usi==k)
		{

			z=0;
			Usi=0;
			printf("验证聚合数据有效性： \n");
			gettimeofday(&start,NULL);
			int x= verifyAggSig( ASig,ACs , ts, Us,ht,k);
			gettimeofday(&end,NULL);
			if(x==0){
				printf("数据完整性验证成功 验证时间：%d ms\n",computTime());


				int ms[2];
				printf("开始处理且解密数据： \n");
				gettimeofday(&start,NULL);
				decryption(ms,ACs);
				gettimeofday(&end,NULL);
				printf("明文和: %d  平方和: %d  解密时间 %d ms\n",ms[0],ms[1],computTime());
printf("开始分析数据： \n");
float avg=(float)ms[0]/(float)k;
float var=(float)ms[1]/(float)k-(float)ms[0]/(float)k;
printf("平均值: %f  方差: %f  \n ",avg,var);

				
			}
			else
				printf("数据完整性验证失败 %d 验证时间：ms\n",computTime());


		}

		int len=receiveS(new_fd,message ,2048);
		if(len>=0)
			process(new_fd,message , len);
		else break;


	}

//close(new_fd);
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
	printf("初始化签名参数\n");
	initParamSig();
	printf("初始化加密参数 \n");
	initParam();
	printf("生成加密公私钥 \n");
	initKey();
	element_t* g= getg();
	element_t* pub= getPub();

	element_t* g1= getg1();
	element_printf("g: %B \n",g[0]);
	element_printf("g1: %B \n",g1[0]);
        element_printf("公钥: %B \n %B \n %B \n ",pub[0],pub[1],pub[2]);
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
	printf("开启服务等待连接........ \n");
	while(1){
		sin_size=sizeof(struct sockaddr_in);
		int new_fd=accept(sockfd,(struct sockaddr*)&their_addr,&sin_size);//在这里阻塞知道接收到消息，参数分别是socket句柄，接收到的地址信息以及大小  /*socket句柄和建立连接后的句柄*/
		if(new_fd==-1){
			printf("receive failed");
		} else{
			printf("建立连接发送系统参数 \n");

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

			printf("系统参数发送完成 \n");

			//recv(new_fd,mess,1024,0);
			//  printf("\n end:  %s \n",mess);
		}
	}

	return 0;
}
