#ifndef OCL_DEBUG_H
#define OCL_DEBUG_H

#undef OCL_DEBUG
#ifndef NDEBUG
#ifndef _OpenCL
#define OCL_DEBUG
//#define printf(...) printf(__VA_ARGS__);fflush(stdout);
#endif
//#define debug_printf(format, ...)                                  \
//    {                                                                   \
//            printf(":%i: " format "\n", __LINE__ , __VA_ARGS__);\
//    }
#else
//#define printf(...)
//void printf(...) {};

#endif

#endif