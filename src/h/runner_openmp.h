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
        //global_id = i;
        global_ids[omp_get_thread_num()] = i;
        run_cpu();
    }
}

void run_hybrid() {
    int scale = hybrid_scale;
    int downscale = 100;
    auto clrunner = Runner(*this);
    debug_printf("post copy");
    //TODO fix this to localwork size (256) scaled
    clrunner.N = std::min(N,multiple_of_local((N*scale+downscale-1)/downscale));
    debug_printf("clrunner.N = %i", clrunner.N);
    int leftover = N-clrunner.N;
    for( int i = 0 ; i < sizeof...(Args);++i)
    {
        if(read_mem[i]) {
            if(clrunner.sizes[i] == N) {
                clrunner.sizes[i] = std::min(clrunner.sizes[i],clrunner.N);
            }
            else {
                throw std::runtime_error("wrong read mem size for hybrid");
            }
        }
    }
    #pragma omp parallel for
    for(int i =0; i < leftover+1;++i) 
    {
        global_ids[omp_get_thread_num()] = clrunner.N-1+i;
        if(i==0)
            clrunner.run_opencl();
        else
            run_cpu();
    }
}