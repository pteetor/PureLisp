//
//  eval() and friends
//

#include "consVM.h"

static Cell* assoc(Cell* x, Cell* e);
static Cell* evcon(Cell* c, Cell* e);
static Cell* pairlis(Cell* v, Cell* a, Cell* e);
static Cell* evlis(Cell* a, Cell* b);
static Cell* apply(Cell* fn, Cell* x, Cell* a);

Cell* eval(Cell* e, Cell* a)
{
  // ...
}

static Cell* assoc(Cell* x, Cell* e)
{
  if (e == nil) {
    return nil;
  } else if (x == car(car(e))) {
    return cdr(car(e));
  } else {
    return assoc(x, cdr(e));
  }
}

static Cell* evcon(Cell* c, Cell* e)
{
  if (is_true(eval(car(car(c)), e))) {
    return eval(car(cdr(car(c))), e);
  } else {
    return evcon(cdr(c), e);
  }
}

// a.k.a "zip"
static Cell* pairlis(Cell* x, Cell* y, Cell* e)
{
  if (x == nil) {
    return e;
  } else {
    return cons(cons(car(x), car(y)),
                pairlis(cdr(x), cdr(y), e) );
  }
}

static Cell* evlis(Cell* m, Cell* a)
{
  if (m == nil) {
    return nil;
  } else {
    return cons(eval(car(m), a),
                evlis(cdr(m), a) );
  }
}

static Cell* apply(Cell* fn, Cell* x, Cell* a)
{
  // ...
}
