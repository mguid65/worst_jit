#include <cstdlib>
#include <filesystem>
#include <iostream>

class SOJit {
public:
  SOJit(const std::string& lib, int flags = RTLD_LAZY) : m_lib(lib), m_flags(flags) {
    m_so_name = "jit_" + lib + ".so";
    std::string command{"g++ -O3 --shared -o " + m_so_name + " -fPIC jit_" + m_lib
                        + ".c -march=native"};

    // please ignore this
    if (system(nullptr)) {
      if (system(command.c_str())) {
        std::cerr << "Failed to JIT compile: " << m_lib << '\n';
        exit(EXIT_FAILURE);
      }
    } else {
      std::cerr << "Command system not available\n";
      exit(EXIT_FAILURE);
    }

    m_handle = dlopen(m_so_name.c_str(), flags);
    if (m_handle == nullptr) {
      std::cerr << "Null object handle\n" << dlerror();
      exit(EXIT_FAILURE);
    }
  }

  ~SOJit() {
    dlclose(m_handle);

    // also, please ignore this
    std::string command{"rm " + m_so_name};
    if (system(nullptr)) {
      if (system(command.c_str())) {
        std::cerr << "Failed to remove shared object file: " << m_so_name << " " << command << '\n';
      }
    }
  }

  void* get_handle(const char* symbol) {
    dlerror();
    void* func_handle = dlsym(m_handle, symbol);

    char* err = dlerror();
    if (err != nullptr) {
      std::cerr << "Error loading symbol: " << symbol << " " << err;
      exit(EXIT_FAILURE);
    }

    return func_handle;
  }

private:
  std::string m_lib;
  std::string m_so_name;
  void* m_handle;
  int m_flags;
};
