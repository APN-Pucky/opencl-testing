#ifndef DEBUG_H_INCLUDED
#define DEBUG_H_INCLUDED

#ifndef NDEBUG
#define debug_printf(...) printf(__VA_ARGS__);fflush(stdout);
//#define debug_printf(format, ...)                                  \
//    {                                                                   \
//            printf(":%i: " format "\n", __LINE__ , __VA_ARGS__);\
//    }
#else
#define debug_printf(...)
#endif
#endif