#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "consVM.h"

TEST_CASE("000 - initialization") {
  init_consvm();
}

// For the heap's sake, Free, Atom, and Cons must all have same size
TEST_CASE("Heap correctly laid out") {
  REQUIRE(sizeof(Free) == sizeof(Cons));
  REQUIRE(sizeof(Free) == sizeof(Atom));
}
