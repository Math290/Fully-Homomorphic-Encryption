#include "arithmod.h"
#include "polynome.h"
#ifndef ENCRYPTION_H_INCLUDED
#define ENCRYPTION_H_INCLUDED
#define DEGRE_MAX 4
int64_t** Inverse_Modulis(int64_t tab[],int k);
Integer* Setup(int64_t tab[],int k,int64_t precompt[],int shift);
int64_t* InversePModQ(Integer *Q,Integer *P,int k);
void KeyGenRNS(Poly *sk, PolyRNS pk[], Integer *Q, int k);
void EncryptionRNS(Poly *plaintext, PolyRNS cipher[],PolyRNS pk[],Integer *Q, int k);
void DecryptionRNS(PolyRNS cipher[],int size,Poly *sk, Poly *decrypt,Integer *Q,int level,int k);
#endif
