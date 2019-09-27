#include <dlfcn.h>

#include <iostream>
#include <sstream>
#include <string>

#include "so_jit.h"

int main(int argc, const char* argv[]) {
  if (argc < 2) {
    std::cerr << "USAGE: " << argv[0] << " <some_number>\n";
    exit(EXIT_FAILURE);
  }

  std::istringstream iss(argv[1]);

  double val;
  iss >> val;

  {
    SOJit my_sqrt_jit("my_sqrt");
    double (*my_sqrt)(double) = (double (*)(double)) my_sqrt_jit.get_handle("my_sqrt");

    std::cout << "my_sqrt( " << val << " ) = " << (*my_sqrt)(val) << '\n';
  }

  return 0;
}
