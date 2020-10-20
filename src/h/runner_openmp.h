//#include <omp.h>

/*
template<typename ...Args>
void Runner<Args...>::run_openmp()
{
    //TODO enable openmp
    #pragma omp parallel for
    for(int i =0; i < N;++i) 
    {
        global_id = i;
        run_cpu();
    }
}
*/
void run_openmp()
{
    #pragma omp parallel for
    for(int i =0; i < N;++i) 
    {
        global_id = i;
        run_cpu();
    }
}

void run_hybrid() {
    int scale = 2;
    auto clrunner = Runner(*this);
    //TODO fix this to localwork size (256) scaled
    clrunner.N = N/scale;
    for( int i = 0 ; i < sizeof...(Args);++i)
    {
        if(read_mem[i]) {
            clrunner.sizes[i] = sizes[i]/scale;
        }
    }
    #pragma omp parallel for
    for(int i =0; i < N/scale+1;++i) 
    {
        global_id = N/scale+i-1;
        if(i==0)
            run_opencl();
        else
            run_cpu();
    }
}