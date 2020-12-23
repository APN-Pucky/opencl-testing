#include "runner_id.h"
//#include "mpi.h"



/*
template<typename T>
void Runner::add_argument(T& t ,size_t st) 
{
    this->pointers.push_back((void*) &t);
    this->sizes.push_back(st);
}
template<typename T>
void Runner::add_argument(T& t) 
{
    this->pointers.push_back((void*) &t);
    this->sizes.push_back(sizeof(T));
}
*/
//const int num_t = omp_get_max_threads();
int global_ids_a[100];
int* global_ids = &global_ids_a[0];
int global_size;
