#include "pbc.h"
#include <gmp.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
pairing_t sigPairing;
element_t g1,ht1;

element_t s,Q;

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
    
        element_init_Zr(s,sigPairing);
	element_random(s);
        element_init_G1(Q,sigPairing);
        element_pow_zn(Q,g1,s);
        
	//element_printf("g1: %B",g1); 


}

void getPrivactKey(element_t* Y,int ID){

        element_init_G1(Y[0],sigPairing);
        element_init_G1(Y[1],sigPairing);
        unsigned char t1[5];
        unsigned char t2[5];
         memset(t1,0,5);
         memcpy(t1,&ID,4);
         memset(t2,1,5);
         memcpy(t2,&ID,4);
         element_from_hash(Y[0],t1,5);
         printf("ttt  %d %d %d %d  %d\n",t2[0],t2[1],t2[2],t2[3],t2[4]);     
         element_from_hash(Y[1],t2,5);


        element_printf("yh: %B \n",Y[1]);
        element_pow_zn(Y[0],Y[0],s);
        element_pow_zn(Y[1],Y[1],s);

}
void signature(element_t * sig,element_t c,element_t* Y ,int T,int IDS,int IDM){

	element_init_G1(sig[0],sigPairing);
	element_init_G1(sig[1],sigPairing);
	element_t k;
	element_init_Zr(k,sigPairing);
	element_random(k);
	element_pow_zn(sig[0],g1,k);
        unsigned char t[8];
     
        memcpy(t,&IDS,4);
        memcpy(t+4,&T,4);
        element_from_hash(sig[1],t,8);
       // element_printf("hash 1 %B \n",sig[1]);
        element_pow_zn(sig[1],sig[1],k);



        element_t yh;
        element_init_G1(yh,sigPairing);
        unsigned char  data[1024];
	int len=element_to_bytes(data,c);
        memcpy(data+len,&IDM,4);
        element_t h;
        element_init_Zr(h,sigPairing);
        element_from_hash(h,data,len+4);
        element_pow_zn(yh,Y[1],h);
        element_printf("hash: %B \n",h);
  element_mul(sig[1],sig[1],Y[0]);
        element_mul(sig[1],sig[1],yh);



      


}
//danxiangya
int verify(element_t *sig , element_t c,int T,int IDS,int IDM){
        element_t out1,out2,out3;
	element_init_GT(out1,sigPairing);
	element_init_GT(out2,sigPairing);
        element_init_GT(out3,sigPairing);
      element_pairing(out1,sig[1],g1);
        element_t Hash,hash;

        element_init_G1(Hash,sigPairing);
        element_init_Zr(hash,sigPairing);

          unsigned char  data[1024];
	int len=element_to_bytes(data,c);
        memcpy(data+len,&IDM,4);
        element_from_hash(hash,data,len+4);

         unsigned char t[8];
    
        memcpy(t,&IDS,4);
        memcpy(t+4,&T,4);
        element_from_hash(Hash,t,8);
  
        element_pairing(out2,sig[0],Hash);
         memset(t,0,5);
        memcpy(t,&IDM,4);

        element_from_hash(Hash,t,5);

        memset(t,1,5);
        memcpy(t,&IDM,4);
       // element_pow_zn(Hash,Hash,s);//
        //element_printf("Hash: %B \n",Hash);

        element_t HH;
	element_init_G1(HH,sigPairing);
        element_from_hash(HH,t,5);

        element_pow_zn(HH,HH,hash);

	
         element_mul(HH,Hash,HH);
        element_pairing(out3,HH,Q);


        element_mul(out2,out2,out3);
         


        if(element_cmp(out1,out2)==0)return 0;
	return -1;


}

int verifys(element_t sigs[][2] , element_t *c,int T,int IDS,int *IDM,int len){
        element_t out1,out2,out3;
	element_init_GT(out1,sigPairing);
	element_init_GT(out2,sigPairing);
        element_init_GT(out3,sigPairing);
element_t sig[2];
element_init_G1(sig[0],sigPairing);
element_init_G1(sig[1],sigPairing);
element_t HashIDT,Hash,hash,HH;
element_init_G1(HashIDT,sigPairing);
element_init_G1(Hash,sigPairing);
element_init_Zr(hash,sigPairing);
element_init_G1(HH,sigPairing);

  unsigned char t[8];
    
        memcpy(t,&IDS,4);
        memcpy(t+4,&T,4);
        element_from_hash(HashIDT,t,8);
  
       

for(int i=0;i<len;i++)
{

element_mul(sig[0],sig[0],sigs[i][0]);
element_mul(sig[1],sig[1],sigs[i][1]);



 unsigned char  data[1024];
	int clen=element_to_bytes(data,c[i]);
        memcpy(data+clen,&IDM[i],4);
        element_from_hash(hash,data,clen+4);


memset(t,0,5);
        memcpy(t,&IDM[i],4);

        element_from_hash(Hash,t,5);
  element_mul(HH,Hash,HH);
        memset(t,1,5);
        memcpy(t,&IDM[i],4);
       // element_pow_zn(Hash,Hash,s);//
        //element_printf("Hash: %B \n",Hash);

      
        element_from_hash(Hash,t,5);
 element_pow_zn(Hash,Hash,hash);

element_mul(HH,Hash,HH);




}
 element_pairing(out1,sig[1],g1);
 element_pairing(out2,sig[0],HashIDT);
element_pairing(out3,HH,Q);

        element_mul(out2,out2,out3);

 if(element_cmp(out1,out2)==0)return 0;
	return -1;

}


 int main(){


	 /*
	  param initialization
	  * 
	  * 
	  */
	/*  initParamSig();
	  element_t Y[2];
          int IDS=234;
          int IDM=567;
          int T=2;
	  element_t sig[2];
	  element_t c;
	  element_init_G1(c,sigPairing);
	  element_random(c);
	 getPrivactKey( Y,IDM);
	signature(sig, c, Y ,T, IDS, IDM);

	//  int re=verify(sig , c,T,IDS,IDM);

*/
int n=101;
initParamSig();

element_t sigs[n][2];
element_t CS[n];
element_t Y[n][2];
int IDM[n];
int IDS=10;
int T=100;
for(int i=0;i<n;i++)
{
element_init_G1(CS[i],sigPairing);

 element_random(CS[i]);
IDM[i]=12*i;
	 getPrivactKey( Y[i],IDM[i]);
	signature(sigs[i], CS[i], Y[i] ,T, IDS, IDM[i]);



}

// re=verify(sig , c,T,IDS,IDM);

int re=verifys(sigs , CS, T,IDS,IDM,n);

	  if(re==0)printf("verify  success");
	  else
		  printf("verify failed");
/*

element_init_G1(CS[0],sigPairing);
element_set(sigs[0][0],sig[0]);
element_set(sigs[0][1],sig[1]);
element_set(CS[0],c);

element_init_G1(sigs[1][0],sigPairing);
element_init_G1(sigs[1][1],sigPairing);
element_init_G1(CS[1],sigPairing);
element_set(sigs[1][0],sig[0]);
element_set(sigs[1][1],sig[1]);

element_t c1;
	  element_init_G1(c1,sigPairing);
	  element_random(c1);
element_set(CS[1],c1);

int IDMS[2];
IDMS[0]=111;
IDMS[1]=222;
int re=verify(sig , c,T,IDS,IDM);

 re=verifys(sigs , CS, T,IDS,IDMS,2);

	  if(re==0)printf("verify  success");
	  else
		  printf("verify failed");*/
		  }
	 
