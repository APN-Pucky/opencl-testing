#include "catch.hpp"
#include "runner.h"
#include "runner_test.h"
#include "test.h"
#include "clew.h"
#include <chrono>

template<typename ...Args>
void run_test(Runner<Args...>& r) {
    auto start = std::chrono::high_resolution_clock::now();
    SECTION("noparallel") {
        printf("noparallel: ");
        r.run_noparallel();
    }
    SECTION("openmp") {
        printf("openmpi: ");
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

