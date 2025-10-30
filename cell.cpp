//
//  print(Cell*) function
//

#include "consVM.h"

void print(Cell* p)
{
  switch (p->type)
  {
  case ATOM:
    print((Atom*) p);
    break;
  case CONS:
    print((Cons*) p);
    break;
  default:
    fatal("print: invalid cell type");
  }
}
