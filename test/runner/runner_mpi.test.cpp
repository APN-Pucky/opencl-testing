#ifdef TOO_MPI
#ifdef TOO_TEST_MPI
#include "catch.hpp"
#include "runner.h"
#include "test.h"
#include "debug.h"
#include <cstdio>

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
    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    printf("Hello world from processor %s, rank %d out of %d processors\n",
           processor_name, world_rank, world_size);
    if(mpi_rank==0) {
        printf("\n");
        for(int i = 0; i < N;++i) {
            printf("%d",c[i]);
            REQUIRE( c[i] == 3 );
        }
        printf("\n");
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