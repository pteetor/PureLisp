//
//  eval() and friends
//

#include "doctest.h"
#include "consVM.h"

static void assoc(Cell* x, Cell* e);
static void assoc();
static void evcon(Cell* c, Cell* e);
static void evcon();
static void pairlis(Cell* v, Cell* a, Cell* e);
static void evlis(Cell* a, Cell* b);
static void evlis();
static void apply(Cell* fn, Cell* x, Cell* a);
static void apply();

static void audit_env(Cell* a);

static void unpack(Cell*& arg1, Cell*& arg2)
{
  arg1 = down(1);
  arg2 = down(0);
}

static void unpack(Cell*& arg1, Cell*& arg2, Cell*& arg3)
{
  arg1 = down(2);
  arg2 = down(1);
  arg3 = down(0);
}

void eval(Cell* e, Cell* a)
{
  push(e);
  push(a);
  eval();
}

void eval()   // eval(e, a)
{
  Cell* e;
  Cell* a;
  unpack(e, a);

  trace("eval", e, a);

  if (is_atom(e)) {
    assoc();
    return;
  } else if (is_atom(car(e))) {
    if (car(e) == a_quote) {
      push(car(cdr(e)));
    } else if (car(e) == a_cond) {
      evcon(cdr(e), a);
    } else {
      // apply(car(e), evlis(cdr(e), a), a);
      push(car(e));
      evlis(cdr(e), a);
      push(a);
      apply();
    }
  } else {
    // apply(car(e), evlis(cdr(e), a), a);
    push(car(e));
    evlis(cdr(e), a);
    push(a);
    apply();
  }

  collapse(2);
}

static void apply(Cell* fn, Cell* x, Cell* a)
{
  push(fn);
  push(x);
  push(a);
  apply();
}

static void apply()
{
  Cell* fn; Cell* x; Cell* a;
  unpack(fn, x, a);

  trace("apply", fn, (is_atom(x) ? x : car(x)));

  if (is_atom(fn)) {
    if (fn == a_car) {
      push(car(car(x)));
    } else if (fn == a_cdr) {
      push(cdr(car(x)));
    } else if (fn == a_atom) {
      push(is_atom(car(x)) ? a_t : nil);
    } else if (fn == a_cons) {
      cons(car(x), car(cdr(x)));
    } else if (fn == a_eq) {
      push(car(x) == car(cdr(x)) ? a_t : nil);
    } else {
      eval(fn, a);
      apply(pop(), x, a);
    }
  } else if (car(fn) == a_lambda) {
    push(car(cdr(cdr(fn))));
    pairlis(car(cdr(fn)), x, a);
    eval();
  } else {
    throw LispError("apply: Not a function");
  }

  collapse(3);
}

static void assoc(Cell* a, Cell* e)
{
  push(a);
  push(e);
  assoc();
}

static void assoc()
{
  Cell* a; Cell* e;
  unpack(a, e);

  trace("assoc", a);

  while (e != nil) {

    // Sanity checks
    // if (!is_cons(e)) {
    //   println(e);
    //   fatal("assoc: Non-cons environment");
    // }
    // if (!is_cons(car(e))) {
    //   println(e);
    //   fatal("assoc: Ill-formed environment");
    // }

    if (a == car(car(e))) {
      push(cdr(car(e)));
      collapse(2);
      return;
    }
    e = cdr(e);
  }

  throw LispError("assoc: Undefined variable");
}

static void evcon(Cell* c, Cell* e)
{
  push(c);
  push(e);
  evcon();
}

static void evcon()
{
  Cell* c;
  Cell* e;
  unpack(c, e);

  trace("evcon");

  while (c != nil) {
    eval(car(car(c)), e);
    if (is_true(pop())) {
      eval(car(cdr(car(c))), e);
      collapse(2);
      return;
    }
    c = cdr(c);
  }

  throw LispError("evcon: Conditionals exhausted");
}

// a.k.a "zip"
static void pairlis(Cell* x, Cell* y, Cell* e)
{
  push(x);
  push(y);
  push(e);

  trace("pairlis",
        (is_cons(x) ? car(x) : x),
        (is_cons(y) ? car(y) : x) );

  int nPairs = 0;

  // Build a stack of <name,value> pairs
  while (x != nil && y != nil) {
    cons(car(x), car(y));
    nPairs++;
    x = cdr(x);
    y = cdr(y);
  }

  if (x != nil) {
    throw LispError("pairlis: Missing arguments");
  }
  if (y != nil) {
    throw LispError("pairlis: Too many arguments");
  }

  // Unwind the stack into a list, prepended onto 'e'
  push(e);
  while (nPairs-- > 0) {
    cons();
  }

  collapse(3);
  return;

  // -------------------------

  // if (x == nil && y == nil) {
  //   push(e);
  //   collapse(3);
  //   return;
  // }
  //
  // if (x == nil) {
  //   return fatal("pairlis: Too many arguments");
  // }
  // if (y == nil) {
  //   return fatal("pairlis: Missing arguments");
  // }
  //
  // // Sanity check
  // if (!is_atom(car(x))) {
  //   fatal("pairlis: Non-atom in parameters");
  // }
  //
  // push(x);
  // push(y);
  // push(e);
  //
  // cons(car(x), car(y));
  // pairlis(cdr(x), cdr(y), e);
  // cons();
  // collapse(3);
}

static void evlis(Cell* m, Cell* a)
{
  push(m);
  push(a);
  evlis();
}

static void evlis() {
  Cell* m;
  Cell* a;
  unpack(m, a);

  trace("evlis");

  if (m == nil) {
    push(nil);
  } else {
    // cons(eval(car(m), a), evlis(cdr(m), a) );
    eval(car(m), a);
    evlis(cdr(m), a);
    cons();
  }

  collapse(2);
}

// ------------------------------------

static void audit_env(Cell* a)
{
  while (a != nil)
  {
    if (!is_cons(a)) {
      println(a);
      throw LispError("audit_env: Ill-formed environment");
    }
    if (!is_cons(car(a))) {
      println(a);
      throw LispError("audit_env: Ill-formed pair");
    }
    if (!is_atom(car(car(a)))) {
      println(a);
      throw LispError("audit_env: Non-atom in environment");
    }
    a = cdr(a);
  }
}

// -------------------------------------

static void make_assoc(Cell* name, Cell* value)
{
  cons(name, value);
  push(nil);
  cons();
}

TEST_CASE("assoc() works") {
  Cell* name = atom("foo");
  Cell* value = atom("fum");

  make_assoc(name, value);
  Cell* env = pop();

  assoc(name, env);
  REQUIRE(top() == value);
}

TEST_CASE("pairlis() works for simple environment") {
  Cell* name = atom("x");
  cons(name, nil);
  Cell* params = pop();

  Cell* value = atom("foo");
  cons(value, nil);
  Cell* args = pop();

  pairlis(params, args, nil);
  Cell* env = pop();

  assoc(name, env);
  REQUIRE(top() == value);
}

TEST_CASE("eval can evaluate an atom") {
  Cell* name = atom("x");
  Cell* value = atom("foo");

  push(name);
  make_assoc(name, value);

  eval();
  REQUIRE(top() == value);
}

// ((lambda () (quote foo))) should yield 'foo'
// TEST_CASE("apply can handle a nullary function") {
//   Cell* foo = atom("foo");
//   Cell* expr = cons(...);
// }
