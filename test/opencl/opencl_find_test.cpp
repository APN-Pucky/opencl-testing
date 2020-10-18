// clewTest.cpp : Defines the entry point for the console application.
//

#include "clew.h"
#include <vector>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    bool clpresent = 0 == clewInit();
    if( !clpresent ) {
        cout << "opencl library not found." << endl;
        return 0;
    }
    return 0;
}
