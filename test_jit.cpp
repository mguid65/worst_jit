#include <string>
#include <iostream>
#include <sstream>
#include <dlfcn.h>

class SO_Jit {
 public:
  SO_Jit(const std::string& lib, int flag=RTLD_LAZY) : m_lib(lib) {
    m_fname = "jit_" + lib + ".so";
    std::string command = "gcc -O3 --shared -o " + m_fname + " -fPIC jit_" + lib + ".c -march=native";

    int err = system(command.c_str());

    m_handle = dlopen(m_fname.c_str(), flag);
  }

  ~SO_Jit() {
    dlclose(m_handle);
  }

  void* get_handle(const char* symbol) {
    return dlsym(m_handle, symbol);
  }

 private:
  std::string m_lib;
  std::string m_fname;
  void* m_handle;
};

int main(int argc, const char* argv[]) {
  double (*my_sqrt)(double);
  std::istringstream iss(argv[1]);

  double val;
  iss >> val;

  {
    SO_Jit my_sqrt_jit("my_sqrt");
    my_sqrt = (double (*)(double)) my_sqrt_jit.get_handle("my_sqrt");

    std::cout << "my_sqrt( " << val << " ) = ";

    std::cout << (my_sqrt)(val) << '\n';
  }

  return 0;
}
