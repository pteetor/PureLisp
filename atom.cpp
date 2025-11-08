//
//  atom space and functions
//

#include "consVM.h"

static const int SPACE_SIZE = 1000;
static const int WORD_SIZE = sizeof(int);

static char space[SPACE_SIZE];
static int free_space = 0;
static Atom* chain = NULL;

//
// Global variables
//
Atom* nil;
Atom* a_t;
Atom* a_quote;
Atom* a_cond;
Atom* a_atom;
Atom* a_car;
Atom* a_cdr;
Atom* a_cons;
Atom* a_eq;
Atom* a_lambda;

static Atom* find_atom(const char* p, int len);
static Atom* implied_next(Atom* p);

void init_atoms()
{
  free_space = 0;
  chain = NULL;

  nil = atom("nil");
  a_t = atom("t");
  a_quote = atom("quote");
  a_cond = atom("cond");
  a_atom = atom("atom");
  a_car = atom("car");
  a_cdr = atom("cdr");
  a_cons = atom("cons");
  a_eq = atom("eq");
  a_lambda = atom("lambda");
}

Atom* atom(const char* p)
{
  return atom(p, std::strlen(p));
}

Atom* atom(const char* p, int len)
{
  Atom* atm = find_atom(p, len);
  if (atm != NULL) {
    return atm;
  }

  // Round 'len' to next word boundary
  int string_alloc = ((len + WORD_SIZE - 1) / WORD_SIZE) * WORD_SIZE;
  int n_bytes = sizeof(Atom) + string_alloc;

  atm = (Atom*) &space[free_space];
  free_space = free_space + n_bytes;

  if (free_space >= SPACE_SIZE)
  {
    throw LispError("atom space exhausted", true);
  }

  atm->type = ATOM;
  atm->flags = 0;
  atm->next = chain;
  atm->n_char = len;
  std::memcpy(atm->string, p, len);

  // Sanity check
  // if (chain != NULL) {
  //   Atom* imp = implied_next(chain);
  //   if (atm != imp) {
  //     fatal("atom: alloc failure?");
  //   }
  // }

  chain = atm;
  return atm;
}

static Atom* find_atom(const char* p, int len)
{
  Atom* q = chain;
  while(q != NULL)
  {
    if (q->n_char == len && std::memcmp(p, q->string, len) == 0) {
      break;
    }
    q = q->next;
  }
  return q;
}

static Atom* implied_next(Atom* p)
{
  int string_alloc = ((p->n_char + WORD_SIZE - 1) / WORD_SIZE) * WORD_SIZE;
  int n_bytes = sizeof(Atom) + string_alloc;

  return (Atom*) ((char*) p + n_bytes);
}

bool is_atom(Cell* p)
{
  return (p->type == ATOM);
}

void print(Atom* p)
{
  std::cout.write(p->string, p->n_char);
}

void audit_atoms()
{
  Atom* p = chain;
  while (p != NULL) {
    if (p->type != ATOM) {
      throw LispError("audit_atoms: bad type", true);
    }
    p = p->next;
  }
}
