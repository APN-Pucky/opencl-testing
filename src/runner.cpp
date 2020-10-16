#include "runner.h"


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




void add( int a , int b , int* c  ) {
    *c = a+b;
}

int main() {
    int a=1,b=2,c=0;
    auto r = Runner("add",add,a,b,&c);
    r.run_cpu();
    r.run_cpu();
    printf("%d",c);

}