//
//  cons heap and functions
//

#include "doctest.h"
#include "consVM.h"

// const int N_CONS = 200;

void cons(Cell* car, Cell* cdr)
{
  push(car);
  push(cdr);
  cons();
}

void cons() {
  // Sanity checks
  validate_cell_ptr(down(1));   // car
  validate_cell_ptr(down(0));   // cdr

  Cons* p = alloc_cons();
  p->type = Tag::CONS_TAG;
  p->cdr = pop();
  p->car = pop();
  push(p);
  return;
}

Cons* as_cons(Cell* p)
{
  if (p->type != Tag::CONS_TAG) {
    LispError("as_cons: not a cons");
  }
  return (Cons*) p;
}

Cell* car(Cell* p)
{
  if (p->type != Tag::CONS_TAG)
  {
    throw LispError("car: not a cons", true);
  }

  return ((Cons*) p)->car;
}

Cell* cdr(Cell* p)
{
  if (p->type != Tag::CONS_TAG)
  {
    throw LispError("cdr: not a cons", true);
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
    case Tag::ATOM_TAG:
      std::cout << ". ";
      print((Atom*) q);
      std::cout << ")";
      return;
    case Tag::CONS_TAG:
      r = (Cons*) q;
      print(r->car);
      q = r->cdr;
      break;
    default:
      throw LispError("print: invalid cell type", true);
    }
  }
  std::cout << ")";
}

// -------------------------------------

void audit_cons()
{
    // TBD
}

// -------------------------------------

TEST_CASE("testing car()") {
  cons(nil, a_t);
  REQUIRE(car(top()) == nil);
}

TEST_CASE("testing cdr()") {
  cons(nil, a_t);
  REQUIRE(cdr(top()) == a_t);
}
