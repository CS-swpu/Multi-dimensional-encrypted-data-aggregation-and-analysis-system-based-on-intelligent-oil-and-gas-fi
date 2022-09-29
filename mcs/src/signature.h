#ifndef _SIGNATURE_H_
#define _SIGNATURE_H_


void initParamSig();
void initKeySig(element_t u,element_t U);
void signature(element_t * sig,int* tim,element_t u,element_t c,int tag,int id);
int verify(element_t*  sig,int tim ,element_t U,element_t c,int tag,int id);
int verifys(int * tim,element_t sigs[][2],element_t * U,element_t * cs,int tag,int k);
void g1ToByte(char * datas,int* length);
void setg1(char* data);
void setpublic(element_t* U,unsigned char* data);

void setsigs(element_t* sig,unsigned char* data);
element_t* getg1();
void setASig(element_t* ASig,unsigned char* data);
element_t* getHt(char * data,int tag,int k,element_t ACs);
#endif
