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

  init_tracing();
  init_atoms();
  init_cons();
  Cell* globals = build_globals();

  Cell* p = read(true);
  Cell* e;
  while (p != NULL) {
    std::cout << "S-expr: "; print(p); std::cout << std::endl;
    e = eval(p, globals);
    std::cout << "Eval: "; print(e); std::cout << std::endl;
    p = read(true);
  }

  // std::cout << "End of sexpr stream" << std::endl;
  return 0;
}
