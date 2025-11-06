//
//  cons heap and functions
//

#include "consVM.h"

const int N_CONS = 1000 * 1000;

static Cons heap[N_CONS];
static int free_cons;

void init_cons()
{
  free_cons = 0;
}

void cons(Cell* car, Cell* cdr)
{
  push(car);
  push(cdr);
  cons();
}

void cons() {
  if (free_cons == N_CONS)
  {
    fatal("Heap space exhausted");
  }

  // Sanity checks
  validate_cell_ptr(down(1));   // car
  validate_cell_ptr(down(0));   // cdr

  Cons* p = &heap[free_cons++];
  p->type = CONS;
  p->cdr = pop();
  p->car = pop();
  push(p);
  return;
}

bool is_cons(Cell* p)
{
  return (p->type == CONS);
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
      // DEBUG
      std::cout << "Cell type: " << q->type << std::endl;

      fatal("print: invalid cell type");
    }
  }
  std::cout << ")";
}

void audit_cons()
{
  int i = 0;
  while (i < free_cons)
  {
    Cell* p = &heap[i];
    if (p->type != CONS)
    {
      fatal("audit_cons: Bad type");
    }
    if (car(p)->type != ATOM && car(p)->type != CONS) {
      fatal("audit_cons: Bad car ptr");
    }
    if (cdr(p)->type != ATOM && cdr(p)->type != CONS) {
      fatal("audit_cons: Bad cdr ptr");
    }
    i++;
  }
}
