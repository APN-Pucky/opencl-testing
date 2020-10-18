#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include <iostream>
#include "mpi_util.h"


int main(int argc, char* argv[])
{
  for(int i = 0; i < argc; ++i)
        std::cout << argv[i] << ' ';
  std::cout << std::endl;
  global_mpi_init(&argc, &argv);
  const int result = Catch::Session().run(argc, argv);
  return result;
}