#include "catch.hpp"
#include "runner.h"
#include "runner_test.h"
#include "test.h"


TEST_CASE( "unsigned add", "[cpu]" ) {
    unsigned int a=1,b=2,c=0;
    auto r = Runner("add",add,1,a,b,&c);
    r.run_cpu();
    REQUIRE( c == 3 );
}

TEST_CASE( "signed add", "[cpu]" ) {
    int a=2,b=2,c=0;
    auto r = Runner("addi",addi,1,a,b,&c);
    r.run_cpu();
    REQUIRE( c == 4 );
}


TEST_CASE( "adjust params", "[addi]" ) {
    int a=2,b=2,c=0;
    auto r = Runner("addi",addi,1,a,b,&c);
    r.set_arg<0>(-2);
    r.run_cpu();
    REQUIRE( c == 0 );

}
TEST_CASE( "run void noop", "[cpu]" ) {
    auto r = Runner<>("test_noop",test_noop,1);
    r.run_cpu();
}
TEST_CASE( "pass param", "[cpu]" ) {
    int a=1,b=2,c=3;
    auto r = Runner<int>("test_single_param",test_single_param,1,b);
    r.run_cpu();
}
TEST_CASE( "pass pointer", "[cpu]" ) {
    int a=1,b=2,c=3;
    auto r = Runner<int*>("test_single_pointer",test_single_pointer,1,&b);
    r.set_mem<0>(CL_MEM_READ_ONLY,1,false);
    r.run_cpu();
}
TEST_CASE( "write to pointer", "[cpu]" ) {
    int a=1,b=2,c=3;
    auto r = Runner<int,int*>("test_return",test_return,a,1,&b);
    r.set_mem<1>(CL_MEM_WRITE_ONLY,1,true);
    //r.run_opencl(1024);
    r.run_cpu();
    REQUIRE ( b == 1);
}



TEST_CASE( "set read/write access", "[addi]" ) {
}

TEST_CASE( "opencl buffer sizes", "[addi]" ) {
}