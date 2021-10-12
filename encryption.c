#include "arithmod.h"
#include "encryption.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#define t 2

int64_t** Inverse_Modulis(int64_t tab[],int k){
    int64_t **tab_Inverse;
    tab_Inverse = (int64_t**)malloc((k-1)*sizeof(int64_t*));
    int64_t u,v;
    for(int i=0;i<k-1;i++){
        tab_Inverse[i] =(int64_t*)malloc((k-i-1)*sizeof(int64_t));
        for(int j = 0;j<k-i-1;j++){
            Euclide(tab[i+1+j],tab[i],&u,&v);
            if(u<0){
                tab_Inverse[i][j]= (u%tab[i]) + tab[i];
            }
            else{
                tab_Inverse[i][j]= u%tab[i];
            }
        }

    }
    return tab_Inverse;
}

Integer*Setup(int64_t tab[],int k,int64_t precompt[],int shift){
    int64_t u,v;
    int64_t** Inverse=Inverse_Modulis(tab,k);
    Integer *tableau = (Integer *)malloc(k*sizeof(Integer));
    for(int i=0;i<k;i++){
        Euclide(-t+tab[i],tab[i],&u,&v);
        tableau[i].modulus = tab[i];
        tableau[i].precomp = precompt[i];
        tableau[i].shift = shift;
        tableau[i].inverse_t = u;
        tableau[i].nb_inverse = k-i-1;
        tableau[i].tab_inverse_modulis = Inverse[i];
    }
   free(Inverse);
   return tableau;
}
int64_t *InversePModQ(Integer *Q,Integer *P,int k){
    int64_t *res = (int64_t*)malloc(k*sizeof(int64_t));
    int64_t tmp[k];int64_t u,v;
    for(int i=0;i<k;i++){
        tmp[i]=1;
    }
    for(int i=0;i<k;i++){
        for(int j=0;j<k;j++){
            tmp[i] = MultiplicationModulaire(P[j].modulus,tmp[i],Q[i]);
        }
        Euclide(tmp[i],Q[i].modulus,&u,&v);
        res[i]=u;
    }

    return res;
}

void KeyGenRNS(Poly *sk, PolyRNS pk[],Integer *Q, int k){
	Poly e, res,a,tmp;
    Alloc_Poly(&e,DEGRE_MAX,Q[0]);
	for(int i = 0; i< DEGRE_MAX;i++){
        sk->coef[i] = Xkey();
        e.coef[i] = Binomiale();
	}
	Entier2Poly(&e,&e);
	for(int i = 0; i < k; i++){
	    Alloc_Poly(&a,DEGRE_MAX,Q[i]);
	    Alloc_Poly(&res,DEGRE_MAX,Q[i]);
	    Alloc_Poly(&tmp,DEGRE_MAX,Q[i]);
        for(int j = 0; j < DEGRE_MAX; j++){
            a.coef[j] = DistributionUniforme(Q[i]);
        }
        ConvertSk(sk,&res,Q[i]);
        Mult_ModPoly(&a,&res,&pk[0].tab[i],Q[i]);//a*sk
        ConvertSk(&e,&tmp,Q[i]);//t*e
        AddModPoly(&tmp,&pk[0].tab[i],&pk[0].tab[i],Q[i]);
        for(int j=0; j < DEGRE_MAX; j++){
            pk[1].tab[i].coef[j] = Q[i].modulus -a.coef[j];
        }
        Free_Poly(&a);Free_Poly(&res);Free_Poly(&tmp);
	}
	Free_Poly(&e);

}

void EncryptionRNS(Poly *plaintext, PolyRNS cipher[],PolyRNS pk[],Integer *Q, int k){
    cipher[0].level=0;
    cipher[1].level=0;

    Poly u,e0,e1,tmp0,tmp1,tmp2,tmp3,res;
	Alloc_Poly(&u,DEGRE_MAX,Q[0]);Alloc_Poly(&e0,DEGRE_MAX,Q[0]);Alloc_Poly(&e1,DEGRE_MAX,Q[0]);
    for(int j=0;j<DEGRE_MAX;j++){
        u.coef[j] = Xkey();
        e0.coef[j] = Binomiale();
        e1.coef[j] = Binomiale();
    }
    Entier2Poly(&e0,&e0);
    Entier2Poly(&e1,&e1);
	for(int i = 0; i < k; i++){
        Alloc_Poly(&tmp0,DEGRE_MAX,Q[i]);
        Alloc_Poly(&tmp1,DEGRE_MAX,Q[i]);
        Alloc_Poly(&tmp2,DEGRE_MAX,Q[i]);
        Alloc_Poly(&tmp3,DEGRE_MAX,Q[i]);
        Alloc_Poly(&res,DEGRE_MAX,Q[i]);

        ConvertSk(&u,&res,Q[i]);
        ConvertSk(&e0,&tmp0,Q[i]);
        ConvertSk(&e1,&tmp1,Q[i]);
        Mult_ModPoly(&res,&pk[0].tab[i],&tmp2,Q[i]);
        AddModPoly(plaintext,&tmp2,&tmp2,Q[i]);
        AddModPoly(&tmp2,&tmp0,&tmp2,Q[i]);
        Mult_ModPoly(&pk[1].tab[i],&res,&tmp3,Q[i]);
        AddModPoly(&tmp3,&tmp1,&tmp3,Q[i]);
        for(int j = 0; j < DEGRE_MAX;j++){
            cipher[0].tab[i].coef[j] = tmp2.coef[j];
            cipher[1].tab[i].coef[j] = tmp3.coef[j];
        }
        Free_Poly(&tmp0);Free_Poly(&tmp1);Free_Poly(&res);
        Free_Poly(&tmp2);Free_Poly(&tmp3);
	}
	Free_Poly(&u);Free_Poly(&e0);Free_Poly(&e1);


}
void DecryptionRNS(PolyRNS cipher[],int size,Poly *sk, Poly *decrypt,Integer *Q,int level,int k){
    assert(level + 1 <= size);
    Poly tmp,res,tmp0;
    Alloc_Poly(&tmp0,DEGRE_MAX,Q[0]);
    Alloc_Poly(&tmp,DEGRE_MAX,Q[0]);
    Alloc_Poly(&res,DEGRE_MAX,Q[0]);
    for(int i=1; i<size;i++){
        Mult(sk,&tmp,Q[0],i);
        Mult_ModPoly(&cipher[i].tab[0],&tmp,&res,Q[0]);
        AddModPoly(&res,&tmp0,&tmp0,Q[0]);
    }

    AddModPoly(&cipher[0].tab[0],&tmp0,decrypt,Q[0]);
    restcentre(decrypt,Q[0]);
    for(int i=0;i<DEGRE_MAX;i++){
        decrypt->coef[i] %= 2;
    }
    Free_Poly(&tmp0);Free_Poly(&tmp);Free_Poly(&res);
}

