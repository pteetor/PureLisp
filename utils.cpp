//
//  Utility functions
//

#include "consVM.h"

//
// Global variables
//

bool tracing = false;

//
// Global functions
//

Cell* fatal(const char* msg, Cell* cell)
{
  std::cerr << msg;
  if (cell != NULL) {
    if (is_atom(cell)) {
      std::cout << " [";
      print(cell);
      std::cout << "]";
    }
  }
  std::cout << std::endl;
  std::exit(1);
  return nil;   // Keep compiler happy
}

bool is_true(Cell* p)
{
  return (p != nil);
}

void init_tracing()
{
  const char* TRACE = std::getenv("TRACE");
  if (TRACE == NULL) {
    tracing = false;
  } else if (std::strcmp(TRACE, "on") == 0) {
    tracing = true;
  } else {
    tracing = false;
  }
}

void trace(const char* tag, Cell* cell, Cell* cell2)
{
  if (tracing) {
    std::cout << "[trace] " << tag;
    if (cell != NULL) {
      std::cout << " [";
      if (is_atom(cell)) {
        print(cell);
      }
      std::cout << "]";
    }
    if (cell2 != NULL) {
      std::cout << " [";
      if (is_atom(cell2)) {
        print(cell2);
      }
      std::cout << "]";
    }
    std::cout << std::endl;
  }
}

void validate_cell_ptr(Cell* p)
{
  if (p == NULL) {
    fatal("validate_cell_ptr: NULL pointer");
  }
  if (p->type != ATOM && p->type != CONS)
  {
    fatal("validate_cell_ptr: Bad type");
  }
  if (p->type == CONS) {
    if (car(p) == NULL)
    {
      fatal("validate_cell_ptr: NULL car ptr");
    }
    if (cdr(p) == NULL)
    {
      fatal("validate_cell_ptr: NULL cdr ptr");
    }
    if (car(p)->type != ATOM && car(p)->type != CONS)
    {
      fatal("validate_cell_ptr: Bad car ptr");
    }
    if (cdr(p)->type != ATOM && cdr(p)->type != CONS)
    {
      fatal("validate_cell_ptr: Bad cdr ptr");
    }
  }
}
