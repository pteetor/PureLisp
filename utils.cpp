//
//  Utility functions
//

#include "consVM.h"

void fatal(const char* msg)
{
  std::cerr << msg << std::endl;
  std::exit(1);
}
