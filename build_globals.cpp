//
// Build the environment of globals
//

#include "consVM.h"

Cell* build_globals()
{
  return cons(cons(nil, nil),
              cons(
                cons(a_t, a_t),
                nil ));
}
