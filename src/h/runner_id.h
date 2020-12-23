#include <omp.h>
extern int* global_ids;
extern int global_size;
#define get_global_id global_ids[omp_get_thread_num()] +
#define get_global_size global_size +
