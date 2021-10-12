#ifndef ARITHMOD_H_INCLUDED
#define ARITHMOD_H_INCLUDED
#include <stdint.h>
struct Integer
{
	uint64_t modulus;
	uint64_t precomp;
	int shift;
	uint64_t inverse_t;
	int nb_inverse;
	uint64_t *tab_inverse_modulis;

};
typedef struct Integer Integer;
uint64_t Rest_Centre(uint64_t a,Integer p);
void Shift(uint64_t u[2], uint64_t v[2], int shift);
int64_t pgcd(int64_t a, int64_t b);
void Euclide(int64_t a,int64_t b, int64_t *u, int64_t *v);
uint32_t high(uint64_t x);
uint32_t lower(uint64_t x);
void Add128(uint64_t a[2], uint64_t b[2],uint64_t c[2]);
void Mult128(int64_t a, int64_t b, uint64_t result[2]);
int64_t ReductionModulaire(uint64_t a[2], Integer p);
int64_t MultiplicationModulaire(int64_t a, int64_t b, Integer p);
int64_t AdditionModulaire(int64_t a, int64_t b, Integer p);
int64_t SoustractionModulaire(int64_t a, int64_t b, Integer p);
#endif
