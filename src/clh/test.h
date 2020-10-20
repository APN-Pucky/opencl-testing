#include "ocl.h"
__kernel void test_noop();
__kernel void test_single_param(int a);
__kernel void test_single_pointer(__global int* a);
__kernel void test_return(int a, __global int* b);
__kernel void test_add(int a, int b, __global int* c);
__kernel void test_array_add(int a, int b, __global int* c, int N) ; 
__kernel void test_vector_add( __global int* a,  __global int* b, __global int* c, int N) ; 
/*
 * NxN matrix, N vector, N result
 */
__kernel void test_mat_mul( __global double* matrix, __global  double* vector, __global  double* result,int N);
/*
 * NxN matrix, N vector, N result
 */
__kernel void test_speed( __global double* matrix, __global  double* vector, __global  double* result,int N);