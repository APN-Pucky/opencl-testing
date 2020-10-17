#ifndef OCL_H
#define OCL_H

#ifndef _OpenCL
#define __global
#define __constant
#define __kernel
#define get_global_id
#define ulong unsigned long
#include <cstdio>

#else

#define bool int
#define false 0
#define true 1

#endif

#endif
