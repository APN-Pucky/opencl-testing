#include "ocl.h"
__kernel void test_noop();
__kernel void test_single_param(int a);
__kernel void test_single_pointer(__global int* a);
__kernel void test_return(int a, __global int* b);