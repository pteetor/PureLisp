//
//  cons heap and functions
//

#include "consVM.h"

const int N_CONS = 1000;

static Cons heap[1000];
static int free_cons;

void init_cons()
{
  free_cons = 0;
}

Cons* cons(Cell* car, Cell* cdr)
{
  if (free_cons == N_CONS)
  {
    fatal("Heap space exhausted");
  }

  Cons* p = &heap[free_cons++];
  p->type = CONS;
  p->car = car;
  p->cdr = cdr;
  return p;
}

Cell* car(Cell* p)
{
  if (p->type != CONS)
  {
    fatal("car: not a cons");
  }

  return ((Cons*) p)->car;
}

Cell* cdr(Cell* p)
{
  if (p->type != CONS)
  {
    fatal("cdr: not a cons");
  }

  return ((Cons*) p)->cdr;
}

void print(Cons* p)
{
  std::cout << "(";
  print(p->car);

  Cell* q = p->cdr;
  Cons* r;

  while (q != nil) {
    std::cout << " ";
    switch (q->type)
    {
    case ATOM:
      std::cout << ". ";
      print((Atom*) q);
      std::cout << ")";
      return;
    case CONS:
      r = (Cons*) q;
      print(r->car);
      q = r->cdr;
      break;
    default:
      fatal("print: invalid cell type");
    }
  }
  std::cout << ")";
}
