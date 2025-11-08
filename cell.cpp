//
//  print(Cell*) function
//

#include "consVM.h"

void print(Cell* p)
{
  switch (p->type)
  {
  case ATOM_TAG:
    print((Atom*) p);
    break;
  case CONS_TAG:
    print((Cons*) p);
    break;
  default:
    // DEBUG
    std::cout << "Cell type: " << p->type << std::endl;
    fatal("print: invalid cell type");
  }
}

void println(Cell* p)
{
  print(p);
  std::cout << std::endl;
}
