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
    //SECTION("noparallel") {
    //    printf("noparallel: ");
    //    r.run_noparallel();
    //}
    SECTION("openmp") {
        printf("openmp: ");
        r.run_openmp();
    }
#ifdef TOO_TEST_OPENCL
    SECTION("opencl") {
        printf("opencl: ");
        r.run_opencl();
    }
    SECTION("hybrid") {
        printf("hybrid: ");
        r.run_hybrid();
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

void array_add(const int N) {
    std::vector<int> c(N);
    int a=1,b=2;
    auto r = Runner<int,int,int*,int>("test_array_add",test_array_add,N,a,b,&c[0],N);
    r.set_mem<2>(CL_MEM_WRITE_ONLY,N,true);
    run_test(r);
    for(int i = 0; i < N;++i) {
        REQUIRE( c[i] == 3 );
    }
}

TEST_CASE( "array add", "[runner]" ) {
    SECTION("N=1") {
        array_add(1);
    }
    SECTION("N=10") {
        array_add(10);
    }
    SECTION("N=100") {
        array_add(100);
    }
};

void vector_add(const int N) {
    std::vector<int> a(N),b(N),c(N);
    for( int i = 0; i  < N ;++i) a[i] = 1;
    for( int i = 0; i  < N ;++i) b[i] = 2;
    auto r = Runner<int*,int*,int*,int>("test_vector_add",test_vector_add,N,&a[0],&b[0],&c[0],N);
    r.set_mem<0>(CL_MEM_READ_ONLY,N,false);
    r.set_mem<1>(CL_MEM_READ_ONLY,N,false);
    r.set_mem<2>(CL_MEM_WRITE_ONLY,N,true);
    run_test(r);
    for(int i = 0; i < N;++i) {
        REQUIRE( c[i] == 3 );
    }
}

TEST_CASE( "vector add", "[runner]" ) {
    SECTION("N=1") {
        vector_add(1);
    }
    SECTION("N=10") {
        vector_add(10);
    }
    SECTION("N=100") {
        vector_add(100);
    }
};

void mat_mul(int N) {
    std::vector<double> matrix(N*N);
    std::vector<double> vector(N);
    std::vector<double> result(N);
    for(int i = 0; i  < N ; ++i) {
        for(int j = 0; j  < N ; ++j) {
            //matrix[i*N+j] = 0.0;
            matrix[i*N+j] = i+j;
        }
        vector[i]=i;
    }
    debug_printf("Result location %d \n", &result);
    auto r = Runner<double*,double*,double*,int>("test_mat_mul",test_mat_mul,N,&matrix[0],&vector[0],&result[0],N);
    r.set_mem<0>(CL_MEM_READ_ONLY,N*N,false);
    r.set_mem<1>(CL_MEM_READ_ONLY,N,false);
    r.set_mem<2>(CL_MEM_WRITE_ONLY,N,true);
    run_test(r);
    double cmp_result[N];
    for(int i = 0; i < N;++i) {
        global_ids[0]=i;
        test_mat_mul((double*)matrix,(double*)vector,(double*)cmp_result,N);
        //printf("%f %f  %d \n", result[i],cmp_result[i],i);
        REQUIRE( CLOSE(result[i],cmp_result[i]  ));
    }
}
TEST_CASE( "mat mul", "[runner]" ) {
    //SECTION("N=10") {
    //    mat_mul(10);
    //}

    //SECTION("N=3*256") {
    //    mat_mul(256*3);
    //}
    //SECTION("N=100000") {
    //    mat_mul(100000);
    //}
};


void speed(const int N) {
    std::vector<double> matrix(N*N);
    std::vector<double> vector(N);
    std::vector<double> result(N);
    for(int i = 0; i  < N ; ++i) {
        for(int j = 0; j  < N ; ++j) {
            //matrix[i*N+j] = 0.0;
            matrix[i*N+j] = i*j + i+j;
        }
        vector[i]=i;
    }
    debug_printf("Result location %d \n", &result);
    auto r = Runner<double*,double*,double*,int>("test_speed",test_speed,N,&matrix[0],&vector[0],&result[0],N);
    r.set_mem<0>(CL_MEM_READ_ONLY,N*N,false);
    r.set_mem<1>(CL_MEM_READ_ONLY,N,false);
    r.set_mem<2>(CL_MEM_WRITE_ONLY,N,true);
    run_test(r);
    double cmp_result[N];
    for(int i = 0; i < N;++i) {
        global_ids[0]=i;
        //test_mat_mul((double*)matrix,(double*)vector,(double*)cmp_result,N);
        //printf("%f %f  ", result[i],cmp_result[i]);
        //REQUIRE( CLOSE(result[i],cmp_result[i]  ));
    }
}
TEST_CASE( "speed", "[runner]" ) {
    //SECTION("N=10") {
    //    mat_mul(10);
    //}
    /*
    SECTION("N=40") {
        speed(40);
    }
    //*/
    //SECTION("N=100000") {
    //    mat_mul(100000);
    //}
};

// TODO separate speed/benchmark executables
void speed2(const int N) {
    std::vector<double> matrix(N);
    std::vector<double> vector(N);
    std::vector<double> result(N);
    for(int i = 0; i  < N ; ++i) {
        //matrix[i*N+j] = 0.0;
        matrix[i] = i*i - i-i;
        vector[i]=i;
    }
    debug_printf("Result location %d \n", &result);
    auto r = Runner<double*,double*,double*,int>("test_speed2",test_speed2,N,&matrix[0],&vector[0],&result[0],N);
    r.set_mem<0>(CL_MEM_READ_ONLY,N,false);
    r.set_mem<1>(CL_MEM_READ_ONLY,N,false);
    r.set_mem<2>(CL_MEM_WRITE_ONLY,N,true);
    run_test(r);
    double cmp_result[N];
    for(int i = 0; i < N;++i) {
        global_ids[0]=i;
        //test_mat_mul((double*)matrix,(double*)vector,(double*)cmp_result,N);
        //printf("%f %f  ", result[i],cmp_result[i]);
        //REQUIRE( CLOSE(result[i],cmp_result[i]  ));
    }
}
TEST_CASE( "speed2", "[runner]" ) {
    //SECTION("N=10") {
    //    mat_mul(10);
    //}
    /*
    SECTION("N=10x256") {
        speed2(9*256);
    }
    //*/
    //SECTION("N=100000") {
    //    mat_mul(100000);
    //}
};


