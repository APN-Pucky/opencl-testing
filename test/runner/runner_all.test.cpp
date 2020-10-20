#include "catch.hpp"
#include "runner.h"
#include "runner_test.h"
#include "test.h"
#include "clew.h"
#include <chrono>

#define EPS 0.000001
#define CLOSE(a , b) fabs(a-b) <= EPS*std::max(fabs(a),fabs(b))

template<typename ...Args>
void run_test(Runner<Args...>& r ) {
    auto start = std::chrono::high_resolution_clock::now();
    SECTION("noparallel") {
        printf("noparallel: ");
        r.run_noparallel();
    }
    SECTION("openmp") {
        printf("openmp: ");
        r.run_openmp();
    }
#ifdef TOO_TEST_OPENCL
    SECTION("opencl") {
        printf("opencl: ");
        r.run_opencl();
    }
#endif
#ifdef TOO_TEST_MPI
    SECTION("mpi") {
        printf("mpi: ");
        r.run_mpi();
    }
#endif
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
}

TEST_CASE( "run void noop", "[runner]" ) {
    auto r = Runner<>("test_noop",test_noop,1);
    run_test(r);
}
TEST_CASE( "pass param", "[runner]" ) {
    int a=1,b=2,c=3;
    auto r = Runner<int>("test_single_param",test_single_param,1,b);
    run_test(r);
}
TEST_CASE( "pass pointer", "[runner]" ) {
    int a=1,b=2,c=3;
    auto r = Runner<int*>("test_single_pointer",test_single_pointer,1,&b);
    r.set_mem<0>(CL_MEM_READ_ONLY,1,false);
    run_test(r);
}
TEST_CASE( "write to pointer", "[runner]" ) {
    int a=1,b=2,c=3;
    auto r = Runner<int,int*>("test_return",test_return,1,a,&b);
    r.set_mem<1>(CL_MEM_WRITE_ONLY,1,true);
    run_test(r);
    REQUIRE ( b == 1);
}

TEST_CASE( "add together", "[runner]" ) {
    int a=1,b=2,c=0;
    auto r = Runner<int,int,int*>("test_add",test_add,1,a,b,&c);
    r.set_mem<2>(CL_MEM_WRITE_ONLY,1,true);
    run_test(r);
    REQUIRE ( c == 3);
}

void array_add(int N) {
    int c[N];
    int a=1,b=2;
    auto r = Runner("test_array_add",test_array_add,N,a,b,(int*)c,N);
    r.set_mem<2>(CL_MEM_WRITE_ONLY,N,true);
    run_test(r);
    for(int i = 0; i < N;++i) {
        REQUIRE( c[i] == 3 );
    }
}

TEST_CASE( "array add", "[runner]" ) {
    SECTION("N=10") {
        array_add(10);
    }
    SECTION("N=1000") {
        array_add(1000);
    }
    SECTION("N=100000") {
        array_add(100000);
    }
};

void vector_add(int N) {
    int a[N],b[N],c[N];
    for( int i = 0; i  < N ;++i) a[i] = 1;
    for( int i = 0; i  < N ;++i) b[i] = 2;
    auto r = Runner("test_vector_add",test_vector_add,N,(int*)a,(int*)b,(int*)c,N);
    r.set_mem<0>(CL_MEM_READ_ONLY,N,false);
    r.set_mem<1>(CL_MEM_READ_ONLY,N,false);
    r.set_mem<2>(CL_MEM_WRITE_ONLY,N,true);
    run_test(r);
    for(int i = 0; i < N;++i) {
        REQUIRE( c[i] == 3 );
    }
}

TEST_CASE( "vector add", "[runner]" ) {
    SECTION("N=10") {
        vector_add(10);
    }
    SECTION("N=1000") {
        vector_add(1000);
    }
    SECTION("N=100000") {
        vector_add(100000);
    }
};

void mat_mul(int N) {
    double matrix[N*N];
    double vector[N];
    double result[N];
    for(int i = 0; i  < N ; ++i) {
        for(int j = 0; j  < N ; ++j) {
            //matrix[i*N+j] = 0.0;
            matrix[i*N+j] = i*j + i+j;
        }
        vector[i]=i;
    }
    debug_printf("Result location %d \n", &result);
    auto r = Runner("test_mat_mul",test_mat_mul,N,(double*)matrix,(double*)vector,(double*)result,N);
    r.set_mem<0>(CL_MEM_READ_ONLY,N*N,false);
    r.set_mem<1>(CL_MEM_READ_ONLY,N,false);
    r.set_mem<2>(CL_MEM_WRITE_ONLY,N,true);
    run_test(r);
    double cmp_result[N];
    for(int i = 0; i < N;++i) {
        global_id=i;
        test_mat_mul((double*)matrix,(double*)vector,(double*)cmp_result,N);
        //printf("%f %f  ", result[i],cmp_result[i]);
        REQUIRE( CLOSE(result[i],cmp_result[i]  ));
    }
}
TEST_CASE( "mat mul", "[runner]" ) {
    //SECTION("N=10") {
    //    mat_mul(10);
    //}

    SECTION("N=1000") {
        mat_mul(1000);
    }
    //SECTION("N=100000") {
    //    mat_mul(100000);
    //}
};


void speed(int N) {
    double matrix[N*N];
    double vector[N];
    double result[N];
    for(int i = 0; i  < N ; ++i) {
        for(int j = 0; j  < N ; ++j) {
            //matrix[i*N+j] = 0.0;
            matrix[i*N+j] = i*j + i+j;
        }
        vector[i]=i;
    }
    debug_printf("Result location %d \n", &result);
    auto r = Runner("test_speed",test_speed,N,(double*)matrix,(double*)vector,(double*)result,N);
    r.set_mem<0>(CL_MEM_READ_ONLY,N*N,false);
    r.set_mem<1>(CL_MEM_READ_ONLY,N,false);
    r.set_mem<2>(CL_MEM_WRITE_ONLY,N,true);
    run_test(r);
    double cmp_result[N];
    for(int i = 0; i < N;++i) {
        global_id=i;
        //test_mat_mul((double*)matrix,(double*)vector,(double*)cmp_result,N);
        //printf("%f %f  ", result[i],cmp_result[i]);
        //REQUIRE( CLOSE(result[i],cmp_result[i]  ));
    }
}
TEST_CASE( "speed", "[runner]" ) {
    //SECTION("N=10") {
    //    mat_mul(10);
    //}

    SECTION("N=40") {
        speed(40);
    }
    //SECTION("N=100000") {
    //    mat_mul(100000);
    //}
};

void speed2(int N) {
    double matrix[N];
    double vector[N];
    double result[N];
    for(int i = 0; i  < N ; ++i) {
        //matrix[i*N+j] = 0.0;
        matrix[i] = i*i - i-i;
        vector[i]=i;
    }
    debug_printf("Result location %d \n", &result);
    auto r = Runner("test_speed2",test_speed2,N,(double*)matrix,(double*)vector,(double*)result,N);
    r.set_mem<0>(CL_MEM_READ_ONLY,N,false);
    r.set_mem<1>(CL_MEM_READ_ONLY,N,false);
    r.set_mem<2>(CL_MEM_WRITE_ONLY,N,true);
    run_test(r);
    double cmp_result[N];
    for(int i = 0; i < N;++i) {
        global_id=i;
        //test_mat_mul((double*)matrix,(double*)vector,(double*)cmp_result,N);
        //printf("%f %f  ", result[i],cmp_result[i]);
        //REQUIRE( CLOSE(result[i],cmp_result[i]  ));
    }
}
TEST_CASE( "speed2", "[runner]" ) {
    //SECTION("N=10") {
    //    mat_mul(10);
    //}

    SECTION("N=256") {
        speed2(256);
    }
    //SECTION("N=100000") {
    //    mat_mul(100000);
    //}
};


