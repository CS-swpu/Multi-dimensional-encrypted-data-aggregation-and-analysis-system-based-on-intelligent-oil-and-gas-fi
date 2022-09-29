
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<sys/wait.h>
void main(){

int code=1024;
char mess[1024];
memcpy(mess,&code,4);
int h=0;
memcpy(&h,mess,4);
printf("%d \n",*(int *)mess);
printf("%d ",h);
}
