//
//  cons heap and functions
//

#include "consVM.h"

const int N_CONS = 200;

static Cons* alloc_cons();

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
  p->type = CONS_TAG;
  p->cdr = pop();
  p->car = pop();
  push(p);
  return;
}

bool is_cons(Cell* p)
{
  return (p->type == CONS_TAG);
}

Cell* car(Cell* p)
{
  if (p->type != CONS_TAG)
  {
    fatal("car: not a cons");
  }

  return ((Cons*) p)->car;
}

Cell* cdr(Cell* p)
{
  if (p->type != CONS_TAG)
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
    case ATOM_TAG:
      std::cout << ". ";
      print((Atom*) q);
      std::cout << ")";
      return;
    case CONS_TAG:
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

// -------------------------------------

//
// Heap management and garbage collection
//

static int sweep();

static Cons heap[N_CONS];
static Cons* free_list;

void init_cons()
{
  free_list = &heap[0];

  Cons* p;
  for (int i = 0; i < N_CONS-1; ++i)
  {
    p = &heap[i];
    p->type = CONS_TAG;
    p->flags = 0;
    p->car = &heap[i+1];
    p->cdr = NULL;
  }

  p = &heap[N_CONS-1];
  p->type = CONS_TAG;
  p->flags = 0;
  p->car = NULL;
  p->cdr = NULL;
}

static Cons* alloc_cons()
{
  if (free_list == NULL) {
    GCStatus status = gc();
    std::cout <<
      "gc: Heap size " << status.heap_size <<
        ", marked " << status.n_marked <<
          ", recovered " << status.n_recovered << std::endl;
  }

  Cons* p = free_list;
  free_list = (Cons*) free_list->car;
  return p;
}

GCStatus gc()
{
  std::cout << std::endl << "gc: Starting" << std::endl;

  int nMarked = mark(global_env);
  nMarked += mark_stack();
  int nRecovered = sweep();

  if (free_list == NULL)
  {
    fatal("gc: Cons heap exhausted");
  }

  std::cout << "gc: Done" << std::endl;

  return GCStatus(N_CONS, nMarked, nRecovered);
}

int mark(Cell* p)
{
  // Sanity check
  if (p == NULL) {
    fatal("gc: null pointer to mark");
  }

  int nMarked = 0;

  if (is_cons(p)) {
    if ((p->flags & MARK_FLAG) == 0) {
      p->flags |= MARK_FLAG;
      ++nMarked;

      Cons* q = (Cons*) p;
      nMarked += mark(q->car);
      nMarked += mark(q->cdr);
    }
  }

  return nMarked;
}

static int sweep()
{
  free_list = NULL;
  int nRecovered = 0;

  for (int i = 0; i < N_CONS; ++i)
  {
    Cons* p = &heap[i];
    if ((p->flags & MARK_FLAG) == 0) {
      p->car = free_list;
      free_list = p;
      ++nRecovered;
    } else {
      p->flags &= ~MARK_FLAG;
    }
  }

  return nRecovered;
}

// -------------------------------------

void audit_cons()
{
  for (int i = 0; i < N_CONS; ++i)
  {
    Cell* p = &heap[i];
    if (p->type != CONS_TAG)
    {
      fatal("audit_cons: Bad type");
    }
  }
}
