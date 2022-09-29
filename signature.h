#ifndef _SIGNATURE_H_
#define _SIGNATURE_H_




void initParamSig();
void getPrivactKey(element_t* Y,int ID);
void signature(element_t * sig,element_t c,element_t* Y ,int T,int IDS,int IDM);
int verify(element_t *sig , element_t c,int T,int IDS,int IDM);
int verifys(element_t sigs[][2] , element_t *c,int T,int IDS,int *IDM,int len);
#endif
