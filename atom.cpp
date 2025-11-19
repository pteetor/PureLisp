//
//  atom space and functions
//

#include "consVM.h"

// static const int SPACE_SIZE = 1000;
// static const int WORD_SIZE = sizeof(int);
//
// static char space[SPACE_SIZE];
// static int free_space = 0;

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

static Atom* find_atom(const char* p);
// static Atom* implied_next(Atom* p);

void init_atoms()
{
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
  Atom* atm = find_atom(p);
  if (atm != NULL) {
    return atm;
  }

  atm = alloc_atom();
  atm->next = chain;
  atm->string = intern_string(p);
  chain = atm;
  return atm;
}

static Atom* find_atom(const char* p)
{
  Atom* q = chain;
  while(q != NULL)
  {
    if (std::strcmp(p, q->string->body) == 0) {
      break;
    }
    q = q->next;
  }
  return q;
}

Atom* as_atom(Cell* p)
{
  if (p->type != Tag::ATOM_TAG) {
    LispError("as_atom: not an atom");
  }
  return (Atom*) p;
}

void print(Atom* p)
{
  print(p->string);
}

void sweep_atoms()
{
  // Find the first atom that we want to keep
  Atom* p = chain;
  while (p != NULL) {
    if (is_marked(p)) {
      break;
    }
    p = p->next;
  }

  // That becomes the new head of the chain
  chain = p;
  Atom* q;

  // Remove all unmarked atoms from the chain
  // Find all atoms on the chain
  //   - For marked atoms, update their string pointer
  //   - For unmarked atoms, remove from chain
  while (p != NULL) {
    p->string = p->string->forward;
    q = p->next;
    while (q != NULL && not_marked(q)) {
      q = q->next;
    }
    p->next = q;
    p = q;
  }
}

// -------------------------------------

void audit_atoms()
{
    Atom* p = chain;
    int n = 0;

    while (p != NULL) {
        if (p->type != Tag::ATOM_TAG) {
            LispError("audit_atoms: bad tag");
        }
        if (p->string->type != Tag::STRING_TAG) {
            LispError("audit_atoms: bad string pointer");
        }
        ++n;
        p = p->next;
    }

    std::cout << "audit_atoms: Verified " << n << " atoms" << std::endl;
}

