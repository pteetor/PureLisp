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

bool is_cons(Cell* p)
{
  return (p->type == Tag::CONS_TAG);
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
      // DEBUG
      std::cout << "Cell type: " << (int) (q->type) << std::endl;

    throw LispError("print: invalid cell type", true);
    }
  }
  std::cout << ")";
}

// -------------------------------------

//
// Heap management and garbage collection
//

// static int sweep();
//
// static Cons heap[N_CONS];
// static Cons* free_list;
//
// void init_heap()
// {
//   free_list = &heap[0];
//
//   Cons* p;
//   for (int i = 0; i < N_CONS-1; ++i)
//   {
//     p = &heap[i];
//     p->type = Tag::CONS_TAG;
//     p->flags = 0;
//     p->car = &heap[i+1];
//     p->cdr = NULL;
//   }
//
//   p = &heap[N_CONS-1];
//   p->type = Tag::CONS_TAG;
//   p->flags = 0;
//   p->car = NULL;
//   p->cdr = NULL;
// }
//
// Cons* alloc_heap()
// {
//   if (free_list == NULL) {
//     GCStatus status = gc();
//     std::cout <<
//       "gc: Heap size " << status.heap_size <<
//         ", marked " << status.n_marked <<
//           ", recovered " << status.n_recovered << std::endl;
//   }
//
//   Cons* p = free_list;
//   free_list = (Cons*) free_list->car;
//   return p;
// }
//
// GCStatus gc()
// {
//   std::cout << std::endl << "gc: Starting" << std::endl;
//
//   int nMarked = mark(global_env);
//   nMarked += mark_stack();
//   int nRecovered = sweep();
//
//   if (free_list == NULL)
//   {
//     throw LispError("gc: Cons heap exhausted", true);
//   }
//
//   std::cout << "gc: Done" << std::endl;
//
//   return GCStatus(N_CONS, nMarked, nRecovered);
// }
//
// int mark(Cell* p)
// {
//   // Sanity check
//   if (p == NULL) {
//     throw LispError("gc: null pointer to mark", true);
//   }
//
//   int nMarked = 0;
//
//   if (is_cons(p)) {
//     if ((p->flags & MARK_FLAG) == 0) {
//       p->flags |= MARK_FLAG;
//       ++nMarked;
//
//       Cons* q = (Cons*) p;
//       nMarked += mark(q->car);
//       nMarked += mark(q->cdr);
//     }
//   }
//
//   return nMarked;
// }
//
// static int sweep()
// {
//   free_list = NULL;
//   int nRecovered = 0;
//
//   for (int i = 0; i < N_CONS; ++i)
//   {
//     Cons* p = &heap[i];
//     if ((p->flags & MARK_FLAG) == 0) {
//       p->car = free_list;
//       free_list = p;
//       ++nRecovered;
//     } else {
//       p->flags &= ~MARK_FLAG;
//     }
//   }
//
//   return nRecovered;
// }

// -------------------------------------

TEST_CASE("testing car()") {
  cons(nil, a_t);
  REQUIRE(car(top()) == nil);
}

TEST_CASE("testing cdr()") {
  cons(nil, a_t);
  REQUIRE(cdr(top()) == a_t);
}
