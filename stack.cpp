//
//  Stack functions
//
// NOTE: This stacks grows *downward*, not upward.
//

#include "consVM.h"

const int STACK_SIZE = 1000;
Cell* stack[STACK_SIZE];
Cell** sp;
Cell** stack_base;

void init_stack()
{
  stack_base = stack + STACK_SIZE;
  sp = stack_base;
}

Cell* top()
{
  if (sp == stack_base) {
    fatal("top: stack underflow");
  }
  return *sp;
}

void push(Cell* p)
{
  --sp;
  if (sp == stack) {
    fatal("push: stack overflow");
  }
  *sp = p;
}

Cell* pop()
{
  if (sp == stack_base) {
    fatal("pop: stack underflow");
  }
  return *sp++;
}

