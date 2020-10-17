#ifndef RUNNER_OPENCL_H
#define RUNNER_OPENCL_H

#include <sstream>
#include <boost/filesystem.hpp>

#include "runner.h"
cl_program load_cl_programs(cl_context context) {
    std::vector<std::string> filenames;
    for (boost::filesystem::directory_entry& entry : boost::filesystem::directory_iterator("cl/"))  {
        filenames.push_back(entry.path().string());
    }
    std::sort(filenames.begin(),filenames.end());


	int n =  filenames.size();
	std::vector<std::string> files(n);
    const char** programBuffer = (const char**) malloc(n);
    size_t* programSize = ( size_t*) malloc(n);
	for(int i=0; i < n; ++i) {
        std::ifstream inFile;
		inFile.open(filenames[i]); //open the input file
    	std::stringstream strStream;
    	strStream << inFile.rdbuf(); //read the file
    	std::string str = strStream.str(); //str holds the content of the file
		files.push_back(str);
        programBuffer[i] = files.back().c_str();
        programSize[i] = files.back().length();
	}
    return clCreateProgramWithSource(context, n,
            programBuffer, programSize, NULL);
}
template<size_t I = 0,typename ...Args>
void loop0(std::tuple<Args...>& args,cl_context& context, std::vector<size_t>& sizes,std::vector<cl_mem_flags>& flags,std::vector<cl_mem>& mems) {
    if constexpr(sizeof...(Args) >0) {
    if constexpr(std::is_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::value) 
    {
        if(sizes[I] != 0) {
            cl_int err;
            mems[I] = clCreateBuffer(context,  flags[I],  sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)* sizes[I], NULL, &err);
            printf("Create %d %d \n",I,err);
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
        if(sizes[I] != 0 && !read_mem[I]) {
            cl_int ret = clEnqueueWriteBuffer(commands, mems[I], CL_TRUE, 0, sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)* sizes[I], std::get<I>(args), 0, NULL, NULL);
            printf("Write %d %d\n",I,ret);
        }
    }
    // do things
    if constexpr(I+1 != sizeof...(Args))
        loop1<I+1>(args,commands,sizes,mems,read_mem);
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
                printf("Set Pointer %d %d",I,index);
            }
        }
        else {
            clSetKernelArg(kernel, index++, sizeof(typename std::tuple_element<I,std::tuple<Args...>>::type), &(std::get<I>(args)));
            printf("Set Direct %d %d",I,index);
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
            printf("Read %d %d", I,ret);
            printf("here where i should be %d %d %d ",I,sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type),std::get<I>(args)[1]);
        }
    }
    // do things
    if constexpr(I+1 != sizeof...(Args))
        loop3<I+1>(args,commands,sizes,mems,read_mem);
    }
}

template<typename ...Args>
void Runner<Args...>::run_opencl(size_t global)
{
    if( !CLPRESENT ) {
        printf("opencl library not found.\n");
        return;
    }

    int err;

    cl_platform_id platform;
    cl_device_id device_id;
    cl_command_queue commands;
    cl_context context;
    cl_program program;

    size_t kernelSourceSize;
    size_t local;
    char  *kernelSource;

    printf("clGetPlatformIDs\n");
    // get first available platform and gpu and create context
    err = clGetPlatformIDs(1, &platform, NULL);
    if (err != CL_SUCCESS) {
        printf("something went wrong, errorcode %i\n", err);
    	return;
  	}

    printf("clGetDeviceIDs\n");
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, NULL);

    // create program from buffer
    program = load_cl_programs(context);
    err = clBuildProgram(program,0,NULL,"-D _OpenCL -I clh/ -cl-std=CL2.0",NULL,NULL);
    size_t len = 0;
    cl_int ret = CL_SUCCESS;
    ret = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, 0, NULL, &len);
    char *buffer = (char*) calloc(len, sizeof(char));
    ret = clGetProgramBuildInfo(program, device_id, CL_PROGRAM_BUILD_LOG, len, buffer, NULL);

    printf("%s",buffer);

    if(err) printf("BUILD Kernel error%d",err);

    // read kernel source back in from program to check
    clGetProgramInfo(program, CL_PROGRAM_SOURCE, 0, NULL, &kernelSourceSize);
    kernelSource = (char*) malloc(kernelSourceSize);
    clGetProgramInfo(program, CL_PROGRAM_SOURCE, kernelSourceSize, kernelSource, NULL);
    printf("\nKernel source:\n\n%s\n", kernelSource);
    free(kernelSource);

    commands = clCreateCommandQueue(context, device_id, 0, &err);


    cl_kernel kernel = clCreateKernel(program, function_name.c_str(),&err);
    if(err) {
        printf("Kernel name: %s",function_name.c_str());
        printf("KERNEL Kernel error%d",err);
    }

    for(int j = 0; j < sizeof...(Args);++j) {
        const int i = j;
        
    }

    loop0<0,Args...>(args,context,sizes,flags,mems);
    loop1<0,Args...>(args,commands,sizes,mems,read_mem);
    

    int index = 0;

    loop2<0,Args...>(args,kernel, index, sizes,mems);


    clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    printf("global: %d local %d\n",global, local);
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
    if(err) printf("NDRANGE Kernel error%d",err);

    clFinish(commands);

    loop3<0,Args...>(args,commands,sizes,mems,read_mem);

    for(int j = 0; j < sizeof...(Args);++j) {
        const int i = j;
        if(sizes[i] != 0) {
            clReleaseMemObject(mems[i]);
        }
    }

    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);
} 
#endif