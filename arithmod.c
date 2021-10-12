#include "arithmod.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
int64_t pgcd(int64_t a, int64_t b){
    int64_t r = 0;
    while(b>0){
        r = a%b;
        a = b;
        b = r;
    }
    return a;

}
void Euclide(int64_t a,int64_t b, int64_t *u, int64_t *v){
    assert(pgcd(a,b)==1);
    int64_t bcopy=b;
	int64_t tmp=0,r=0,s=0,t=1,q=0;
	*u = 1;*v=0;
	while(b > 0){
		q = a/b;r = a%b;a=b;b = r;
		tmp = s; s = *u - q*s; *u = tmp;
		tmp = t; t = *v -q*t; *v = tmp;

	}
	if(*u<0){
        *u += bcopy;
	}
}


uint32_t high(uint64_t x) {
    return x >> 32;
}

uint32_t lower(uint64_t x) {
    return (uint32_t) x;
}
void Mult128(int64_t a, int64_t b, uint64_t result[2]){
	for (int i = 0; i < 2; i++)
	{
		*(result + i) = 0;
	}
	uint64_t a0 = lower(a), b0 = lower(b);
	uint64_t a1 = high(a), b1 = high(b);
	result[0] = a0*b0;
	result[1] = a1*b1;
	uint64_t tmp = a1*b0 + a0*b1;
	uint64_t t1 = lower(tmp)*powl(2,32);
	uint64_t test = result[0] + t1;
	int carry = 0;
	if (test < result[0])
	{
		carry += 1;
	}
	result[0] = test;
	result[1] += high(tmp) + carry;

}

void Add128(uint64_t a[2], uint64_t b[2],uint64_t c[2]){
	c[0] = a[0] + b[0];
	int carry = 0;
	if (c[0] < a[0])
	{
		carry += 1;
	}
	c[1] = a[1] + b[1] + carry;
}
void Shift(uint64_t u[2], uint64_t v[2], int shift){
	uint64_t carry = u[0]>>(64-shift);
	v[0] = u[0] << shift;
	v[1] = (u[1] << shift) + carry;

}

int64_t ReductionModulaire(uint64_t u[2], Integer p){
	uint64_t tmp[2],q[2], r;
	Mult128(u[1],p.precomp,q);
	Shift(u,tmp,p.shift);
	Add128(q,tmp,q);
	Mult128(q[1],p.modulus,tmp);
	r = u[0] - tmp[0];
	if (r >= p.modulus)
	{
		r -= p.modulus;
	}

	return r;
}

int64_t AdditionModulaire(int64_t a, int64_t b, Integer p){
	int64_t res = a + b;
	if (res >= p.modulus)
	{
		res -= p.modulus;
	}
	return res;

}

int64_t SoustractionModulaire(int64_t a, int64_t b, Integer p){
	int64_t res = a - b;
	if (res <  0)
	{
		res += p.modulus;
	}
	return res;

}



int64_t MultiplicationModulaire(int64_t a, int64_t b, Integer p){
	int64_t tmp[2];
	Mult128(a,b,tmp);
	int64_t result = ReductionModulaire(tmp,p);
	return result;

}

uint64_t Rest_Centre(uint64_t a,Integer p){
	if (a >= p.modulus/2)
	{
		a -= p.modulus;
	}
	return a;
}



