//
// ConsVM - main
//

#include "consVM.h"

int main(int argc, char** argv)
{
  init_atoms();
  init_cons();

  // print(nil);
  // std::cout << std::endl << std::endl;
  //
  // print(cons(a_atom, cons(nil, nil)));
  // std::cout << std::endl;

  Cell* p = read_sexpr(true);
  while (p != NULL) {
    std::cout << "S-expr: ";
    print(p);
    std::cout << std::endl;
    p = read_sexpr(true);
  }

  std::cout << "End of sexpr stream" << std::endl;
}
