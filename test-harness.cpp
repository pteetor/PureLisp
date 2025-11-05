#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "consVM.h"

TEST_CASE("000 - initialization") {
    init_atoms();
    init_cons();
    Cell* globals = build_globals();
}

TEST_CASE("testing car()") {
    CHECK(car(cons(nil, a_t)) == nil);
}

TEST_CASE("testing cdr()") {
    CHECK(cdr(cons(nil, a_t)) == a_t);
}
