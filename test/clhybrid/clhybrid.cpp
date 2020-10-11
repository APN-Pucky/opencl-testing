#include "clew.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include "tyrant.h"
#include "9_tuo.h"
#include <boost/filesystem.hpp>

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
int main() {
    int clpresent = 0 == clewInit();
    if( !clpresent ) {
        printf("opencl library not found.\n");
        return -1;
    }
    const unsigned int size_card=10+4*3*9+Skill::num_skills*2+2;
    const unsigned int size_all_cards = 2*size_card;
    const unsigned int size_deck=10;
    const unsigned int sims = 1024;
    const unsigned int N = sims;
    const unsigned long seed = 5;
    int err;

    cl_platform_id platform;
    cl_device_id device_id;
    cl_command_queue commands;
    cl_context context;
    cl_program program;

    size_t kernelSourceSize;
    size_t global,local;
    char  *kernelSource;

    printf("clGetPlatformIDs\n");
    // get first available platform and gpu and create context
    err = clGetPlatformIDs(1, &platform, NULL);
    if (err != CL_SUCCESS) {
        printf("something went wrong, errorcode %i\n", err);
    		return -1;
  	}

    printf("clGetDeviceIDs\n");
    clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device_id, NULL);
    context = clCreateContext(NULL, 1, &device_id, NULL, NULL, NULL);

    // create program from buffer
    program = load_cl_programs(context);
    err = clBuildProgram(program,0,NULL,"-D _OpenCL -I clh/",NULL,NULL);
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
    //printf("\nKernel source:\n\n%s\n", kernelSource);
    free(kernelSource);

    commands = clCreateCommandQueue(context, device_id, 0, &err);



    cl_kernel kernel = clCreateKernel(program, "simulate",&err);
    if(err) printf("KERNEL Kernel error%d",err);
    cl_mem input0 = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(int) * size_all_cards, NULL, NULL);
    cl_mem input1 = clCreateBuffer(context,  CL_MEM_READ_ONLY,  sizeof(int) * size_deck, NULL, NULL);
    cl_mem input2 = clCreateBuffer(context, CL_MEM_READ_ONLY, sizeof(int) * size_deck, NULL, NULL);
    cl_mem output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int) * N, NULL, NULL);



    int mydeck[size_deck];
    int enemydeck[size_deck];
    int allcards[size_all_cards];
    for( int i =0; i < size_all_cards;++i) {
        allcards[i] = 0;
    }
    allcards[0] = 2;
    allcards[1] = 2;
    allcards[2] = 2;
    allcards[4] = 2;
    allcards[5] = 2;

    allcards[size_card +0] = 1;
    allcards[size_card +1] = 1;
    allcards[size_card +2] = 1;
    allcards[size_card +4] = 1;
    allcards[size_card +5] = 1;

    int results[N];

    for(int i =0; i  < size_deck ;++i) {
	    mydeck[i] = 1;
	    enemydeck[i] = 2;
    }
    for(int i =0; i  < N; ++i) {
	    results[i] = 2;
    }


    clEnqueueWriteBuffer(commands, input0, CL_TRUE, 0, sizeof(int) * size_all_cards, allcards, 0, NULL, NULL);
    clEnqueueWriteBuffer(commands, input1, CL_TRUE, 0, sizeof(int) * size_deck, mydeck, 0, NULL, NULL);
    clEnqueueWriteBuffer(commands, input2, CL_TRUE, 0, sizeof(int) * size_deck, enemydeck, 0, NULL, NULL);

    int index = 0;
    clSetKernelArg(kernel, index++, sizeof(cl_mem), &input0);
    clSetKernelArg(kernel, index++, sizeof(unsigned int), &size_all_cards);
    clSetKernelArg(kernel, index++, sizeof(cl_mem), &input1);
    clSetKernelArg(kernel, index++, sizeof(cl_mem), &input2);
    clSetKernelArg(kernel, index++, sizeof(cl_mem), &output);
    clSetKernelArg(kernel, index++, sizeof(unsigned int), &N);
    clSetKernelArg(kernel, index++, sizeof(unsigned long), &seed);

    clGetKernelWorkGroupInfo(kernel, device_id, CL_KERNEL_WORK_GROUP_SIZE, sizeof(local), &local, NULL);
    global = N;
    err = clEnqueueNDRangeKernel(commands, kernel, 1, NULL, &global, &local, 0, NULL, NULL);
    if(err) printf("NDRANGE Kernel error%d",err);

    clFinish(commands);
    clEnqueueReadBuffer( commands, output, CL_TRUE, 0, sizeof(int) * N, results, 0, NULL, NULL );


    for (int i = 0; i  < N ; ++i) {
	    printf("%d",results[i]);
    }

    simulate(allcards,size_all_cards,mydeck,enemydeck,results,N,seed);
	printf("\nsingle run %d\n",results[0]);



    clReleaseMemObject(input1);
    clReleaseMemObject(input2);
    clReleaseMemObject(output);
    clReleaseProgram(program);
    clReleaseKernel(kernel);
    clReleaseCommandQueue(commands);
    clReleaseContext(context);
    return 0;

}