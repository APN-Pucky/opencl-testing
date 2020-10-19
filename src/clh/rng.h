#include "ocl.h"
//simple rng
int rng_int(ulong seed) {
	seed = (seed * 0x5DEECE66DL + 0xBL) & ((1L << 48) - 1);
	return (int)( seed >> 16);
}

int rng_in(ulong seed,int min, int max) {
	return rng_int(seed)%((max+1)-min)+min;
}