#ifdef TOO_MPI
#ifdef TOO_TEST_MPI
#include "catch.hpp"
#include "runner.h"
#include "runner_test.h"
#include "test.h"

TEST_CASE( "mpi unsigned add", "[mpi]" ) {
    int a=1,b=2,c=0;
    auto r = Runner("test_add",test_add,1,a,b,&c);
    r.set_mem<2>(CL_MEM_WRITE_ONLY,1,true);
    r.run_mpi();
    REQUIRE( c == 3 );
};
TEST_CASE( "mpi unsigned add2", "[mpi]" ) {
    int a=1,b=2,c=0;
    auto r = Runner("test_add",test_add,1,a,b,&c);
    r.set_mem<2>(CL_MEM_WRITE_ONLY,1,true);
    r.run_mpi();
    REQUIRE( c == 3 );
};
//*/

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

#endif
#endif