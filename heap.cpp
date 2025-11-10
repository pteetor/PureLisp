//
// Heap management and garbage collection
//

#include "doctest.h"
#include "consVM.h"

const int N_CELLS = 1000 * 1000;;

static int sweep_heap();

static Free heap[N_CELLS];
static Free* free_list;

void init_heap()
{
  free_list = &heap[0];

  Free* p;
  for (int i = 0; i < N_CELLS-1; ++i)
  {
    p = &heap[i];
    p->type = FREE_TAG;
    p->flags = 0;
    p->next = &heap[i+1];
  }

  p = &heap[N_CELLS-1];
  p->type = FREE_TAG;
  p->flags = 0;
  p->next = NULL;
}

static Free* alloc_heap()
{
  if (free_list == NULL) {
    GCStatus status = gc();
    std::cout <<
      "gc: Heap size " << status.heap_size <<
        ", marked " << status.n_marked <<
          ", recovered " << status.n_recovered << std::endl;
  }

  Free* p = free_list;
  free_list = free_list->next;
  return p;
}

Cons* alloc_cons()
{
  Cons* p = (Cons*) alloc_heap();
  p->type = CONS_TAG;
  p->flags = 0;
  return p;
}

Atom* alloc_atom()
{
  Atom* p = (Atom*) alloc_heap();
  p->type = ATOM_TAG;
  p->flags = 0;
  return p;
}

// -------------------------------------

GCStatus gc()
{
  std::cout << std::endl << "gc: Starting" << std::endl;

  int nMarked = mark(global_env);
  nMarked += mark_stack();
  int nRecovered = sweep_heap();

  if (free_list == NULL)
  {
    throw LispError("gc: Heap exhausted", true);
  }

  std::cout << "gc: Done" << std::endl;

  return GCStatus(N_CELLS, nMarked, nRecovered);
}

int mark(Cell* p)
{
  // Sanity check
  if (p == NULL) {
    throw LispError("gc: null pointer to mark", true);
  }

  if ((p->flags & MARK_FLAG) != 0) {
    return 0;
  }

  int nMarked = 0;

  p->flags |= MARK_FLAG;
  ++nMarked;

  switch (p->type) {
  case CONS_TAG:
  {
    Cons* q = (Cons*) p;
    nMarked += mark(q->car);
    nMarked += mark(q->cdr);
  }
    break;
  case ATOM_TAG:
  {
    Atom* a = (Atom*) p;
    mark(a->string);
  }
    break;
  default:
    throw LispError("mark: unhandled cell type", true);
  }

  return nMarked;
}

static int sweep_heap()
{
  free_list = NULL;
  int nRecovered = 0;

  for (int i = 0; i < N_CELLS; ++i)
  {
    Free* p = &heap[i];
    if ((p->flags & MARK_FLAG) == 0) {
      p->next = free_list;
      free_list = p;
      ++nRecovered;
    } else {
      p->flags &= ~MARK_FLAG;
    }
  }

  return nRecovered;
}

// -------------------------------------

void audit_heap()
{
  for (int i = 0; i < N_CELLS; ++i)
  {
    Cell* p = &heap[i];
    switch (p->type)
    {
    case FREE_TAG:
    case ATOM_TAG:
    case CONS_TAG:
      break;
    default:
      throw LispError("audit_heap: Bad type", true);
    }
  }
}

// -------------------------------------

// UNIT TESTS HERE
