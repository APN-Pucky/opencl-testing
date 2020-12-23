#ifndef RUNNER_OPENCL_H
#define RUNNER_OPENCL_H

#include <sstream>
#include <boost/filesystem.hpp>
#include <chrono>

#include "runner.h"

template< typename... Args>
cl_program Runner<Args...>::load_cl_programs(cl_context context) {
    std::vector<std::string> filenames;
    std::string path1 = "cl";
    std::string path2 = "src/cl";
    boost::filesystem::path full_path(boost::filesystem::current_path());
    debug_printf("Current path is : %s" , full_path.c_str());
    if( ! boost::filesystem::is_directory(path1) ) {
            path1 = path2;
    }
    for (boost::filesystem::directory_entry& entry : boost::filesystem::directory_iterator(path1))  {
        filenames.push_back(entry.path().string());
    }
    std::sort(filenames.begin(),filenames.end());


	int n =  filenames.size();
    const char** programBuffer = (const char**) malloc(n);
    size_t* programSize = ( size_t*) malloc(n);
    std::stringstream strStream;
	for(int i=0; i < n; ++i) {
        std::ifstream inFile;
		inFile.open(filenames[i]); //open the input file
    	strStream << inFile.rdbuf() << "\n"; //read the file
    	//std::string str = strStream.str(); //str holds the content of the file
		//files.push_back(str);
    }
    code = strStream.str();
    programBuffer[0] = code.c_str();
    //std::cout << programBuffer[0] << std::endl;
    programSize[0] = 0;//files.back().length();

    cl_int err;
    cl_program tmp = clCreateProgramWithSource(context, 1,
            programBuffer, programSize, &err);
    if(err) printf("clCreateProgramWithSource Error %d\n",err);
    return tmp;
}

template<size_t I = 0,typename ...Args>
void loop0(std::tuple<Args...>& args,cl_context& context, std::vector<size_t>& sizes,std::vector<cl_mem_flags>& flags,std::vector<cl_mem>& mems) {
    if constexpr(sizeof...(Args) >0) {
    if constexpr(std::is_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::value) 
    {
        if(sizes[I] != 0) {
            int size = sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)* sizes[I];
            cl_int err;
            mems[I] = clCreateBuffer(context,  flags[I],  size, NULL, &err);
            debug_printf("clCreateBuffer %ld %ld %ld NULL %d\n", I, flags[I],size,err);
            //debug_printf("Create %ld flag %ld of size %ld totsize %ld err %d\n",I,flags[I],sizes[I],size,err);
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
            int size = sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)* sizes[I];
            cl_int ret = clEnqueueWriteBuffer(commands, mems[I], CL_TRUE, 0, size, std::get<I>(args), 0, NULL, NULL);
            debug_printf("clEnqueueWriteBuffer %ld mems[I] CL_TRUE 0 %ld args[I] 0 NULL NULL ret=%d\n",I,size,ret);
            //debug_printf("Write %ld %d\n",I,ret);
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
            mems[I] = clCreateBuffer(context,  flags[I],  size, NULL, &err);
            debug_printf("Create %ld flag %ld of size %ld totsize %ld err %d\n",I,flags[I],sizes[I],size,err);
            auto tmp= clCreateBuffer(context,  flags[I] | CL_MEM_ALLOC_HOST_PTR,  size, NULL, &err);
            map_mem[I] = tmp;
            auto tmp_pointer = (unsigned char *) clEnqueueMapBuffer(commands,tmp,CL_TRUE,read_mem[I]?CL_MAP_READ:CL_MAP_WRITE,0,size,0,NULL,NULL,NULL);
            map_buffer[I] = tmp_pointer;
            if(!read_mem[I]) {
                for(int i=0; i  < size;++i) {
                    tmp_pointer[i] = ((unsigned char *)   std::get<I>(args))[i];
                }
                cl_int ret = clEnqueueWriteBuffer(commands, mems[I], CL_TRUE, 0, size, tmp_pointer, 0, NULL, NULL);
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
               debug_printf("clSetKernelArg %ld kernel %d sizeof(cl_mem) &mems[I]\n",I,index);
               //debug_printf("Set Pointer %ld %d",I,index);
            }
        }
        else {
            int size = sizeof(typename std::tuple_element<I,std::tuple<Args...>>::type);
            clSetKernelArg(kernel, index++, size, &(std::get<I>(args)));
            debug_printf("clSetKernelArg %ld kernel %d %ld &args[I]\n",I,index,size);
            //debug_printf("Set Direct %ld %d",I,index);
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
            int size = sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)*sizes[I];
            cl_int ret =clEnqueueReadBuffer( commands, mems[I], CL_TRUE, 0, size, std::get<I>(args), 0, NULL, NULL );
            debug_printf("clEnqueueReadBuffer %ld commands mems[I] CL_TRUE, 0, %ld args[I] 0 NULL NULL err=%d \n", I,size,ret);
            //debug_printf("Read %ld %d \n", I,ret);
            //debug_printf("Read %d \n", std::get<I>(args));
            //debug_printf("Read %ld %ld\n",  sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)*sizes[I],sizes[I]);
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
            cl_int ret =clEnqueueReadBuffer( commands, mems[I], CL_TRUE, 0, size, map_buffer[I], 0, NULL, NULL );
            for(int i=0; i  < size;++i) {
                ((unsigned char *)   std::get<I>(args))[i] = map_buffer[I][i];
            }
            debug_printf("Read %ld %d \n", I,ret);
            debug_printf("Read %d \n", std::get<I>(args));
            debug_printf("Read %ld %ld\n",  size,sizes[I]);
            //printf("here where i should be %d %ld %d ",I,sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type),std::get<I>(args)[1]);
        }
    }
    // do things
    if constexpr(I+1 != sizeof...(Args))
        loop03<I+1>(args,commands,sizes,mems,read_mem,map_buffer);
    }
}

template< typename... Args>
void Runner<Args...>::finish_opencl() {
    auto t3 = std::chrono::high_resolution_clock::now();
    debug_printf("clReleaseProgram");
    clReleaseProgram(program);
    debug_printf("clReleaseKernel");
    clReleaseKernel(kernel);
    debug_printf("clReleaseCommandQueue");
    clReleaseCommandQueue(commands);
    debug_printf("clReleaseContext");
    clReleaseContext(context);
    debug_printf("clReleaseContext done");
    auto t4 = std::chrono::high_resolution_clock::now();
    debug_printf("Cleanup time: %f s\n" ,((std::chrono::duration<double>)(t4-t3)).count());
}

template< typename... Args>
void Runner<Args...>::init_opencl()
{
    auto t0 = std::chrono::high_resolution_clock::now();
    if( !CLPRESENT ) {
        printf("opencl library not found.\n");
        return;
    }
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
    if(err) {
        printf("clBuildProgram error %d",err);
    }

    size_t len = 0;
    cl_int ret = CL_SUCCESS;
    ret = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &len);
    char *buffer = (char*) calloc(len, sizeof(char));
    ret = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, len, buffer, NULL);


    if(err) {
        printf("%s",buffer);
        printf("clGetProgranBuildInfo error %d",err);
    }

    // read kernel source back in from program to check
    clGetProgramInfo(program, CL_PROGRAM_SOURCE, 0, NULL, &kernelSourceSize);
    kernelSource = (char*) malloc(kernelSourceSize);
    clGetProgramInfo(program, CL_PROGRAM_SOURCE, kernelSourceSize, kernelSource, NULL);
    debug_printf("\nKernel source:\n\n%s\n", kernelSource);
    free(kernelSource);

    commands = clCreateCommandQueue(context, device_id, 0, &err);


    kernel = clCreateKernel(program, function_name.c_str(),&err);
    if(err) {
        printf("Kernel name: %s",function_name.c_str());
        printf("KERNEL Kernel error%d",err);
    }
    auto t1 = std::chrono::high_resolution_clock::now();
    debug_printf("Compile time: %f s\n" ,((std::chrono::duration<double>)(t1-t0)).count());

    clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);

    opencl_initialized = true;
}



template< typename... Args>
void Runner<Args...>::run_opencl()
{
    auto t1 = std::chrono::high_resolution_clock::now();

    /*
    loop0<0,Args...>(args,context,sizes,flags,mems);
    loop1<0,Args...>(args,commands,sizes,mems,read_mem);
    //*/
    loop01<0,Args...>(args,context,commands,sizes,flags,mems,read_mem,map_mem,map_buffer);

    

    int index = 0;

    loop2<0,Args...>(args,kernel, index, sizes,mems);



    //local = 64;
    global = ((N+local-1)/local)*local;
    debug_printf("N: %ld, global: %ld local %ld\n",N,global, local);
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
    if(err) printf("NDRANGE Kernel error%d",err);

    auto t2 = std::chrono::high_resolution_clock::now();
    debug_printf("MemLoad time: %f s\n" ,((std::chrono::duration<double>)(t2-t1)).count());
    clFinish(commands);
    auto t3 = std::chrono::high_resolution_clock::now();
    debug_printf("Run time: %f s\n" ,((std::chrono::duration<double>)(t3-t2)).count());

    //loop3<0,Args...>(args,commands,sizes,mems,read_mem);
    loop03<0,Args...>(args,commands,sizes,mems,read_mem,map_buffer);

    for(int j = 0; j < sizeof...(Args);++j) {
        const int i = j;
        if(sizes[i] != 0) {
            debug_printf("release mem(-map) %d \n" ,i);
            clReleaseMemObject(mems[i]);
            clReleaseMemObject(map_mem[i]);
        }
    }
} 
#endif