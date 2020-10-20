#ifndef RUNNER_MPI_H
#define RUNNER_MPI_H
#ifdef TOO_MPI
#include "mpi.h"
#include "mpi_util.h"
#include <vector>
#include "debug.h"

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
                debug_printf("MPI: Send Array %d %ld\n",dest, I);
            }
        }
        else {
            MPI_Send(&std::get<I>(args),sizeof(typename std::tuple_element<I,std::tuple<Args...>>::type), MPI_BYTE,dest,I,MPI_COMM_WORLD);
            debug_printf("MPI: Send %d %ld\n",dest,I);
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
                debug_printf("MPI: Recv Array %ld\n",I);
            }
        }
        else {
            MPI_Recv(&std::get<I>(args),sizeof(typename std::tuple_element<I,std::tuple<Args...>>::type), MPI_BYTE,source,I,MPI_COMM_WORLD,&status);
            debug_printf("MPI: Recv %ld\n",I);
            //printf("here where i should NAT be %d %d %d %d ",I,index,std::get<I>(args),sizeof(int));
        }
    // do things
    if constexpr(I+1 != sizeof...(Args))
        recv<I+1>(args,source,status,sizes);
    }
}

template<size_t I=0, typename...Args>
void send_result(std::tuple<Args...>& args, int dest,std::vector<size_t>& sizes,std::vector<bool>& read_mem, int taskid,int chunksize) 
{
    if constexpr(sizeof...(Args) >0) {
        if(sizes[I] != 0 && read_mem[I] == true) {
                //printf("here where i should be %d %d ",I,index);
            if constexpr(std::is_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::value) 
            {
                MPI_Send(&std::get<I>(args)[taskid*chunksize],sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)*chunksize, MPI_BYTE,dest,I,MPI_COMM_WORLD);
                debug_printf("MPI: Send Result Array %d %ld\n",dest, I);
            }
        }
    // do things
    if constexpr(I+1 != sizeof...(Args))
        send_result<I+1>(args,dest,sizes,read_mem,taskid,chunksize);
    }
}


template<size_t I=0, typename...Args>
void recv_result(std::tuple<Args...>& args, int source, MPI_Status& status,std::vector<size_t>& sizes,std::vector<bool>& read_mem,int chunksize) 
{
    if constexpr(sizeof...(Args) >0) {
        if(sizes[I] != 0 && read_mem[I] == true) {
            if constexpr(std::is_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::value) 
            {
                MPI_Recv(&std::get<I>(args)[source*chunksize],sizeof(typename std::remove_pointer<typename std::tuple_element<I,std::tuple<Args...>>::type>::type)* chunksize, MPI_BYTE,source,I,MPI_COMM_WORLD,&status);
                debug_printf("MPI: Recv Result Array %ld\n",I);
            }
        }
    // do things
    if constexpr(I+1 != sizeof...(Args))
        recv_result<I+1>(args,source,status,sizes,read_mem,chunksize);
    }
}


template<typename ...Args>
void Runner<Args...>::run_mpi()
{
    int numtasks,taskid,leftover,chunksize;
    MPI_Status status;
    global_mpi_init(NULL, NULL);
    global_id =0;
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    debug_printf ("MPI task %d has started...  ", taskid);
    chunksize = (N / numtasks);
    leftover = (N % numtasks);

    if(taskid == MASTER) {
#ifndef NDEBUG
        printf("MASTER: ");
        std::apply([](auto&... a) {((std::cout << a << " "), ...);}, args);
        std::cout << std::endl;
#endif


        //printf("tasks %d",numtasks);
        for(int i = 1 ; i  < numtasks;++i){
            send<0,Args...>(args,i,sizes);
        } 
        //run_opencl();
        //printf("main");
        for(int i = 0; i < chunksize;++i) {
            run_cpu();
            global_id++;
        }
        //printf("left");
        global_id = N-leftover;
        for(int i = 0; i < leftover;++i) {
            run_cpu();
            global_id++;
        }
        //printf("recv start");
        for( int i = 1 ; i < numtasks;++i) {
            recv_result<0,Args...>(args,i,status,sizes,read_mem,chunksize);
        }
        //printf("recv done");
    }

    if(taskid > MASTER) {

#ifndef NDEBUG
        printf("NOT MASTER: ");
        std::apply([](auto&&... a) {((std::cout << a  << " "), ...);}, args);
        std::cout << std::endl;
#endif
        recv<0,Args...>(args,MASTER,status,sizes);
#ifndef NDEBUG
        std::apply([](auto&&... a) {((std::cout << a  << " "), ...);}, args);
        std::cout << std::endl;
#endif

        //run_opencl();
        global_id = taskid*chunksize;
        for(int i = 0; i < chunksize;++i) {
            run_cpu();
            global_id++;
        }
        send_result<0,Args...>(args,MASTER,sizes,read_mem,taskid,chunksize);
        //printf("send done");
    }
    //MPI_Barrier(MPI_COMM_WORLD);
    //MPI_Finalize();
}
#endif
#endif