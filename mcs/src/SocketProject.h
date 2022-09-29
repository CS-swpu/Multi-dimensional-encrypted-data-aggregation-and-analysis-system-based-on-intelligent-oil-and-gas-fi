#ifndef _SOCKETPROJECT_H_
#define _SOCKETPROJECT_H_

int  initSocket(char * ip,int port);
int sendMess(int code,char* message, int len);
int receive(char * message , int len);



#endif
