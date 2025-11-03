//
//  ConsVM definitions
//

#include <stddef.h>
#include <iostream>
#include <cstring>
#include <string>

#include <stdio.h>
#include <stdint.h>

const uint32_t ATOM = 1;
const uint32_t CONS = 2;

struct Cell
{
  uint32_t type;
};

struct Cons: public Cell
{
  Cell* car;
  Cell* cdr;
};

struct Atom: public Cell
{
  Atom* next;
  int n_char;
  char string[0];
};

//
// Global variables
//
extern Atom* nil;
extern Atom *a_t, *a_quote, *a_cond, *a_atom,
            *a_car, *a_cdr, *a_cons, *a_eq,
            *a_lambda;

extern bool tracing;

//
// Global functions ----
//

extern void init_cons();
extern Cons* cons(Cell* car, Cell* cdr);
extern Cell* car(Cell* p);
extern Cell* cdr(Cell* p);
extern void print(Cons* p);
extern bool is_cons(Cell* p);

extern void init_atoms();
extern Atom* atom(const char* p);
extern Atom* atom(const char* p, int len);
extern void print(Atom* p);
extern bool is_atom(Cell* p);

extern void init_stack();
extern Cell* top();
extern void push(Cell*);
extern Cell* pop();

extern void print(Cell* p);

extern Cell* build_globals();
extern Cell* eval(Cell* e, Cell* a);

extern bool is_true(Cell*);
extern Cell* fatal(const char* msg);
extern void trace(const char* tag, Cell* cell = NULL);

//
// Sexpr reader
//
extern Cell* read(bool top_level = false);
