//
//  eval() and friends
//

#include "doctest.h"
#include "consVM.h"

static Cell* assoc(Cell* x, Cell* e);
static Cell* evcon(Cell* c, Cell* e);
static Cell* pairlis(Cell* v, Cell* a, Cell* e);
static Cell* evlis(Cell* a, Cell* b);
static Cell* apply(Cell* fn, Cell* x, Cell* a);
static void audit_env(Cell* a);

Cell* eval(Cell* e, Cell* a)
{
  trace("eval", e);

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
  trace("assoc", x, e);

  if (e == nil) {
    return fatal("assoc: Undefined variable", x);
  }

  // Sanity checks
  // if (!is_cons(e)) {
  //   println(e);
  //   fatal("assoc: Non-cons environment");
  // }
  // if (!is_cons(car(e))) {
  //   println(e);
  //   fatal("assoc: Ill-formed environment");
  // }

  if (x == car(car(e))) {
    return cdr(car(e));
  } else {
    return assoc(x, cdr(e));
  }
}

static Cell* evcon(Cell* c, Cell* e)
{
  trace("evcon");

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
  trace("pairlis",
        (is_cons(x) ? car(x) : x),
        (is_cons(y) ? car(y) : x) );

  if (x == nil && y == nil) {
    return e;
  }
  if (x == nil) {
    return fatal("pairlis: Too many arguments");
  }
  if (y == nil) {
    return fatal("pairlis: Missing arguments");
  }

  // Sanity check
  if (!is_atom(car(x))) {
    fatal("pairlis: Non-atom in parameters");
  }

  return cons(cons(car(x), car(y)),
              pairlis(cdr(x), cdr(y), e) );
}

static Cell* evlis(Cell* m, Cell* a)
{
  trace("evlis");

  if (m == nil) {
    return nil;
  }
  return cons(eval(car(m), a),
              evlis(cdr(m), a) );
}

static Cell* apply(Cell* fn, Cell* x, Cell* a)
{
  trace("apply", fn, (is_atom(x) ? x : car(x)));

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

// ------------------------------------

static void audit_env(Cell* a)
{
  while (a != nil)
  {
    if (!is_cons(a)) {
      println(a);
      fatal("audit_env: Ill-formed environment");
    }
    if (!is_cons(car(a))) {
      println(a);
      fatal("audit_env: Ill-formed pair");
    }
    if (!is_atom(car(car(a)))) {
      println(a);
      fatal("audit_env: Non-atom in environment");
    }
    a = cdr(a);
  }
}

// -------------------------------------

Cell* make_assoc(Cell* name, Cell* value)
{
  return cons(cons(name, value), nil);
}

TEST_CASE("test assoc()") {
  Cell* foo = atom("foo");
  Cell* fum = atom("fum");
  Cell* env = cons(cons(foo, fum), nil);

  CHECK(assoc(foo, env) == fum);
}

TEST_CASE("eval can evaluate an atom") {
  Cell* name = atom("x");
  Cell* value = atom("foo");
  Cell* env = make_assoc(name, value);

  CHECK(eval(name, env) == value);
}

// ((lambda () (quote foo))) should yield 'foo'
// TEST_CASE("apply can handle a nullary function") {
//   Cell* foo = atom("foo");
//   Cell* expr = cons(...);
// }
