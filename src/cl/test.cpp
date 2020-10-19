
#ifndef _OpenCL
#include <cstdio>
#define __global
#define __constant
#define __kernel
//#define ulong unsigned long
#include "runner.h"

#else

#define bool int
#define false 0
#define true 1

#endif


__kernel void test_noop() {}
__kernel void test_single_param(int a) { 
	int id = get_global_id(0);	
}
__kernel void test_single_pointer(__global int* a) { 
}
__kernel void test_return(int a, __global int* b) { 
    if(get_global_id(0)==0) {
        *b = a;
        //printf("that's fun");
    }
    else {
        //printf("less funny");
        *b = a;
    }
}
__kernel void test_add(int a, int b, __global int* c) { 
    *c = a+b;
}

__kernel void test_array_add(int a, int b, __global int* c, int N) { 
	int id = get_global_id(0);	
    if(id < N) {
        c[id] = a+b;
        //printf("id=%d\n",id);

    }
}