
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

__kernel void test_vector_add(__global int* a, __global int* b, __global int* c, int N) { 
	int id = get_global_id(0);	
    if(id < N) {
        c[id] = a[id]+b[id];
        //printf("id=%d\n",id);

    }
}

__kernel void test_mat_mul( __global double* matrix,  __global double* vector, __global double* result, int N) {
    int id = get_global_id(0);	
    if(id < N && id >= 0) {
        double tmp = 0;
            for(int i = 0; i  < N ; ++i) {
                tmp += matrix[N*id+i]*vector[i];
            }
        result[id] = tmp;
        //printf("MY ID IS :%d @ %d \n",id,omp_get_num_threads());
    }
}
__kernel void test_speed( __global double* matrix,  __global double* vector, __global double* result, int N) {
    __local double lmatrix[40];
    __local double lvector[40];

    int id = get_global_id(0);	
    
    if(id < N && id >= 0) {
        for(int i = 0; i  < N ;++i) lmatrix[i] = matrix[N*id+i];
        for(int i = 0; i  < N ;++i) lvector[i] = vector[i]; 
        double tmp = 0;
        for(int j = 0; j < 10000000; ++j) {
            for(int i = 0; i  < N ; ++i) {
                tmp += lmatrix[i]*lvector[i];
            }
            for(int i = N; i  >= 0 ; --i) {
                tmp -= lmatrix[i]*lvector[i];
            }
        }
        result[id] = tmp;
    }
}
__kernel void test_speed2( __global double* matrix,  __global double* vector, __global double* result, int N) {
    __local double lmatrix;
    __local double lvector;
    int id = get_global_id(0);	
    
    if(id < N && id >= 0) {
        lmatrix = matrix[id];
        lvector = vector[id];
        double tmp = 0;
        for(int j = 0; j < 10000000; ++j) {
            tmp += lmatrix*lvector;
            tmp += lmatrix+lvector;
            tmp -= lmatrix-lvector;
        }
        result[id] = tmp;
    }
}