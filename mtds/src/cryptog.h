#ifndef _CRYPTOG_H_
#define _CRYPTOG_H_

void initCryCilent();
void setCryPubKeypub(unsigned char * data, int type);
void setCryg(unsigned char * data);

void initKey();
void initParam();
void initRegist(mpz_t ek,int k,int id);
void encryption(element_t c,mpz_t m);
void decryption(int*  ms,element_t c);
void aggrea(element_t ac,element_t * cs,int len);



















#endif
