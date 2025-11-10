//
// ConsVM - main
//

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest.h"

#include <fstream>
#include "consVM.h"

int main(int argc, char** argv)
{
  std::ifstream in_file;

  if (argc > 1) {
    in_file.open(argv[1]);
    if (!in_file.is_open()) {
      std::cerr << "Error: Could not open " << argv[1] << std::endl;
      return 1;
    }
    std::cin.rdbuf(in_file.rdbuf());
  }

  init_consvm();

  for (;;) {
    try {
      if (read(true)) {
        std::cout << "s-expr: "; print(top()); std::cout << std::endl;
        push(global_env);
        eval();
        std::cout << "=> ";
        println(top());
        drop(1);
      } else {
        break;
      }
    } catch (const LispError& e) {
      std::cerr << "Error: " << e.what() << std::endl;
      if (e.is_fatal) {
        return 1;
      }
    }
  }

  // std::cout << "End of sexpr stream" << std::endl;
  return 0;
}
