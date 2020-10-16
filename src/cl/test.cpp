#include "ocl.h"
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
        *b = 5;
    }
}