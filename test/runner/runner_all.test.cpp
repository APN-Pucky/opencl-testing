#include "catch.hpp"
#include "runner.h"
#include "runner_test.h"
#include "test.h"
#include "clew.h"

template<typename ...Args>
void run_test(Runner<Args...>& r) {
    SECTION("cpu") {
        r.run_cpu();
    }
#ifdef TOO_TEST_OPENCL
    SECTION("opencl") {
        r.run_opencl();
    }
#endif
#ifdef TOO_TEST_MPI
    //SECTION("mpi") {
    //    r.run_mpi();
    //}
#endif

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
