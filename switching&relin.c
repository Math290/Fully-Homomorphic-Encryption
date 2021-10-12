#include "arithmod.h"
#include "polynome.h"
#include "switching&relin.h"
#include "encryption.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#define DEGRE_MAX 4
#define t 2

void ModuleSwitching(PolyRNS cipher[],int size,PolyRNS cipherms[],Integer *Q){
    int nb_modulis=cipher[0].nombre-1-cipher[0].level;
    //printf("nombre de modules = %d\n",nb_modulis);
    Poly delta[size];
    for(int i=0;i<size;i++){
      Alloc_Poly(&delta[i],DEGRE_MAX,Q[nb_modulis-1]);
      for(int j=0;j<DEGRE_MAX;j++){
            delta[i].coef[j] = MultiplicationModulaire(cipher[i].tab[nb_modulis].coef[j],Q[nb_modulis].inverse_t,Q[nb_modulis]);
            delta[i].coef[j] *= t;
            //cipher[i].tab[nb_modulis].coef[j] = 0;
        }
    }
    for(int i=0;i<size;i++){
        for(int j=0;j<=nb_modulis-1;j++){
            AddModPoly(&cipher[i].tab[j],&delta[i],&cipherms[i].tab[j],Q[j]);
            for(int k=0;k<DEGRE_MAX;k++){
                cipherms[i].tab[j].coef[k] = MultiplicationModulaire(cipherms[i].tab[j].coef[k],Q[j].tab_inverse_modulis[nb_modulis-j-1],Q[j]);

            }
        }
        cipherms[i].level=cipher[i].level+1;

    }
    for(int i=0;i<size;i++){
        Free_Poly(&delta[i]);
    }

}

PolyRNS *FastBaseExtension(PolyRNS *a,Integer *Q,Integer *P,int k){
    PolyRNS *res = (PolyRNS*) malloc(sizeof(PolyRNS));
    Alloc_PolyRNS(res,k,P);
    res->nombre = a->nombre;res->level = a->level;
    int64_t tmp[k];int64_t u,v;int64_t tmp0[k];int64_t tmp1[k][k];int64_t tmp2=0;
    for(int i=0;i<k;i++){
        for(int j=0;j<k;j++){
            tmp1[i][j] = 1;
        }
    }
    for(int i=0;i<k;i++){
        tmp[i] = 1;
    }
    for(int i=0;i<k;i++){
        for(int j=0;j<k;j++){
            if(i != j){
                tmp[i] = MultiplicationModulaire(Q[j].modulus,tmp[i],Q[i]);
            }
        }
        Euclide(tmp[i],Q[i].modulus,&u,&v);
        tmp0[i] = u;
        for(int j=0;j<k;j++){
            for(int l=0;l<k;l++){
                if(l != i){
                tmp1[i][j] = MultiplicationModulaire(Q[l].modulus,tmp1[i][j],P[j]);
                }
            }
        }

    }
    for(int j=0;j<k;j++){
        for(int i=0;i<k;i++){
            for(int l=0;l<DEGRE_MAX;l++){
                tmp2 = MultiplicationModulaire(a->tab[i].coef[l],tmp0[i],Q[i]);
                tmp2 = MultiplicationModulaire(tmp1[i][j],tmp2,P[j]);
                res->tab[j].coef[l] = AdditionModulaire(res->tab[j].coef[l],tmp2,P[j]);
            }

        }

    }

    return res;
}


PolyRNS *RelinearKeyRNS(Integer *Q,Integer *P,Poly *sk,int k){
    Poly ai,ei,tmp,tmp0,tmp1,eiconv;
    Alloc_Poly(&ai,DEGRE_MAX,Q[0]);Alloc_Poly(&ei,DEGRE_MAX,Q[0]);Alloc_Poly(&tmp,DEGRE_MAX,Q[0]);
    Alloc_Poly(&tmp0,DEGRE_MAX,Q[0]);Alloc_Poly(&tmp1,DEGRE_MAX,Q[0]);Alloc_Poly(&eiconv,DEGRE_MAX,Q[0]);
    Integer *PQ = (Integer*)malloc(2*k*sizeof(Integer));
    for(int i=0;i<k;i++){
        PQ[i] = Q[i];
        PQ[i+k] = P[i];
    }
    PolyRNS *rlk = (PolyRNS*)malloc(2*sizeof(PolyRNS));
    if(rlk==NULL){
        printf("Allocation memoire impossible\n");
        exit(0);
    }
    uint64_t p0[k];
    for(int i=0;i<k;i++){
        p0[i]=1;
    }
    for(int i=0;i<k;i++){
        for(int j=0;j<k;j++){
            p0[i]=MultiplicationModulaire(P[j].modulus,p0[i],Q[i]);//P mod q_i
        }
    }

    Alloc_PolyRNS(&rlk[0],2*k,PQ);Alloc_PolyRNS(&rlk[1],2*k,PQ);
    for(int j=0;j<DEGRE_MAX;j++){
        ei.coef[j] = Binomiale();
    }
    for(int i=0;i<2*k;i++){
        ai.q = PQ[i];ei.q = PQ[i];tmp.q = PQ[i];
        tmp0.q = PQ[i];tmp1.q = PQ[i];
        for(int j=0;j<DEGRE_MAX;j++){
            ai.coef[j] = DistributionUniforme(PQ[i]);
        }
        Entier2Poly(&ei,&eiconv);
        ConvertSk(&eiconv,&eiconv,PQ[i]);
        ConvertSk(sk,&tmp,PQ[i]);
        Mult_ModPoly(&tmp,&tmp,&tmp0,PQ[i]);//sk^2
        Mult_ModPoly(&ai,&tmp,&tmp1,PQ[i]);//ai*sk
        AddModPoly(&tmp1,&eiconv,&rlk[0].tab[i],PQ[i]);//ai*sk+t*ei

        if(i<k){
            for(int j = 0;j<DEGRE_MAX;j++){
                tmp0.coef[j] = MultiplicationModulaire(tmp0.coef[j],p0[i],PQ[i]);//sk^2*P
          }
            AddModPoly(&tmp0,&rlk[0].tab[i],&rlk[0].tab[i],PQ[i]);//sk^2*P+ai*sk+t*ei % PQ_i
        }

        for(int j=0;j<DEGRE_MAX;j++){
            rlk[1].tab[i].coef[j] = PQ[i].modulus-ai.coef[j];
        }

    }
    Free_Poly(&ai);Free_Poly(&tmp0);Free_Poly(&tmp1);Free_Poly(&tmp);
    Free_Poly(&ei);free(PQ);Free_Poly(&eiconv);
    return rlk;
}


PolyRNS *RelinearisationGHS(PolyRNS ciphermult[],PolyRNS *rlk,Integer *Q,Integer *P,int k,int level){
    //int nb_modulis=ciphermult[0].nombre-1-ciphermult[0].level;
    PolyRNS *res = (PolyRNS*) malloc(2*sizeof(PolyRNS));
    Alloc_PolyRNS(&res[0],k,Q);Alloc_PolyRNS(&res[1],k,Q);
    PolyRNS *delta = (PolyRNS*) malloc(2*sizeof(PolyRNS));
    Integer *PQ = (Integer*)malloc(2*k*sizeof(Integer));
    PolyRNS *tmp = (PolyRNS*) malloc(2*sizeof(PolyRNS));
    Alloc_PolyRNS(&tmp[0],k,Q);Alloc_PolyRNS(&tmp[1],k,Q);
    for(int i=0;i<k;i++){
        PQ[i] = Q[i];
        PQ[i+k] = P[i];
    }
    Alloc_PolyRNS(&delta[0],2*k,PQ);Alloc_PolyRNS(&delta[1],2*k,PQ);
    PolyRNS *ciphermodp = FastBaseExtension(&ciphermult[2],Q,P,k);//
    //ciphermodq*rlk
    for(int i=0;i<k;i++){
        //ciphermodq*rlk
        Mult_ModPoly(&(ciphermult[2].tab[i]),&rlk[0].tab[i],&delta[0].tab[i],Q[i]);
        Mult_ModPoly(&(ciphermult[2].tab[i]),&rlk[1].tab[i],&delta[1].tab[i],Q[i]);
        //ciphermodp*rlk
        Mult_ModPoly(&ciphermodp->tab[i],&rlk[0].tab[i+k],&delta[0].tab[i+k],P[i]);
        Mult_ModPoly(&(ciphermodp->tab[i]),&rlk[1].tab[i+k],&delta[1].tab[i+k],P[i]);

    }
    for(int i=0;i<k;i++){
        for(int j=0;j<DEGRE_MAX;j++){
            delta[0].tab[i+k].coef[j]=MultiplicationModulaire(delta[0].tab[i+k].coef[j],P[i].inverse_t,P[i]);
            delta[1].tab[i+k].coef[j]=MultiplicationModulaire(delta[1].tab[i+k].coef[j],P[i].inverse_t,P[i]);
        }
    }
    //FastExtension delta
    for(int i=0;i<k;i++){
        tmp[0].tab[i]=delta[0].tab[i+k];
        tmp[1].tab[i]=delta[1].tab[i+k];
    }
    PolyRNS *tmp0=FastBaseExtension(&tmp[0],P,Q,k);
    PolyRNS *tmp1=FastBaseExtension(&tmp[1],P,Q,k);
    int64_t *tmp2=InversePModQ(Q,P,k);//Inverse de P modq_i
    for(int i=0;i<k;i++){
        for(int j=0;j<DEGRE_MAX;j++){
            tmp0->tab[i].coef[j] *= t;
            tmp1->tab[i].coef[j] *= t;
        }
        AddModPoly(&(delta[0].tab[i]),&(tmp0->tab[i]),&(tmp0->tab[i]),Q[i]);
        AddModPoly(&(delta[1].tab[i]),&(tmp1->tab[i]),&(tmp1->tab[i]),Q[i]);
        for(int j=0;j<DEGRE_MAX;j++){
            tmp0->tab[i].coef[j] = MultiplicationModulaire(tmp0->tab[i].coef[j],tmp2[i],Q[i]);
            tmp1->tab[i].coef[j] = MultiplicationModulaire(tmp1->tab[i].coef[j],tmp2[i],Q[i]);
        }
        AddModPoly(&(ciphermult[0].tab[i]),&(tmp0->tab[i]),&(res[0].tab[i]),Q[i]);
        AddModPoly(&(ciphermult[1].tab[i]),&(tmp1->tab[i]),&(res[1].tab[i]),Q[i]);

    }
    return res;
}
