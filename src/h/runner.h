#ifndef RUNNER_H
#define RUNNER_H

#include <vector>
#include <string>
#include <iostream>
#include <tuple>

#include "clew.h"
#include "debug.h"
#include <omp.h>


const int CLPRESENT = 0 == clewInit();
cl_program load_cl_programs(cl_context context); 
//extern int global_id;
//const int num_threads = omp_get_max_threads();
extern int* global_ids;
#define get_global_id global_ids[omp_get_thread_num()] +
enum Mode {
    noop,
    cpu,
    openmp,
    opencl,
    mpi,
    num_modes
};
template<typename ... Args>
class Runner
{
    public:
        Runner(std::string function_name_, void(*function_pointer_)(Args...), size_t N_,Args...args_) :
            function_name(function_name_),
            function_pointer(function_pointer_),
            N(N_),
            args(args_...),
            flags(sizeof...(Args)),
            mems(sizeof...(Args)),
            map_buffer(sizeof...(Args)),
            map_mem(sizeof...(Args)),
            read_mem(sizeof...(Args)),
            sizes(sizeof...(Args))
        {
            //global_id = 0;
            mode = noop;
            for(auto a : sizes) a = 0;
            if( !CLPRESENT) {
                printf("opencl library not found.\n");
            }
        };
        Runner(const Runner<Args...>& r) = default;
         
        /*
        template<typename T>
        set_argument(int index, &T t,size_t st);
        template<typename T>
        set_argument(int index, &T t);
        */
        template<std::size_t I, typename T>
        void set_arg(T t) {
            std::get<I>(args)=t;
        }
        template<std::size_t I>
        void set_mem(cl_mem_flags f, size_t t, bool read=false) {
            flags[I] = f;
            sizes[I] = t;
            read_mem[I] = read;
        }
        template<std::size_t I>
        void set_flags(cl_mem_flags f) {
            flags[I] = f;
        }
        template<std::size_t I>
        void set_sizes(size_t f) {
            sizes[I] = f;
        }
        void run() 
        {
            switch(mode) {
                case cpu:
                    run_cpu();
                    break;
                case noop: 
                    break;
                case mpi:
                    run_mpi();
                    break;
                case openmp:
                    run_openmp();
                    break;
                case opencl:
                    run_opencl();
                    break;
            }
        }

/*
        template<typename Function, typename Tuple, size_t ... I>
        auto call(Function f, Tuple t, std::index_sequence<I ...>)
        {
             return f(std::get<I>(t) ...);
        }

        template<typename Function, typename Tuple>
        auto call(Function f, Tuple t)
        {
            static constexpr auto size = std::tuple_size<Tuple>::value;
            return call(f, t, std::make_index_sequence<size>{});
        }*/
        void run_cpu()
        {
            std::apply(function_pointer,this->args);
            //call(function_pointer,args);
        }
        
        //void run_openmp();
        void run_noparallel() 
        {
            for(int i =0; i < N;++i) 
            {
                global_ids[0] = i;
                run_cpu();
            }
        }
        void run_opencl(); 
        #ifdef TOO_MPI
        void run_mpi();
        #endif

#include "runner_openmp.h"

    //private:
        size_t N;
        Mode mode;
        std::vector<cl_mem> mems;
        std::vector<cl_mem> map_mem;
        std::vector<unsigned char *> map_buffer;
        std::vector<bool> read_mem;
        std::vector<size_t> sizes;
        std::vector<cl_mem_flags> flags;
        std::string function_name;
        std::tuple<Args...> args;
        void(*function_pointer)(Args...);
};

#include "runner_opencl.h"
#include "runner_mpi.h"
#endif