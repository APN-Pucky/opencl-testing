#include "mpi.h"
#include <csignal>
#include <cstdlib>

#ifdef TOO_MPI
/*
 * Calls MPI_Finalize if it isn't finalized already.
 */
void global_mpi_finalize() {
    int already_finalized;
    MPI_Finalized(&already_finalized);
    if(!already_finalized) MPI_Finalize();
}
/*
 * Calls MPI_Init if it isn't initialized already and registers mpirunnerfinalize on various exit-conditions.
 */
void global_mpi_init(int* argc, char** argv[]) {
    int already_initialized;
    MPI_Initialized(&already_initialized);
    if(!already_initialized) MPI_Init(argc,argv);
    auto sfinalize = [](int i = 0){ global_mpi_finalize();};
    //^C
    std::signal(SIGINT, sfinalize);
    //abort()
    std::signal(SIGABRT, sfinalize );
    //sent by "kill" command
    std::signal(SIGTERM,  sfinalize);
    //^Z
    //std::signal(SIGTSTP,  sfinalize);

    std::atexit(global_mpi_finalize);
}
#endif