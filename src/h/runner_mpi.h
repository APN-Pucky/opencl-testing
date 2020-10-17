#ifndef RUNNER_MPI_H
#define RUNNER_MPI_H
#include "mpi.h"
#define MASTER 0

template<typename ...Args>
void Runner<Args...>::run_mpi(int argc, char* argv[])
{
    int numtasks,taskid,leftover,chunksize;

    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    printf ("MPI task %d has started...  ", taskid);
    chunksize = (N / numtasks);
    leftover = (N % numtasks);

    if(taskid == MASTER) {
        printf("MASTER");
    }

    if(taskid > MASTER) {
        printf("NOT MASTER");
    }
    MPI_Finalize();
}
#endif