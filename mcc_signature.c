//油田MCC中心端mcc_signature.c文件
#include "pbc.h"
#include <gmp.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
pairing_t sigPairing;
element_t g1,ht1;
void initParamSig(){
	printf("start initParam");
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
void setASig(element_t* ASig,unsigned char* data){
	element_init_G1(ASig[0],sigPairing);
	element_init_G1(ASig[1],sigPairing);
	int length=0;
	memcpy(&length,data,4);
	unsigned char data1[length];
	unsigned char data2[length];
	memcpy(data1,data+4,length);
	memcpy(data2,data+4+length,length);
	element_from_bytes(ASig[0],data1);
	element_from_bytes(ASig[1],data2);	
}

element_t* getHt(char * data,int tag,int k,element_t ACs){
	element_t Hash,hash;
	element_init_G1(Hash,sigPairing);
	element_init_G1(ht1,sigPairing);
	element_init_Zr(hash,sigPairing);
	element_from_hash(Hash,&tag,4);
	mpz_t t;
	mpz_init(t);
	 int x=1;
	for(int i=0;i<k-1;i++){
	
	 memcpy(&x,data+i*4,4);
     mpz_t m;
	 mpz_init_set_si(m,x);
	 mpz_add(t,t,m);
    

	}
	element_pow_mpz(ht1,Hash,t);
	
	unsigned char dd[2048];
    int len=element_to_bytes(dd,ACs);
	memcpy(dd+len,data+(k-1)*4,4);
	element_from_hash(hash,dd,len+4);
	element_pow_zn(Hash,Hash,hash);
    element_mul(ht1,ht1,Hash);
	return &ht1;
}
void setpublic(element_t* U,unsigned char* data){
	element_init_G1(*U,sigPairing);
	element_from_bytes(*U,data);

}

void setsigs(element_t* sig,unsigned char* data){
	int length=0;
	memcpy(&length,data,4);
	unsigned char data1[length];
	unsigned char data2[length];
	memcpy(data1,data+4,length);
	memcpy(data2,data+4+length,length);

	element_init_G1(sig[0],sigPairing);
	element_init_G1(sig[1],sigPairing);
	element_from_bytes(sig[0],data1);
	element_from_bytes(sig[1],data2);
	//element_printf("\n sig[0]  %B \n  sig[1]  %B\n",sig[0],sig[1]);
}

void setg1(char* data){
	element_init_G1(g1,sigPairing);
	element_from_bytes(g1,(unsigned char*)data);

}

element_t* getg1(){
	return &g1;

}
void initKeySig(element_t u,element_t U){
	//printf("start initkey \n");
	element_init_Zr(u,sigPairing);
	element_init_G1(U,sigPairing);
	element_random(u);
	element_pow_zn(U,g1,u);
}

void setHt(element_t * Ht,char * data){

element_init_G1(*Ht,sigPairing);
	element_from_bytes(*Ht,(unsigned char*)data);

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
	*tim=time(&t);
	element_random(w);
	element_pow_zn(sig[0],g1,w);
	element_from_hash(Hash,&tag,4);
	int timeid=*tim+id;
	unsigned char  data[1024];
	int len=element_to_bytes(data,c);
	data[0]=(unsigned char)timeid;
	//printf("data is %x  %x\n",data[0],data[1]);
	element_from_hash(hash,data,len);


	//element_printf("hash1 %B\n" ,hash);
	//element_printf("Hash1 %B\n", Hash);
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

		//element_printf("hash  %d   %B\n" ,timeid,hash);
		//element_printf("Hash %B\n", Hash);

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

int verifyAggSig(element_t* ASig,element_t  ACs , int * ts,element_t* Us,element_t ht,int k){


printf("\n in verifyAggSig \n");
element_printf("\n ht %B \n",ht);
element_t U;
element_init_G1(U,sigPairing);

 mpz_t t;
 mpz_init(t);



element_t hash;
	
	
	element_init_Zr(hash,sigPairing);

	
	
	 
	for(int i=0;i<k-1;i++){
	
	
         mpz_t m;
	 mpz_init_set_si(m,ts[i]);
	 mpz_add(t,t,m);
        

    element_mul(U,Us[i],U);

	}
element_mul(U,Us[k-1],U);



	element_t gt1,gt2;
	element_init_G1(gt1,sigPairing);
        element_init_G1(gt2,sigPairing);
    
	element_pow_mpz(gt1,g1,t);
	unsigned char dd[2048];
    int len=element_to_bytes(dd,ACs);
	memcpy(dd+len,&ts[k-1],4);
	element_from_hash(hash,dd,len+4);

    element_pow_zn(gt2,g1,hash);
	element_mul(gt1,gt1,gt2);
	
element_t out1,out2,sig0;
element_init_GT(out1,sigPairing);
element_init_GT(out2,sigPairing);
element_init_G1(sig0,sigPairing);
element_mul(sig0,ASig[0],U);

element_pairing(out1,ASig[1],gt1);


element_pairing(out2,ht,sig0);

if(element_cmp(out1,out2)==0)return 0;
return -1;





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
	  if(re==0)printf("verify  success");
	  else
		  printf("verify failed");
		  }*/