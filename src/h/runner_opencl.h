#ifndef RUNNER_OPENCL_H
#define RUNNER_OPENCL_H

#include <sstream>
#include <boost/filesystem.hpp>
#include <chrono>

#include "runner.h"


template<size_t I = 0,typename ...Args>
void loop0(std::tuple<Args...>& args,cl_context& context, std::vector<size_t>& sizes,std::vector<cl_mem_flags>& flags,std::vector<cl_mem>& mems) {
    if constexpr(sizeof...(Args) >0) {
    if constexpr(std::is_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::value) 
    {
        if(sizes[I] != 0) {
            cl_int err;
            mems[I] = clCreateBuffer(context,  flags[I],  sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)* sizes[I], NULL, &err);
            debug_printf("Create %ld %d with %ld of size %ld \n",I,err,flags[I],sizes[I]);
        }
    }
    // do things
    if constexpr(I+1 != sizeof...(Args))
        loop0<I+1>(args,context,sizes,flags,mems);
    }

}
template<size_t I = 0,typename ...Args>
void loop1(std::tuple<Args...>& args,cl_command_queue& commands, std::vector<size_t>& sizes,std::vector<cl_mem>& mems,std::vector<bool>& read_mem) {
    if constexpr(sizeof...(Args) >0) {
    if constexpr(std::is_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::value) 
    {
        if(sizes[I] != 0 && !read_mem[I] ) {
            cl_int ret = clEnqueueWriteBuffer(commands, mems[I], CL_TRUE, 0, sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)* sizes[I], std::get<I>(args), 0, NULL, NULL);
            debug_printf("Write %ld %d\n",I,ret);
        }
    }
    // do things
    if constexpr(I+1 != sizeof...(Args))
        loop1<I+1>(args,commands,sizes,mems,read_mem);
    }

}
template<size_t I = 0,typename ...Args>
void loop01(std::tuple<Args...>& args,cl_context& context, cl_command_queue& commands,std::vector<size_t>& sizes, std::vector<cl_mem_flags>& flags,std::vector<cl_mem>& mems,std::vector<bool>& read_mem,std::vector<cl_mem>& map_mem,std::vector<unsigned char *>& map_buffer) {
    if constexpr(sizeof...(Args) >0) {
    if constexpr(std::is_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::value) 
    {
        if(sizes[I] != 0 ) {
            int size = sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)* sizes[I];
            cl_int err;
            debug_printf("Create %ld %d with %ld of size %ld \n",I,err,flags[I],sizes[I]);
            mems[I] = clCreateBuffer(context,  flags[I],  size, NULL, &err);
            auto tmp= clCreateBuffer(context,  flags[I] | CL_MEM_ALLOC_HOST_PTR,  sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)* sizes[I], NULL, &err);
            map_mem[I] = tmp;
            auto tmp_pointer = (unsigned char *) clEnqueueMapBuffer(commands,tmp,CL_TRUE,read_mem[I]?CL_MAP_READ:CL_MAP_WRITE,0,sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)* sizes[I],0,NULL,NULL,NULL);
            map_buffer[I] = tmp_pointer;
            if(!read_mem[I]) {
                for(int i=0; i  < size;++i) {
                    tmp_pointer[i] = ((unsigned char *)   std::get<I>(args))[i];
                }
                cl_int ret = clEnqueueWriteBuffer(commands, mems[I], CL_TRUE, 0, sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)* sizes[I], tmp_pointer, 0, NULL, NULL);
                debug_printf("Write %ld %d\n",I,ret);
            }
        }
    }
    // do things
    if constexpr(I+1 != sizeof...(Args))
        loop01<I+1>(args,context,commands,sizes,flags,mems,read_mem,map_mem,map_buffer);
    }

}
template<size_t I = 0, typename... Args>
void loop2(std::tuple<Args...>& args, cl_kernel& kernel, int& index, std::vector<size_t>& sizes, std::vector<cl_mem>& mems){
    if constexpr(sizeof...(Args) >0) {
        if(sizes[I] != 0) {
                //printf("here where i should be %d %d ",I,index);
            if constexpr(std::is_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::value) 
            {
                clSetKernelArg(kernel, index++, sizeof(cl_mem), &mems[I]);
               debug_printf("Set Pointer %ld %d",I,index);
            }
        }
        else {
            clSetKernelArg(kernel, index++, sizeof(typename std::tuple_element<I,std::tuple<Args...>>::type), &(std::get<I>(args)));
            debug_printf("Set Direct %ld %d",I,index);
            //printf("here where i should NAT be %d %d %d %d ",I,index,std::get<I>(args),sizeof(int));
        }
    // do things
    if constexpr(I+1 != sizeof...(Args))
        loop2<I+1>(args,kernel,index,sizes,mems);
    }
}
template<size_t I = 0, typename... Args>
void loop3(std::tuple<Args...>& args,cl_command_queue& commands, std::vector<size_t>& sizes,std::vector<cl_mem>& mems,std::vector<bool>& read_mem ) {
    if constexpr(sizeof...(Args) >0) {
    if constexpr(std::is_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::value) 
    {
        if(sizes[I] != 0 && read_mem[I]) {
            cl_int ret =clEnqueueReadBuffer( commands, mems[I], CL_TRUE, 0, sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)*sizes[I], std::get<I>(args), 0, NULL, NULL );
            debug_printf("Read %ld %d \n", I,ret);
            debug_printf("Read %d \n", std::get<I>(args));
            debug_printf("Read %ld %ld\n",  sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)*sizes[I],sizes[I]);
            //printf("here where i should be %d %ld %d ",I,sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type),std::get<I>(args)[1]);
        }
    }
    // do things
    if constexpr(I+1 != sizeof...(Args))
        loop3<I+1>(args,commands,sizes,mems,read_mem);
    }
}

template<size_t I = 0, typename... Args>
void loop03(std::tuple<Args...>& args,cl_command_queue& commands, std::vector<size_t>& sizes,std::vector<cl_mem>& mems,std::vector<bool>& read_mem ,std::vector<unsigned char *>& map_buffer) {
    if constexpr(sizeof...(Args) >0) {
    if constexpr(std::is_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::value) 
    {
        if(sizes[I] != 0 && read_mem[I]) {
            int size = sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)* sizes[I];
            cl_int ret =clEnqueueReadBuffer( commands, mems[I], CL_TRUE, 0, sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)*sizes[I], map_buffer[I], 0, NULL, NULL );
            for(int i=0; i  < size;++i) {
                ((unsigned char *)   std::get<I>(args))[i] = map_buffer[I][i];
            }
            debug_printf("Read %ld %d \n", I,ret);
            debug_printf("Read %d \n", std::get<I>(args));
            debug_printf("Read %ld %ld\n",  sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)*sizes[I],sizes[I]);
            //printf("here where i should be %d %ld %d ",I,sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type),std::get<I>(args)[1]);
        }
    }
    // do things
    if constexpr(I+1 != sizeof...(Args))
        loop03<I+1>(args,commands,sizes,mems,read_mem,map_buffer);
    }
}



template< typename... Args>
void Runner<Args...>::run_opencl()
{
    auto t0 = std::chrono::high_resolution_clock::now();
    if( !CLPRESENT ) {
        printf("opencl library not found.\n");
        return;
    }

    cl_int err;
    cl_platform_id platform;
    cl_device_id device_id;
    cl_command_queue commands;
    cl_context context;
    cl_program program;

    size_t kernelSourceSize;
    size_t global=N,local;
    char  *kernelSource;

    debug_printf("clGetPlatformIDs\n");
    // get first available platform and gpu and create context
    err = clGetPlatformIDs(1, &platform, NULL);
    if (err != CL_SUCCESS) {
        printf("something went wrong, errorcode %i\n", err);
    	return;
  	}

    debug_printf("clGetDeviceIDs\n");
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device_id, NULL);
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, &err);
    if (err != CL_SUCCESS) {
        printf("clCreateContext Error: %d\n",err);
    }


    // create program from buffer
    program = load_cl_programs(context);
    err = clBuildProgram(program,0,NULL,"-D _OpenCL -I clh/ -I src/clh/ -cl-std=CL2.0",NULL,NULL);
    size_t len = 0;
    cl_int ret = CL_SUCCESS;
    ret = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &len);
    char *buffer = (char*) calloc(len, sizeof(char));
    ret = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, len, buffer, NULL);


    if(err) {
        printf("%s",buffer);
        printf("BUILD Kernel error%d",err);
    }

    // read kernel source back in from program to check
    clGetProgramInfo(program, CL_PROGRAM_SOURCE, 0, NULL, &kernelSourceSize);
    kernelSource = (char*) malloc(kernelSourceSize);
    clGetProgramInfo(program, CL_PROGRAM_SOURCE, kernelSourceSize, kernelSource, NULL);
    //printf("\nKernel source:\n\n%s\n", kernelSource);
    free(kernelSource);

    commands = clCreateCommandQueue(context, device_id, 0, &err);


    cl_kernel kernel = clCreateKernel(program, function_name.c_str(),&err);
    if(err) {
        printf("Kernel name: %s",function_name.c_str());
        printf("KERNEL Kernel error%d",err);
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    debug_printf("Compile time: %d s\n" ,((std::chrono::duration<double>)(t1-t0)).count());
    /*
    loop0<0,Args...>(args,context,sizes,flags,mems);
    loop1<0,Args...>(args,commands,sizes,mems,read_mem);
    */
    loop01<0,Args...>(args,context,commands,sizes,flags,mems,read_mem,map_mem,map_buffer);

    

    int index = 0;

    loop2<0,Args...>(args,kernel, index, sizes,mems);


    clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);

    //local = 64;
    global = ((N+local-1)/local)*local;
    printf("global: %ld local %ld\n",global, local);
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
    if(err) printf("NDRANGE Kernel error%d",err);

    auto t2 = std::chrono::high_resolution_clock::now();
    debug_printf("Compile time: %d s\n" ,((std::chrono::duration<double>)(t2-t1)).count());
    clFinish(commands);
    auto t3 = std::chrono::high_resolution_clock::now();
    debug_printf("Compile time: %d s\n" ,((std::chrono::duration<double>)(t3-t2)).count());

    //loop3<0,Args...>(args,commands,sizes,mems,read_mem);
    loop03<0,Args...>(args,commands,sizes,mems,read_mem,map_buffer);

    for(int j = 0; j < sizeof...(Args);++j) {
        const int i = j;
        if(sizes[i] != 0) {
            clReleaseMemObject(mems[i]);
            clReleaseMemObject(map_mem[i]);
        }
    }

    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);
    auto t4 = std::chrono::high_resolution_clock::now();
    debug_printf("Compile time: %d s\n" ,((std::chrono::duration<double>)(t4-t3)).count());


} 
#endif