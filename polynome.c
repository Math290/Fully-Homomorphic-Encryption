#include "arithmod.h"
#include "polynome.h"
#include <stdio.h>
#include <stdlib.h>
#define DEGRE_MAX 4
#define t 2
const int k = 21,MAX = 3;
void Alloc_Poly(Poly *p, int n,Integer modulis){
	p->degre = n;
	p->q = modulis;
	p->coef = calloc(p->degre,sizeof(uint64_t));
	if (p->coef==NULL)
	{
		printf("Probleme allocation\n");
		exit(0);
	}

}
void Alloc_PolyRNS(PolyRNS *polyRNS,int nb, Integer *modulis){
	polyRNS->nombre = nb;
	polyRNS->tab = calloc(polyRNS->nombre,sizeof(Poly));
	if (polyRNS->tab==NULL)
	{
		printf("Probleme allocation\n");
		exit(0);
	}
	for(int i = 0; i < polyRNS->nombre;i++){

        Alloc_Poly(&polyRNS->tab[i],DEGRE_MAX,modulis[i]);
	}
}
void Free_Poly(Poly *p){
	free(p->coef);
}

void affiche(Poly *p, int n){
	p->degre = n;
	 printf("%llu+", p->coef[0]);
	for (int i = 1; i < p->degre-1; i++)
	{
        printf("%llu*x^%d+", p->coef[i],i);

	}
    printf("%llu*x^%d\n", p->coef[p->degre-1],p->degre-1);
}

void ConvertSk(Poly *sk, Poly *tmp,Integer p){
	int64_t tt;
	for (int i = 0; i < DEGRE_MAX; i++)
	{
		tt = sk->coef[i];
		if (tt < 0)
		{
			tt += p.modulus;
			tmp->coef[i] = tt;
		}
		else{
			tmp->coef[i] = tt;
		}
	}

}

void Entier2Poly(Poly *e, Poly *tmp){
	for (int i = 0; i < DEGRE_MAX; i++)
	{
		tmp->coef[i] = t*(e->coef[i]);
	}
}

void AddModPoly(Poly *a, Poly *b,Poly *c, Integer p){
   uint64_t tmp;
	if (a->coef!=NULL && b->coef!=NULL)
	{
		for (int i = 0; i < DEGRE_MAX; i++)
		{
			tmp = AdditionModulaire(a->coef[i],b->coef[i],p);
			c->coef[i] = tmp;
		}

	}
	else{
        printf("Probleme allocation");
	}
}

void SubtractModPoly(Poly *a, Poly *b,Poly *c, Integer p){
    uint64_t tmp;
	if (a->coef!=NULL && b->coef!=NULL)
	{
		for (int i = 0; i < DEGRE_MAX; i++)
		{
			tmp = SoustractionModulaire(a->coef[i] , b->coef[i],p);
			c->coef[i] =tmp;
		}

	}
	else{
        printf("Probleme allocation");
	}
}

void Mult_ModPoly(Poly *a, Poly *b, Poly *c,Integer p){
	int taille = 2*(DEGRE_MAX)-1;
	uint64_t tmp0,tmp1;
	int64_t tmp[taille];
	for (int i = 0; i < taille; i++)
	{
		tmp[i] = 0;
	}
	for (int i = 0; i < DEGRE_MAX; i++)
	{
		for (int j = 0; j < DEGRE_MAX; j++)
		{
			tmp1=MultiplicationModulaire(a->coef[i],b->coef[j],p);
			tmp[i+j] = AdditionModulaire(tmp[i+j],tmp1,p);
		}
	}

	for (int i = 0; i < DEGRE_MAX-1; i++)
	{

		tmp0 = SoustractionModulaire(tmp[i],tmp[DEGRE_MAX+i],p);
		c->coef[i] =tmp0;

	}
	c->coef[DEGRE_MAX-1] = tmp[DEGRE_MAX-1];


}
void Mult(Poly *sk,Poly *tmp, Integer p,int k){
    if(k ==1){
       ConvertSk(sk,tmp,p);
    }
    else{
        for(int i=1;i<k;i++){
            ConvertSk(sk,tmp,p);
            Mult_ModPoly(tmp,tmp,tmp,p);
        }
    }
}


void restcentre(Poly *a,Integer p){
	for (int i = 0; i < DEGRE_MAX; i++)
	{
		if (a->coef[i] > (p.modulus)/2)
		{
			a->coef[i] -= p.modulus;
		}
	}


}
int Xkey(void){
	int a = rand()%MAX;
	a -= 1;
	return a;
}
uint64_t DistributionUniforme(Integer p){
	uint64_t x = ((uint64_t) rand()) + (((uint64_t) rand())<<15) + (((uint64_t) rand())<<30) + (((uint64_t) rand())<<45);
	x = x % p.modulus;

	return x;
}
int Binomiale(){
	int x = 0;
	int *tab1 = malloc(k*sizeof(int));
	int *tab2 = malloc(k*sizeof(int));
	if (tab1 == NULL || tab2 == NULL)
	{
		printf("Probleme d'allocation\n");
		exit(0);
	}
	for (int i = 0; i < k; i++)
	{
		*(tab1 + i) = rand()%2;
		*(tab2 + i) = rand()%2;
	}
	for (int i = 0; i < k; i++ )
	{
		x += tab1[i] - tab2[i];
	}

	free(tab1);
	free(tab2);
	return x;

}
int64_t max(Poly *a){
	int64_t maximum = 0;
	for (int i = 0; i < DEGRE_MAX; i++)
	{
		if (a->coef[i] > maximum)
		{
			maximum = a->coef[i];
		}
	}
	return maximum;
}
