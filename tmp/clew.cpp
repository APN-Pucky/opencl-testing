#include "clew.h"

int main() {
	bool clpresent = 0 == clewInit();
	if( !clpresent ) {
		throw std::runtime_error("OpenCL library not found");
	}
}