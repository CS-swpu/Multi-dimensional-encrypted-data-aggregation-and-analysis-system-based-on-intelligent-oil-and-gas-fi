#ifndef _CRYPTOG_H_
#define _CRYPTOG_H_

void initCryCilent();
void setCryPubKeypub(char * data, int type);
void setCryg(char * data);

void initKey();
void initParam();
void initRegist(mpz_t ek,int k,int id);
void encryption(element_t c,mpz_t m);
void decryption(int*  ms,element_t c);
void aggrea(element_t* ac,element_t * cs,int len);
element_t* getg();
element_t* getPub();
void decryptionByte(int*  ms,char* data);
void setCry(element_t* c,char * data);














#endif
