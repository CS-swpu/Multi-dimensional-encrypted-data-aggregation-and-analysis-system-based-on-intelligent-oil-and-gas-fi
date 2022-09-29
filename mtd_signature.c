//油田MTD中心端mtd_signature.c文件


#include "pbc.h"
#include <gmp.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
pairing_t sigPairing;
element_t g1;
void initParamSig(){

	//printf("start initParam");
	char param[1024];
	FILE *file=NULL;
	file=fopen("a.param","r");
	size_t count=fread(param,1,1024,file);
	if(!count)pbc_die("in error");

	pairing_init_set_buf(sigPairing,param,count);

	element_init_G1(g1,sigPairing);
	element_random(g1); 

	//element_printf("g1: %B",g1); 


}
void setg1(char* data){

	element_init_G1(g1,sigPairing);
	element_from_bytes(g1,(unsigned char*)data);


}
void initKeySig(element_t u,element_t U){

	//printf("start initkey \n");
	element_init_Zr(u,sigPairing);
	element_init_G1(U,sigPairing);
	element_random(u);
	element_pow_zn(U,g1,u);

}

void signature(element_t * sig,int* tim,element_t u,element_t c,int tag,int id){

	//printf("start signature \n");
	element_t w,hash,Hash;
	element_init_Zr(w,sigPairing);
	element_init_Zr(hash,sigPairing);
	element_init_G1(Hash,sigPairing);
	element_init_G1(sig[0],sigPairing);
	element_init_G1(sig[1],sigPairing);
	time_t t=time(NULL);
	// *tim=time(&t);
	element_random(w);
	element_pow_zn(sig[0],g1,w);
	element_from_hash(Hash,&tag,4);
	int timeid=*tim+id;
	unsigned char  data[1024];
	int len=element_to_bytes(data,c);
	data[0]=(unsigned char)timeid;
	//printf("data is %x  %x\n",data[0],data[1]);
	element_from_hash(hash,data,len);


	////element_printf("hash %B\n" ,hash);
	////element_printf("Hash %B\n", Hash);
	element_mul(hash,hash,w);
	element_add(hash,hash,u);
	element_pow_zn(sig[1],Hash,hash);


}

int verify(element_t*  sig,int tim ,element_t U,element_t c,int tag,int id){


	//printf("start verify\n");
	element_t hash,Hash;

	element_init_Zr(hash,sigPairing);
	element_init_G1(Hash,sigPairing);
	element_from_hash(Hash,&tag,4);
	int timeid=tim+id;
	unsigned char  data[1024];

	int len=element_to_bytes(data,c);
	data[0]=(unsigned char)timeid;
	//printf("data is %x  %x\n",data[0],data[1]);
	element_from_hash(hash,data,len);
	element_t out1,out2;
	element_init_GT(out1,sigPairing);
	element_init_GT(out2,sigPairing);

	//element_printf("hash %B\n" ,hash);
	//element_printf("Hash %B\n", Hash);
	element_pairing(out1,g1,sig[1]);
	element_t hha;

	element_init_G1(hha,sigPairing);
	element_pow_zn(hha,sig[0],hash);
	element_mul(hha,hha,U);
	element_pairing(out2,Hash,hha);

	if(element_cmp(out1,out2)==0)return 0;
	return -1;


}


int verifys(int * tim,element_t  sigs[][2],element_t * U,element_t * cs,int tag,int k){

	element_t Hash;
	element_init_G1(Hash,sigPairing);
	element_from_hash(Hash,&tag,4);
	element_t allSig[2];
	element_init_G1(allSig[0],sigPairing);
	element_init_G1(allSig[1],sigPairing);
	for(int i=0;i<k;i++)
	{
		element_mul(allSig[1],allSig[1],sigs[i][1]);
		element_t hash;

		element_init_Zr(hash,sigPairing);

		int timeid=tim[i]+i;
		unsigned char  data[1024];




		int len=element_to_bytes(data,cs[i]);


		data[0]=(unsigned char)timeid;
		//printf("data is %x  %x\n",data[0],data[1]);
		element_from_hash(hash,data,len);

		element_pow_zn(sigs[i][0],sigs[i][0],hash);
		element_mul(sigs[i][0],sigs[i][0],U[i]);
		element_mul(allSig[0],allSig[0],sigs[i][0]);

		//element_printf("hash1  %d   %B\n" ,timeid,hash);
		//element_printf("Hash1 %B\n", Hash);

	}

	element_t out1,out2;
	element_init_GT(out1,sigPairing);
	element_init_GT(out2,sigPairing);

	element_pairing(out1,g1,allSig[1]);

	element_pairing(out2,Hash,allSig[0]);
	//element_printf("out1  %B \n  out2  %B\n",allSig[1],allSig[0]);
	if(element_cmp(out1,out2)==0)return 0;
	return -1;
}


void g1ToByte(char * datas,int* length){

	*length = element_length_in_bytes(g1);
	unsigned char gSerializ[*length];
	element_to_bytes(gSerializ,g1);

	for(int i=0;i<*length;i++)
	{
		datas[i]=gSerializ[i];
		//printf(" %d ",datas[i]);
	}

}
/*
 * 
 int main(){


	 /*
	  param initialization
	  * 
	  * 
	  * 
	  initParam();
	  element_t u,U;
	  element_t sig[2];
	  element_t c;
	  element_init_G1(c,sigPairing);
	  element_random(c);
	  initkey(u,U);
	  int t;
	  int tag=3;
	  signature(sig,&t, u, c,tag,4);
	  int re=verify(sig,t,U,c,tag,4);
	  if(re==0)//printf("verify  success");
	  else
		  //printf("verify failed");
		  }*/
	 
