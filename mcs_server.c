//油田MCS中心端mcs_server.c文件
#define DEST_PORT 30000//目标地址端口号 
#define DEST_IP "10.0.0.95"/*目标地址IP，这里设为本机*/ 
#define MAX_DATA 100//接收到的数据最大程度 
/*cocket句柄和接受到连接后的句柄 */



int sockfd;

element_t ACs;
element_t ASig[2];
int z=0;
int k=50;

char receiveSMesss[1024][2048];
int receiveSEnds[1024];
struct timeval start,end;
int timeuse=0;
int computTime(){
	timeuse=1000*(end.tv_sec-start.tv_sec)+(end.tv_usec-start.tv_usec)/1000;
	return timeuse;
}



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



void sendtoMCC(element_t ht){
	element_printf("接收到挑战信息，计算ht: %B\n",ht);
	printf("回传挑战数据到MCC\n");
	char message[2048];
	memset(message,0,2048);
	int len=element_to_bytes(message,ht);
	sendMess(1330, message, len);
         //element_printf("\n ht %B \n",ht);
	memset(message,0,2048);
	len=element_to_bytes(message,ACs);
	sendMess(1300, message, len);

	memset(message,0,2048);
	len=element_to_bytes(message+4,ASig[0]);
	memcpy(message,&len,4);
	element_to_bytes(message+4+len,ASig[1]);
	sendMess(1310, message,2*len+4);


}


void process(char * message , int len){
	int code;
	if(len<4)return ;
	
	memcpy(&code,message,4);
	unsigned char  mess[len-4];
	memset(mess,0,len-4);
	memcpy(mess,message+4,len-4);
	
	element_t* ht;
        
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
			//setpublic(&Us[id],mess+4);
			//Usi++;
			break;
		case 1261: 
			memcpy(&id,mess,4); 
			//setsigs(sigs[id], mess+4);
			//sigsi++;
			break;
		case 1262: 

			memcpy(&id,mess,4); 
			//setCry(&cs[id], mess+4);
			//csi++;
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
			break;
		case 1321:
			//response challenge
			ht=getHt(mess, 2,k,ACs);
			sendtoMCC(*ht);
			break;
		case 1330:
			//ht

			break;
		case 1340:
			//ACs
			break;
		case 1350:
			//ASig
			break;
			default: printf("\n code is default \n");
			break;
	}


}


void print_welcome()
{
	printf("===================================================================\n");
	printf("=                                                                 =\n");
	printf("=                      Military  Cloud   Storage                  =\n");
	printf("=                                                          beta   =\n");
	printf("===================================================================\n");
	printf("Compiling time: UTC " __TIME__ " " __DATE__ "\n");
	printf("initializing...\n");
}
int  init(){

	initParamSig();
	initParam();
	
	initSocket("127.0.0.1",30001);

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


void thread(void * data){
	printf("thread start success\n");

	//
	//printf("thread new_fd is %d",new_fd);
	char message[2048];
	memset(message,0,2048);
	int new_fd=0;
	new_fd=*(int *) data;
	int tims[1024];
	memset(tims,0,1024);
	while(1){

		if(z==2)
		{
 printf("MAD聚合数据上传成功，等待MCC发起挑战\n");
                        z=0;
			int x=sendMess(1320,"1", 1);
             

		}

		int len=receiveS(new_fd,message ,2048);
		if(len>0)
			process(message , len);
		else break;

	}

//close(new_fd);
}


int main(){
print_welcome();
	printf("连接MCC获取系统参数\n");
	init();
printf("系统初始化完毕\n");
	struct sockaddr_in my_addr;/*本方地址信息结构体，下面有具体的属性赋值*/
	struct sockaddr_in their_addr;/*对方地址信息*/
	int sin_size;

	sockfd=socket(AF_INET,SOCK_STREAM,0);//建立socket 
	if(sockfd==-1){
		printf("socket failed:%d",errno);
		return -1;
	}
	my_addr.sin_family=AF_INET;/*该属性表示接收本机或其他机器传输*/
	my_addr.sin_port=htons(30003);/*端口号*/
	my_addr.sin_addr.s_addr=htonl(INADDR_ANY);/*IP，括号内容表示本机IP*/
	bzero(&(my_addr.sin_zero),8);/*将其他属性置0*/
	if(bind(sockfd,(struct sockaddr*)&my_addr,sizeof(struct sockaddr))<0){//绑定地址结构体和socket
		printf("bind error");
		return -1;
	}
	listen(sockfd,1000);//开启监听 ，第二个参数是最大监听数 
	printf("开启服务等待MAD连接\n");
	while(1){
		sin_size=sizeof(struct sockaddr_in);
		int new_fd=accept(sockfd,(struct sockaddr*)&their_addr,&sin_size);//在这里阻塞知道接收到消息，参数分别是socket句柄，接收到的地址信息以及大小  /*socket句柄和建立连接后的句柄*/
		if(new_fd==-1){
			printf("receive failed");
		} else{
			printf("receive success \n");

			
			pthread_t pt;
			int ret_thread=pthread_create(&pt,NULL,(void *)&thread,(void *)&new_fd);
			//pthread_join(pt,NULL);//wait thread end
			if(ret_thread != 0)
			{
				printf("create thread faile \n");
				return 1;
			}

			else{
				printf("MAD连接成功，等待MAD上传聚合数据\n");
				pthread_detach(pt);
			}

		}
	}

	return 0;

}

