//
//  print(Cell*) function
//

#include "consVM.h"

void print(Cell* p)
{
  switch (p->type)
  {
  case Tag::ATOM_TAG:
    print((Atom*) p);
    break;
  case Tag::CONS_TAG:
    print((Cons*) p);
    break;
  case Tag::INSTR:
    print((Instr*) p);
    break;
  default:
    throw LispError("print: invalid cell type", true);
  }
}

void println(Cell* p)
{
  print(p);
  std::cout << std::endl;
}
