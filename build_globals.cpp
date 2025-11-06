//
// Build the environment of globals
//

#include "consVM.h"

Cell* global_env;

void build_globals()
{
  int nPairs = 0;

  cons(nil, nil);
  nPairs++;

  cons(a_t, a_t);
  nPairs++;

  push(nil);

  while (nPairs > 0)
  {
    cons();
    nPairs--;
  }

  global_env = pop();
}
