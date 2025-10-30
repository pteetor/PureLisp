//
// ConsVM - main
//

#include "consVM.h"

int main(int argc, char** argv)
{
  init_atoms();
  init_cons();
  init_reader();

  print(nil);
  std::cout << std::endl << std::endl;

  print(cons(a_atom, cons(nil, nil)));
  std::cout << std::endl;

  // Cell* p = read();
  // print(p);
}
