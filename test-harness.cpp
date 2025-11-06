#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "consVM.h"

TEST_CASE("000 - initialization") {
    init_atoms();
    init_cons();
    init_stack();
    build_globals();
}

TEST_CASE("testing car()") {
  cons(nil, a_t);
  REQUIRE(car(top()) == nil);
}

TEST_CASE("testing cdr()") {
  cons(nil, a_t);
  REQUIRE(cdr(top()) == a_t);
}
