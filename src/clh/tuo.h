#include "ocl.h"
__kernel void simulate_test(  __global int* all_cards, const int count, const int gseed);

__kernel void simulate( const int size_all_cards, __global int* g_all_cards, const int size_gmydeck, __global int* g_mydeck, const int size_genemydeck, __global int* g_enemydeck, const int size_gwinner,__global int* g_winner, const int count, const int g_seed);