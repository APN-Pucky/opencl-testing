#ifndef RUNNER_H
#define RUNNER_H

#include <vector>
#include <string>
#include <iostream>
#include <tuple>
#include <algorithm>

#include "clew.h"
#include "debug.h"
#include <omp.h>


const int CLPRESENT = 0 == clewInit();
cl_program load_cl_programs(cl_context context); 
//extern int global_id;
//const int num_threads = omp_get_max_threads();
extern int* global_ids;
extern int global_size;
#define get_global_id global_ids[omp_get_thread_num()] +
#define get_global_size global_size +
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
            global_size = N_;
            mode = noop;
            for(auto a : sizes) a = 0;
            if( !CLPRESENT) {
                printf("opencl library not found.\n");
            }
            if(!opencl_initialized) {
                self_init=true;
                init_opencl();
            }
        };
        Runner(const Runner<Args...>& r) {
            self_init =false;

            hybrid_scale= r.hybrid_scale;
            N = r.N;
            mode = r.mode;
            mems = r.mems;
            map_mem = r.map_mem;
            map_buffer = r.map_buffer;
            read_mem = r.read_mem;
            sizes = r.sizes;
            flags = r.flags;
            function_name = r.function_name;
            args = r.args;
            function_pointer = r.function_pointer;
            bool opencl_initialized = true;
            bool self_init = false;
            platform = r.platform;
            device_id = r.device_id;
            commands = r.commands;
            context = r.context;
            kernel = r.kernel;
            program = r.program;

            kernelSourceSize = r.kernelSourceSize;
            global = r.global;
            local = r.local; 
            kernelSource = r.kernelSource;

        };
        ~Runner() {
            if(self_init)finish_opencl();
        }            

         
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

        void init_opencl();
        void finish_opencl();

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
        size_t multiple_of_local(size_t N1) {
            auto tmp = ((N1+local-1)/local)*local;
            return tmp;
            };
        void run_opencl(); 
        #ifdef TOO_MPI
        void run_mpi();
        #endif

#include "runner_openmp.h"

    //private:
        // scaling of opencl portion in run_hybrid() in percent
        int hybrid_scale= 25;
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


        bool opencl_initialized = false;
        bool self_init = false;
        cl_int err;
        cl_platform_id platform;
        cl_device_id device_id;
        cl_command_queue commands;
        cl_context context;
        cl_program program;
        cl_kernel kernel;
        size_t kernelSourceSize;
        size_t global=N,local;
        char  *kernelSource;
};

#include "runner_opencl.h"
#include "runner_mpi.h"
#endif