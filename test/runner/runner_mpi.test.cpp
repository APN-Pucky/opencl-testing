#ifdef TOO_MPI
#ifdef TOO_TEST_MPI
#include "catch.hpp"
#include "runner.h"
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

TEST_CASE( "mpi array add2", "[mpi]" ) {
    int N =10;
    int c[N];
    int a=1,b=2;
    auto r = Runner("test_array_add",test_array_add,N,a,b,c,N);
    r.set_mem<2>(CL_MEM_WRITE_ONLY,N,true);
    r.run_mpi();
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
    if(mpi_rank==0) {
        for(int i = 0; i < N;++i)
            REQUIRE( c[i] == 3 );
    }
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