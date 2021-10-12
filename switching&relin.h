#include "arithmod.h"
#include "polynome.h"
#ifndef SWITCHINGRELIN_H_INCLUDED
#define SWITCHINGRELIN_H_INCLUDED
void ModuleSwitching(PolyRNS cipher[],int size,PolyRNS cipherms[],Integer *tab);
PolyRNS *FastBaseExtension(PolyRNS *a,Integer *Q,Integer *P,int k);
PolyRNS* RelinearKeyRNS(Integer *Q,Integer *P,Poly *sk,int k);
PolyRNS *RelinearisationGHS(PolyRNS ciphermodq[],PolyRNS *rlk,Integer *Q,Integer *P,int k,int level);

#endif
