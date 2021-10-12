#include "arithmod.h"
#include "polynome.h"
#include "noisesize.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <assert.h>
#define DEGRE_MAX 4
#define t 2
float NoiseSizeRNS(PolyRNS cipher[],int size,Poly *sk,Integer *Q,int level,int k){
    float noisesize = 0.0;
    assert(level + 1 <= k);
    Poly tmp,res,tmp0,Noise;
    Alloc_Poly(&res,DEGRE_MAX,Q[0]);
    Alloc_Poly(&tmp0,DEGRE_MAX,Q[0]);
    Alloc_Poly(&Noise,DEGRE_MAX,Q[0]);
    Alloc_Poly(&tmp,DEGRE_MAX,Q[0]);
    for(int i=1; i<size;i++){
        Mult(sk,&tmp,Q[0],i);//sk**i
        Mult_ModPoly(&cipher[i].tab[0],&tmp,&res,Q[0]);
        AddModPoly(&res,&tmp0,&tmp0,Q[0]);
    }
    AddModPoly(&cipher[0].tab[0],&tmp0,&Noise,Q[0]);
    restcentre(&Noise,Q[0]);
	for (int i = 0; i < DEGRE_MAX; i++)
	{
		Noise.coef[i] = abs(Noise.coef[i]);
	}

    noisesize =log2f(max(&Noise))-log2f(t);
    Free_Poly(&res);Free_Poly(&Noise);Free_Poly(&tmp);Free_Poly(&tmp0);
    return noisesize;
}
