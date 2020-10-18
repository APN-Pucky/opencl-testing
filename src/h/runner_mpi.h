#ifndef RUNNER_MPI_H
#define RUNNER_MPI_H
#ifdef TOO_MPI
#include "mpi.h"
#include <vector>
#include <csignal>
#include <cstdlib>

#define MASTER 0


template<size_t I=0, typename...Args>
void send(std::tuple<Args...>& args, int dest,std::vector<size_t>& sizes) 
{
    if constexpr(sizeof...(Args) >0) {
        if(sizes[I] != 0) {
                //printf("here where i should be %d %d ",I,index);
            if constexpr(std::is_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::value) 
            {
                MPI_Send(std::get<I>(args),sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)* sizes[I], MPI_BYTE,dest,I,MPI_COMM_WORLD);
                printf("MPI: Send Array %d %ld\n",dest, I);
            }
        }
        else {
            MPI_Send(&std::get<I>(args),sizeof(typename std::tuple_element<I,std::tuple<Args...>>::type), MPI_BYTE,dest,I,MPI_COMM_WORLD);
            printf("MPI: Send %d %ld\n",dest,I);
            //printf("here where i should NAT be %d %d %d %d ",I,index,std::get<I>(args),sizeof(int));
        }
    // do things
    if constexpr(I+1 != sizeof...(Args))
        send<I+1>(args,dest,sizes);
    }
}

template<size_t I=0, typename...Args>
void recv(std::tuple<Args...>& args, int source, MPI_Status& status,std::vector<size_t>& sizes) 
{
    if constexpr(sizeof...(Args) >0) {
        if(sizes[I] != 0) {
                //printf("here where i should be %d %d ",I,index);
            if constexpr(std::is_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::value) 
            {
                MPI_Recv(std::get<I>(args),sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)* sizes[I], MPI_BYTE,source,I,MPI_COMM_WORLD,&status);
                printf("MPI: Recv Array %ld\n",I);
            }
        }
        else {
            MPI_Recv(&std::get<I>(args),sizeof(typename std::tuple_element<I,std::tuple<Args...>>::type), MPI_BYTE,source,I,MPI_COMM_WORLD,&status);
            printf("MPI: Recv %ld\n",I);
            //printf("here where i should NAT be %d %d %d %d ",I,index,std::get<I>(args),sizeof(int));
        }
    // do things
    if constexpr(I+1 != sizeof...(Args))
        recv<I+1>(args,source,status,sizes);
    }
}
void mpirunnerfinalize(); 
void mpirunnerinit(); 

template<typename ...Args>
void Runner<Args...>::run_mpi()
{
    int numtasks,taskid,leftover,chunksize;
    MPI_Status status;
    mpirunnerinit();
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    printf ("MPI task %d has started...  ", taskid);
    chunksize = (N / numtasks);
    leftover = (N % numtasks);

    if(taskid == MASTER) {
        printf("MASTER: ");
        std::apply([](auto&... a) {((std::cout << a << " "), ...);}, args);
        std::cout << std::endl;


        for(int i = 1 ; i  < numtasks;++i){
            send<0,Args...>(args,i,sizes);
        } 
        //run_opencl();
        run_cpu();
    }

    if(taskid > MASTER) {
        printf("NOT MASTER: ");
        std::apply([](auto&&... a) {((std::cout << a  << " "), ...);}, args);
        std::cout << std::endl;
        recv<0,Args...>(args,MASTER,status,sizes);
        std::apply([](auto&&... a) {((std::cout << a  << " "), ...);}, args);
        std::cout << std::endl;

        //run_opencl();
        run_cpu();
    }
    //MPI_Barrier(MPI_COMM_WORLD);
    //MPI_Finalize();
}
#endif
#endif