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
    std::cout << "[trace] " << tag << std::endl;
    if (cell != NULL) {
      std::cout << "   arg1: "; print(cell); std::cout << std::endl;
    }
    if (cell2 != NULL) {
      std::cout << "   arg2: "; print(cell2); std::cout << std::endl;
    }
    std::cout << std::endl;
  }
}

void validate_cell_ptr(Cell* p)
{
  if (p == NULL) {
    fatal("validate_cell_ptr: NULL pointer");
  }

  switch (p->type)
  {
  case ATOM_TAG:
    break;
  case CONS_TAG:
    if (car(p) == NULL)
    {
      fatal("validate_cell_ptr: NULL car ptr");
    }
    if (cdr(p) == NULL)
    {
      fatal("validate_cell_ptr: NULL cdr ptr");
    }
    if (car(p)->type != ATOM_TAG && car(p)->type != CONS_TAG)
    {
      fatal("validate_cell_ptr: Bad car ptr");
    }
    if (cdr(p)->type != ATOM_TAG && cdr(p)->type != CONS_TAG)
    {
      fatal("validate_cell_ptr: Bad cdr ptr");
    }
    break;
  default:
    fatal("validate_cell_ptr: Invalid cell tag");
  }
}
