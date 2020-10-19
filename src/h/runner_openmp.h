#include <omp.h>
template<typename ...Args>
void Runner<Args...>::run_openmp()
{
    //TODO enable openmp
    #pragma omp for
    for(int i =0; i < N;++i) 
    {
        global_id = i;
        run_cpu();
    }
}