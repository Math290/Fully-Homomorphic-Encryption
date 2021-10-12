#include "arithmod.h"
#include "polynome.h"
#ifndef NOISESIZE_H_INCLUDED
#define NOISESIZE_H_INCLUDED
#include <stdint.h>
float NoiseSizeRNS(PolyRNS cipher[],int size,Poly *sk,Integer *Q,int level,int k);

#endif
