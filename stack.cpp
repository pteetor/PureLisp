//
//  Stack functions
//

#include "consVM.h"

const int STACK_SIZE = 1000;
Cell* stack[STACK_SIZE];
Cell** sp;
Cell** stack_limit;

void init_stack()
{
  sp = &stack[0] - 1;
  stack_limit = stack + STACK_SIZE;
}

Cell* top()
{
  return *sp;
}

void push(Cell* p)
{
  ++sp;
  if (sp == stack_limit) {
    fatal("push: stack overflow");
  }
  *sp = p;
}

Cell* pop()
{
  // TODO: Check for underflow
  return *sp--;
}

