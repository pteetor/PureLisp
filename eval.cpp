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
  if (is_atom(e)) {
    return assoc(e, a);
  }
  if (is_atom(car(e))) {
    if (car(e) == a_quote) {
      return car(cdr(e));
    }
    if (car(e) == a_cond) {
      return evcon(cdr(e), a);
    }
    return apply(car(e), evlis(cdr(e), a), a);
  }
  return apply(car(e), evlis(cdr(e), a), a);
}

static Cell* assoc(Cell* x, Cell* e)
{
  if (e == nil) {
    return fatal("assoc: Undefined variable");
  } else if (x == car(car(e))) {
    return cdr(car(e));
  } else {
    return assoc(x, cdr(e));
  }
}

static Cell* evcon(Cell* c, Cell* e)
{
  if (c == nil) {
    fatal("evcon: Conditionals exhausted");
  }

  if (is_true(eval(car(car(c)), e))) {
    return eval(car(cdr(car(c))), e);
  } else {
    return evcon(cdr(c), e);
  }
}

// a.k.a "zip"
static Cell* pairlis(Cell* x, Cell* y, Cell* e)
{
  if (x == nil && y == nil) {
      return e;
    }
  if (x == nil || y == nil) {
    return fatal("pairlis: Arguments/parameters mismatch");
  }
  return cons(cons(car(x), car(y)),
                pairlis(cdr(x), cdr(y), e) );
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
  if (is_atom(fn)) {
    if (fn == a_car) {
      return car(car(x));
    } else if (fn == a_cdr) {
      return cdr(car(x));
    } else if (fn == a_atom) {
      return (is_atom(car(x)) ? a_t : nil);
    } else if (fn == a_cons) {
      return cons(car(x), car(cdr(x)));
    } else if (fn == a_eq) {
      return (car(x) == car(cdr(x)) ? a_t : nil);
    } else {
      return apply(eval(fn,a), x, a);
    }
  } else if (car(fn) == a_lambda) {
    return eval(car(cdr(cdr(fn))), pairlis(car(cdr(fn)), x, a));
  } else {
    return fatal("apply: Not a function");
  }
}
