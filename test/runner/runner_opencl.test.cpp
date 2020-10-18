#ifdef TOO_TEST_OPENCL
#include "catch.hpp"
#include "runner.h"
#include "runner_test.h"
#include "test.h"

TEST_CASE( "opencl unsigned add", "[opencl]" ) {
    unsigned int a=1,b=2,c=0;
    auto r = Runner("add",add,1,a,b,&c);
    r.run_opencl();
    //REQUIRE( c == 3 );
}

TEST_CASE( "opencl write to pointer", "[opencl]" ) {
    int a=1,b=2,c=3;
    auto r = Runner<int,int*>("test_return",test_return,a,1,&b);
    r.set_mem<1>(CL_MEM_WRITE_ONLY,1,true);
    r.run_opencl();
    REQUIRE ( b == 1);
}

//TEST_CASE( "mpi unsigned add parallel", "[mpi]" ) {
//    unsigned int a=1,b=2,c=0;
//    auto r = Runner("add",add,1,a,b,&c);
//    char* lel = "lel";
//    char* n = "-n";
//    char* num = "4";
//    char* input[] = {lel,n,num};
//    r.run_mpi(3,input);
//    //REQUIRE( c == 3 );
//}
//*/
#endif