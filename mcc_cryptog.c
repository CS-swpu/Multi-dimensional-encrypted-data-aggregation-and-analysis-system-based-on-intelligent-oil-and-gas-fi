//油田MCC中心端mcc_cryptog.c文件

#include "pbc.h"
#include <gmp.h>
#include <string.h>

pairing_t pairing;
pbc_param_t param;
mpz_t n,q1,q2,q3,q4;
element_t g,f;
mpz_t sk[2];
element_t pub[3];
mpz_t lamada,gama;

element_t * getg(){
return &g;
}
element_t * getPub(){
return pub;
}

void initCryCilent(){
	mpz_init_set_str(n,"3237639915790933474290481674189853027816684075432176231827609161475024085293541102042262190855130024046551894103966062182440761690959131162148079146594651",10);
	pbc_param_init_a1_gen(param,n);

	pairing_init_pbc_param(pairing,param);
	
	
	}
void setCryPubKeypub(char * data, int type){
element_init_G1(pub[type],pairing);
element_from_bytes(pub[type],(unsigned char*)data);
	
	}

void setCryg(char * data){
	element_init_G1(g,pairing);
	element_from_bytes(g,(unsigned char*)data);
	
	}


void setACs(element_t * ACs,char * data){
	element_init_G1(ACs[0],pairing);
element_from_bytes(ACs[0],(unsigned char*)data);
}


void setCry(element_t* c,char * data){
	
	
element_init_G1(c[0],pairing);
element_from_bytes(c[0],(unsigned char*)data);

	}

element_t* initKey(){
	//printf("initialzation key\n");
	mpz_init(sk[0]);
	mpz_init(sk[1]);

	element_init_G1(pub[0],pairing);
	element_init_G1(pub[1],pairing);
	element_init_G1(pub[2],pairing);

	mpz_t h0;
	mpz_init(h0);
	mpz_mul(h0,q2,q3);
	mpz_mul(sk[0],h0,q4);

	mpz_t h1;
	mpz_init(h1);
	mpz_mul(h1,q1,q3);
	mpz_mul(sk[1],h1,q4);


	mpz_t h2;
	mpz_init(h2);
	mpz_mul(h2,q2,q3);
	element_pow_mpz(pub[0],g,h2);
	mpz_t h3;
	mpz_init(h3);
	mpz_mul(h3,q1,q3);
	element_pow_mpz(pub[1],g,h3);
	mpz_t h4;
	mpz_init(h4);
	mpz_mul(h4,q1,q2);
	element_pow_mpz(pub[2],g,h4);
return pub;
}


void homomorphism(element_t c, element_t* cs,int size){
	element_init_G1(c,pairing);
	for(int i=0;i<size;i++)
	element_mul(c,c,cs[i]);
}





void encryption(element_t c,mpz_t m){

	//printf("start encryption\n");
	mpz_t m2;
	mpz_init(m2);
	mpz_mul(m2,m,m);
	element_t r,c1,c2,c3;
	element_init_Zr(r,pairing);
	element_init_G1(c,pairing);
	element_init_G1(c1,pairing);
	element_init_G1(c2,pairing);
	element_init_G1(c3,pairing);
	element_random(r);
	element_pow_mpz(c1,pub[0],m);
	element_pow_mpz(c2,pub[1],m2);
	element_pow_zn(c3,pub[2],r);
	element_mul(c,c1,c2);
	element_mul(c,c,c3);


}


void decryption(int*  ms,element_t c){
	//printf("decryption start\n");
int T=1000000;
	memset(ms,-1,sizeof(int)*2);
	element_t ms1,ms2,ms3,ms4,v1,v2;
	element_init_G1(ms1,pairing);
	element_init_G1(ms2,pairing); 
	element_init_G1(ms3,pairing);
	element_init_G1(ms4,pairing);
	element_init_G1(v1,pairing);
	element_init_G2(v2,pairing);
	element_pow_mpz(ms1,c,sk[0]);
	element_pow_mpz(v1,pub[0],sk[0]);
	for(int i=1;i<T;i++){
		element_mul(ms3,ms3,v1);
		if(element_cmp(ms3,ms1)==0){
		ms[0]=i;
		break;
		}
	}
	element_pow_mpz(ms2,c,sk[1]);
	element_pow_mpz(v2,pub[1],sk[1]);
	for(int i=1;i<T;i++){
	element_mul(ms4,ms4,v2);
		if(element_cmp(ms4,ms2)==0){
		ms[1]=i;
		break;
		}
	}

}
void decryptionByte(int*  ms,char* data){
element_t c;
element_init_G1(c,pairing);
element_from_bytes(c,(unsigned char *)data);
decryption(ms,c);

}
element_t* initParam(){
	//printf("param initialzation\n");
	mpz_init(q1);
	mpz_init(q2);
	mpz_init(q3);
	mpz_init(q4);
	mpz_init_set_str(n,"3237639915790933474290481674189853027816684075432176231827609161475024085293541102042262190855130024046551894103966062182440761690959131162148079146594651",10);
	mpz_init_set_str(q1,"228838128488897696962214316024437960089",10);
	mpz_init_set_str(q2,"207152950786020794830682838653235435197",10);
	mpz_init_set_str(q3,"286151768525805050636702696264254564801",10);
	mpz_init_set_str(q4,"238678115424997732951390330113794717647",10);
	pbc_param_init_a1_gen(param,n);





	pairing_init_pbc_param(pairing,param);



pairing_t pairing2;
pbc_param_t param2;
pbc_param_init_a1_gen(param2,n);
pairing_init_pbc_param(pairing2,param);
	element_t x,y,z;
	element_init_G1(g,pairing);
	element_init_G1(x,pairing);
	element_init_G1(y,pairing);
	element_init_G1(z,pairing);
	element_init_G1(f,pairing);
	element_random(g);
	//element_printf("g %B\n",g);
	element_pow_mpz(f,g,q2);
        element_pow_mpz(f,f,q3);
        element_pow_mpz(f,f,q4);
	//element_printf("f %B\n",f);
	//element_pow_mpz(x,g,q1);
	//element_printf("x %B\n",x);
	//element_pow_mpz(y,x,q2);
	//element_printf("y %B\n",y);
	//element_pow_mpz(z,y,q3);
	//element_printf("z %B\n",z);
	//element_pow_mpz(x,z,q4);
	//element_printf("x %B\n",x);
return &g;

}

void initRegist(mpz_t ek,int k,int id){

	mpz_init_set_str(lamada,"30097008700300000230023001",10);
	mpz_init(gama);
	mpz_sub(gama,q1,lamada);
	mpz_init(ek); 
	mpz_set(ek,lamada);
	mpz_t as[1024];
	for(int i=0;i<k;i++){
		//TODO  need change
		mpz_init_set_si(as[i],i*24);

	}

	mpz_t x;
	mpz_init_set_si(x,id);
	for(int i=0;i<k-1;i++){

		mpz_mul(as[i],as[i],x);
		mpz_mul(x,x,x);
		mpz_add(ek,ek,as[i]);

	}

}

void aggrea(element_t ac,element_t * cs,int len){
        
	element_init_G1(ac,pairing);
        
	for(int i=0;i<len;i++)
	{
	element_mul(ac,cs[i],ac);
	}

}
