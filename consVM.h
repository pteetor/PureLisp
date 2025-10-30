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
extern Atom* a_t, *a_quote, *a_cond, *a_atom, *a_car, *a_cdr, *a_cons, *a_eq;

//
// Global functions ----
//

extern void init_cons();
extern Cons* cons(Cell* car, Cell* cdr);
extern Cell* car(Cell* p);
extern Cell* cdr(Cell* p);
extern void print(Cons* p);

extern void init_atoms();
extern Atom* atom(const char* p);
extern Atom* atom(const char* p, int len);
extern void print(Atom* p);

extern void print(Cell* p);

extern void fatal(const char* msg);

//
// read() function
//
extern void init_reader();
extern Cell* read();

