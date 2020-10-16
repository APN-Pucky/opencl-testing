#include "catch.hpp"
#include "runner.h"

void add( unsigned int a , unsigned int b , unsigned int* c  ) {
    *c = a+b;
}

void addi( int a , int b , int* c  ) {
    *c = a+b;
}

TEST_CASE( "unsigned add", "[add]" ) {
    unsigned int a=1,b=2,c=0;
    auto r = Runner("add",add,a,b,&c);
    r.run_cpu();
    REQUIRE( c == 3 );
}

TEST_CASE( "signed add", "[addi]" ) {
    int a=2,b=2,c=0;
    auto r = Runner("addi",addi,a,b,&c);
    r.run_cpu();
    REQUIRE( c == 4 );
}

TEST_CASE( "adjust params", "[addi]" ) {
    int a=2,b=2,c=0;
    auto r = Runner("addi",addi,a,b,&c);
    r.set_arg<0>(-2);
    r.run_cpu();
    REQUIRE( c == 0 );

}

TEST_CASE( "set read/write access", "[addi]" ) {
}

TEST_CASE( "opencl buffer sizes", "[addi]" ) {
}