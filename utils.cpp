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
    throw LispError("validate_cell_ptr: NULL pointer", true);
  }

  switch (p->type)
  {
  case Tag::ATOM_TAG:
    break;
  case Tag::CONS_TAG:
    if (car(p) == NULL)
    {
      throw LispError("validate_cell_ptr: NULL car ptr", true);
    }
    if (cdr(p) == NULL)
    {
      throw LispError("validate_cell_ptr: NULL cdr ptr", true);
    }
    if (car(p)->type != Tag::ATOM_TAG && car(p)->type != Tag::CONS_TAG)
    {
      throw LispError("validate_cell_ptr: Bad car ptr", true);
    }
    if (cdr(p)->type != Tag::ATOM_TAG && cdr(p)->type != Tag::CONS_TAG)
    {
      throw LispError("validate_cell_ptr: Bad cdr ptr", true);
    }
    break;
  default:
    LispError("validate_cell_ptr: Invalid cell tag", true);
  }
}
