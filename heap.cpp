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
    p->type = Tag::FREE_TAG;
    p->flags = 0;
    p->next = &heap[i+1];
  }

  p = &heap[N_CELLS-1];
  p->type = Tag::FREE_TAG;
  p->flags = 0;
  p->next = NULL;
}

static Free* alloc_heap()
{
  if (free_list == NULL) {
    GCStatus status = gc("heap exhausted");
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
  p->type = Tag::CONS_TAG;
  p->flags = 0;
  return p;
}

Atom* alloc_atom()
{
  Atom* p = (Atom*) alloc_heap();
  p->type = Tag::ATOM_TAG;
  p->flags = 0;
  return p;
}

Instr* alloc_instr()
{
  Instr* p = (Instr*) alloc_heap();
  p->type = Tag::INSTR;
  p->flags = 0;
  return p;
}

// -------------------------------------

GCStatus gc(const char* context)
{
  std::cout << std::endl << "gc: Starting";
  if (context != NULL) {
    std::cout << " - " << context;
  }
  std::cout << std::endl;

  int nMarked = mark(global_env);
  nMarked += mark_stack();

  int nBytesRecovered = sweep_strings();
  sweep_atoms();
  int nRecovered = sweep_heap();
  compactify_strings();

  if (free_list == NULL)
  {
    throw LispError("gc: Heap exhausted", true);
  }

  std::cout << "gc: Done" << std::endl;

  // These can be removed when the code matures
  audit_atoms();
  audit_cons();
  audit_strings();

  return GCStatus(N_CELLS, nMarked, nRecovered);
}

int mark(Cell* p)
{
  // Sanity check
  if (p == NULL) {
    throw LispError("gc: null pointer to mark", true);
  }

  if (is_marked(p)) {
    return 0;
  }

  int nMarked = 0;

  set_mark(p);
  ++nMarked;

  switch (p->type) {
  case Tag::CONS_TAG:
  {
    Cons* q = (Cons*) p;
    nMarked += mark(q->car);
    nMarked += mark(q->cdr);
  }
    break;
  case Tag::ATOM_TAG:
  {
    Atom* a = (Atom*) p;
    set_mark(a->string);
  }
    break;
  default:
    throw LispError("mark: unhandled cell type", true);
  }

  return nMarked;
}

static int sweep_heap()
{
  int nRecovered = 0;
  free_list = NULL;

  for (int i = 0; i < N_CELLS; ++i)
  {
    Free* p = &heap[i];
    if (not_marked(p)) {
      p->type = Tag::FREE_TAG;
      p->next = free_list;
      free_list = p;
      ++nRecovered;
    } else {
      clear_mark(p);
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
    case Tag::FREE_TAG:
    case Tag::ATOM_TAG:
    case Tag::CONS_TAG:
      break;
    default:
      throw LispError("audit_heap: Bad type", true);
    }
  }
}

// -------------------------------------

TEST_CASE("gc() is ok for globals") {
  gc("gc() is ok for globals");

  REQUIRE(nil->type == Tag::ATOM_TAG);
  REQUIRE(a_t->type == Tag::ATOM_TAG);
}

TEST_CASE("gc() frees dead cons") {
  cons(nil, nil);
  Cell* dead = pop();

  REQUIRE(dead->type == Tag::CONS_TAG);

  gc("gc() frees dead cons");

  REQUIRE(dead->type == Tag::FREE_TAG);
}

TEST_CASE("gc() leaves mark bits clear") {
  Atom* gc_test_0 = atom("gc_test_0");
  push(gc_test_0);

  gc("gc() leaves mark bits clear");

  REQUIRE(not_marked(nil));
  REQUIRE(not_marked(a_t));
  REQUIRE(not_marked(gc_test_0));
  drop(1);
}

TEST_CASE("Strings can withstand garbage collection") {
  Atom* gc_test_1 = atom("gc_test_1");
  push(gc_test_1);
  String* garbage = intern_string("garbage");   // Will be garbage-collected
  Atom* gc_test_2 = atom("gc_test_2");
  push(gc_test_2);

  gc("Strings can withstand garbage collection");   // Should collect 'garbage' string

  REQUIRE(std::strcmp(gc_test_1->string->body, "gc_test_1") == 0);
  REQUIRE(std::strcmp(gc_test_2->string->body, "gc_test_2") == 0);
  drop(2);
}
