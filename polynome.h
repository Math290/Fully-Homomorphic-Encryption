#ifndef POLYNOME_H_INCLUDED
#define POLYNOME_H_INCLUDED
#include <stdint.h>
struct Poly
{
	int degre;
	uint64_t *coef;//n coefficient modulo q
	Integer q;//Module des textes chiffrés
};
typedef struct Poly Poly;
struct PolyRNS
{
	int nombre;//nombre de modulis
	int level;//niveau des chiffrés
	Poly *tab;//tableau de poly

};
typedef struct PolyRNS PolyRNS;
void Alloc_Poly(Poly *p, int n,Integer modulis);
void Alloc_PolyRNS(PolyRNS *polyRNS,int nb, Integer *modulis);
void Free_Poly(Poly *p);
void affiche(Poly *p, int n);
void AddModPoly(Poly *a, Poly *b,Poly *c, Integer p);
void SubtractModPoly(Poly *a, Poly *b,Poly *c, Integer p);
void Mult_ModPoly(Poly *a, Poly *b, Poly *c,Integer p);
void Mult(Poly *res,Poly *tmp, Integer p,int k);
void restcentre(Poly *a,Integer p);

void ConvertSk(Poly *sk, Poly *tmp ,Integer p);
void Entier2Poly(Poly *e, Poly *tmp);
int Xkey(void);
uint64_t DistributionUniforme(Integer p);
int Binomiale();
int64_t max(Poly *a);



#endif
