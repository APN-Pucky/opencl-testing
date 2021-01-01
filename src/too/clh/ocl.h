#ifndef OCL_H
#define OCL_H

#ifndef _OpenCL
//#include <cstdio>
#include "stdio.h"
//#include <iostream>
#define __private
#define __local
#define __constant
//#define __const
#define __global
#define __kernel
//#define get_global_id
//#define ulong unsigned long
#include "runner_id.h"

#else

#define bool int
#define false 0
#define true 1

#endif

#endif

