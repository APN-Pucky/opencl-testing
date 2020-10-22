#include "catch.hpp"
#include "runner.h"
#include "test.h"
#include "clew.h"
#include <chrono>
#include <vector>

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
    SECTION("mpi: ") {
        printf("mpi: ");
        r.run_mpi();
    }
#endif
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout << " == > Elapsed time: " << elapsed.count() << " s\n";
}

void speed(int N) {
    std::vector<double> matrix(N*N);
    std::vector<double> vecctor(N);
    std::vector<double> result(N);
    for(int i = 0; i  < N ; ++i) {
        for(int j = 0; j  < N ; ++j) {
            //matrix[i*N+j] = 0.0;
            matrix[i*N+j] = i*j + i+j;
        }
        vecctor[i]=i;
    }
    debug_printf("Result location %d \n", &result);
    auto r = Runner<double*,double*,double*,int>("test_speed",test_speed,N,&matrix[0],&vecctor[0],&result[0],N);
    r.set_mem<0>(CL_MEM_READ_ONLY,N*N,false);
    r.set_mem<1>(CL_MEM_READ_ONLY,N,false);
    r.set_mem<2>(CL_MEM_WRITE_ONLY,N,true);
    run_test(r);
    std::vector<double> cmp_result(N);
    for(int i = 0; i < N;++i) {
        global_ids[0]=i;
        //test_mat_mul((double*)matrix,(double*)vector,(double*)cmp_result,N);
        //printf("%f %f  ", result[i],cmp_result[i]);
        //REQUIRE( CLOSE(result[i],cmp_result[i]  ));
    }
}
TEST_CASE( "speed", "[benchmark]" ) {
    //SECTION("N=10") {
    //    printf("Speed: N=%i : ",10);
    //    speed(10);
    //}
    ///*
    SECTION("N=256") {
        printf("Speed: N=%i : ",256);
        speed(256);
    }
    //*/
    //SECTION("N=100000") {
    //    mat_mul(100000);
    //}
};

// TODO separate speed/benchmark executables
void speed2(int N) {
    std::vector<double> matrix(N);
    std::vector<double> vecctor(N);
    std::vector<double> result(N);
    for(int i = 0; i  < N ; ++i) {
        //matrix[i*N+j] = 0.0;
        matrix[i] = i*i - i-i;
        vecctor[i]=i;
    }
    debug_printf("Result location %d \n", &result);
    auto r = Runner<double*,double*,double*,int>("test_speed2",test_speed2,N,&matrix[0],&vecctor[0],&result[0],N);
    r.set_mem<0>(CL_MEM_READ_ONLY,N,false);
    r.set_mem<1>(CL_MEM_READ_ONLY,N,false);
    r.set_mem<2>(CL_MEM_WRITE_ONLY,N,true);
    run_test(r);
    std::vector<double> cmp_result(N);
    for(int i = 0; i < N;++i) {
        global_ids[0]=i;
        //test_mat_mul((double*)matrix,(double*)vector,(double*)cmp_result,N);
        //printf("%f %f  ", result[i],cmp_result[i]);
        //REQUIRE( CLOSE(result[i],cmp_result[i]  ));
    }
}
TEST_CASE( "speed2", "[benchmark]" ) {
    printf("Speed2 Test");
    ///*
    SECTION("N=9x256") {
        printf("Speed2: N=%i : ",9*256);
        speed2(9*256);
    }
    SECTION("N=90x256") {
        printf("Speed2: N=%i : ",90*256);
        speed2(90*256);
    }
    
    //*/
    //SECTION("N=100000") {
    //    mat_mul(100000);
    //}
};

TEST_CASE("speed2 optimize", "[benchmark]") {

}
