#include "catch.hpp"
#include "runner.h"
#include "runner_test.h"
#include "test.h"

TEST_CASE( "mpi unsigned add", "[mpi]" ) {
    unsigned int a=1,b=2,c=0;
    auto r = Runner("add",add,1,a,b,&c);
    r.run_mpi(NULL,NULL);
    //REQUIRE( c == 3 );
}

TEST_CASE( "mpi unsigned add parallel", "[mpi]" ) {
    unsigned int a=1,b=2,c=0;
    auto r = Runner("add",add,1,a,b,&c);
    const char[] input = {"-n","4"}
    r.run_mpi(2,&input);
    //REQUIRE( c == 3 );
}