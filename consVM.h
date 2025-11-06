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

// Pre-defined functions are stored in global_env
extern Cell* global_env;

extern bool tracing;

//
// Global functions ----
//

extern void init_cons();
extern void cons();
extern void cons(Cell* car, Cell* cdr);
extern Cell* car(Cell* p);
extern Cell* cdr(Cell* p);
extern void print(Cons* p);
extern bool is_cons(Cell* p);
extern void audit_cons();

extern void init_atoms();
extern Atom* atom(const char* p);
extern Atom* atom(const char* p, int len);
extern void print(Atom* p);
extern bool is_atom(Cell* p);
extern void audit_atoms();

extern void init_stack();
extern Cell* top();
extern void push(Cell*);
extern Cell* pop();
extern Cell* down(int n);
extern void drop(int n);
extern void collapse(int n);

extern void print(Cell* p);
extern void println(Cell* p);

extern void build_globals();
extern void eval();
extern void eval(Cell* a, Cell* e);

extern bool is_true(Cell*);
extern Cell* fatal(const char* msg, Cell* cell = NULL);
extern void init_tracing();
extern void trace(const char* tag,
                  Cell* cell = NULL, Cell* cell2 = NULL);
extern void validate_cell_ptr(Cell* p);

extern bool read(bool top_level = false);
