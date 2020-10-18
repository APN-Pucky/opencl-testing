#ifndef RUNNER_MPI_H
#define RUNNER_MPI_H
#ifdef USE_MPI
#include "mpi.h"
#define MASTER 0

template<typename ...Args>
void Runner<Args...>::run_mpi()
{
    int numtasks,taskid,leftover,chunksize;

    MPI_Init(NULL,NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    printf ("MPI task %d has started...  ", taskid);
    chunksize = (N / numtasks);
    leftover = (N % numtasks);

    if(taskid == MASTER) {
        printf("MASTER");
        run_cpu();
    }

    if(taskid > MASTER) {
        printf("NOT MASTER");
        run_cpu();
    }
    //MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}
#endif
#endif