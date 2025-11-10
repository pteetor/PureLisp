#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "consVM.h"

TEST_CASE("000 - initialization") {
  init_consvm();
}
