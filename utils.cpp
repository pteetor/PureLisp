//
//  Utility functions
//

#include "consVM.h"

Cell* fatal(const char* msg)
{
  std::cerr << msg << std::endl;
  std::exit(1);
  return nil;
}

bool is_true(Cell* p)
{
    return (p != nil);
}
