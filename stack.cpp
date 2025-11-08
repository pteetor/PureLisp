//
//  Stack functions
//
// NOTE: This stacks grows *downward*, not upward.
//

#include "doctest.h"
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
    throw LispError("top: stack underflow", true);
  }
  return *sp;
}

void push(Cell* p)
{
  --sp;
  if (sp == stack) {
    throw LispError("push: stack overflow", true);
  }
  *sp = p;
}

Cell* pop()
{
  if (sp == stack_base) {
    throw LispError("pop: stack underflow", true);
  }
  return *sp++;
}

Cell* down(int n)
{
  return sp[n];
}

void drop(int n)
{
  sp += n;
}

void collapse(int n)
{
  Cell* p = pop();
  drop(n);
  push(p);
}

void mark_stack()
{
  Cell** p = sp;
  while (p < stack_base)
  {
    mark(*p);
    p++;
  }
}

// -------------------------------------

//
// Unit tests
//

TEST_CASE("down() works") {
  Cell* x = atom("x");
  Cell* y = atom("y");
  Cell* z = atom("z");

  push(x);
  push(y);
  push(z);

  REQUIRE(down(0) == z);
  REQUIRE(down(1) == y);
  REQUIRE(down(2) == x);
}

TEST_CASE("drop() works") {
  Cell* x = atom("x");
  Cell* y = atom("y");
  Cell* z = atom("z");

  push(x);
  push(y);
  push(z);

  drop(2);
  REQUIRE(top() == x);
}

TEST_CASE("collapse() works") {
  Cell* w = atom("w");
  Cell* x = atom("x");
  Cell* y = atom("y");
  Cell* z = atom("z");

  push(w);
  push(x);
  push(y);
  push(z);

  REQUIRE(top() == z);
  collapse(2);
  REQUIRE(top() == z);
  REQUIRE(down(1) == w);
}
