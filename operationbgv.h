#include "arithmod.h"
#include "polynome.h"
#ifndef OPERATIONBGV_H_INCLUDED
#define OPERATIONBGV_H_INCLUDED
#include <stdint.h>
void AdditionRNS(PolyRNS cipher1[],PolyRNS cipher2[],PolyRNS cipheradd[],Integer *Q,int k);
void  MultiplicationRNS(PolyRNS cipher1[],PolyRNS cipher2[],PolyRNS ciphermult[],Integer *Q,int k);
#endif
