//
// ConsVM - main
//

#include "consVM.h"

int main(int argc, char** argv)
{
  init_atoms();
  init_cons();

  Cell* p = read(true);
  Cell* e;
  while (p != NULL) {
    std::cout << "S-expr: "; print(p); std::cout << std::endl;
    e = eval(p, nil);
    std::cout << "Eval: "; print(e); std::cout << std::endl;
    p = read(true);
  }

  std::cout << "End of sexpr stream" << std::endl;
}
