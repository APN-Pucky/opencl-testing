#include "tyrant_optimize.h"
#include <boost/timer/timer.hpp>

#ifndef TEST
int main(int argc,const char** argv)
{
#ifndef NTIMER
	boost::timer::auto_cpu_timer t;
#endif
	if (argc == 2 && strcmp(argv[1], "-version") == 0)
	{
		std::cout << "Tyrant Unleashed Optimizer " << TYRANT_OPTIMIZER_VERSION << std::endl;
		return 0;
	}
	if (argc < 2)
	{
		usage(argc, argv);
		return 255;
	}
	//init();
	start(argc,argv);
	return 0;
}
#endif