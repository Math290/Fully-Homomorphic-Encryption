#include "arithmod.h"
#include "polynome.h"
#include "operationbgv.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#define DEGRE_MAX 4
void AdditionRNS(PolyRNS cipher1[],PolyRNS cipher2[],PolyRNS cipheradd[],Integer *Q,int k){
    assert(cipher1[0].level==cipher2[0].level);
    cipheradd[0].level = cipher1[0].level;
    cipheradd[1].level = cipher1[0].level;
    for(int i = 0;i<k-cipheradd[0].level;i++){
        AddModPoly(&cipher1[0].tab[i],&cipher2[0].tab[i],&cipheradd[0].tab[i],Q[i]);
        AddModPoly(&cipher1[1].tab[i],&cipher2[1].tab[i],&cipheradd[1].tab[i],Q[i]);
    }
}
void  MultiplicationRNS(PolyRNS cipher1[],PolyRNS cipher2[],PolyRNS ciphermult[],Integer *Q,int k){
    assert(cipher1[0].level==cipher2[0].level);
    ciphermult[0].level = cipher1[0].level;
    ciphermult[1].level = cipher1[0].level;
    ciphermult[2].level = cipher1[0].level;
    Poly tmp0,tmp1;
    for(int i=0;i<k-ciphermult[0].level;i++){
        Alloc_Poly(&tmp0,DEGRE_MAX,Q[i]);
        Alloc_Poly(&tmp1,DEGRE_MAX,Q[i]);
        Mult_ModPoly(&cipher1[0].tab[i],&cipher2[0].tab[i],&ciphermult[0].tab[i],Q[i]);
        Mult_ModPoly(&cipher1[0].tab[i],&cipher2[1].tab[i],&tmp0,Q[i]);
        Mult_ModPoly(&cipher1[1].tab[i],&cipher2[0].tab[i],&tmp1,Q[i]);
        AddModPoly(&tmp0,&tmp1,&ciphermult[1].tab[i],Q[i]);
        Mult_ModPoly(&cipher1[1].tab[i],&cipher2[1].tab[i],&ciphermult[2].tab[i],Q[i]);
        Free_Poly(&tmp0);Free_Poly(&tmp1);

    }


}
