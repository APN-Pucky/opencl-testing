#ifndef MPI_UTIL_H
#define MPI_UTIL_H
#ifdef TOO_MPI
/*
 * Calls MPI_Finalize if it isn't finalized already.
 */
void global_mpi_finalize(); 
/*
 * Calls MPI_Init if it isn't initialized already and registers mpirunnerfinalize on various exit-conditions.
 */
void global_mpi_init(int* argc, char** argv[]); 
#endif
#endif