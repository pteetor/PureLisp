//
//  Utility functions
//

#include "consVM.h"

//
// Global variables
//

bool tracing = false;

//
// Global functions
//

Cell* fatal(const char* msg, Cell* cell)
{
  std::cerr << msg;
  if (cell != NULL) {
    if (is_atom(cell)) {
      std::cout << " [";
      print(cell);
      std::cout << "]";
    }
  }
  std::cout << std::endl;
  std::exit(1);
  return nil;   // Keep compiler happy
}

bool is_true(Cell* p)
{
  return (p != nil);
}

void trace(const char* tag, Cell* cell)
{
  if (tracing) {
    std::cout << "[trace] " << tag;
    if (cell != NULL) {
      if (is_atom(cell)) {
        std::cout << " [";
        print(cell);
        std::cout << "]";
      }
    }
    std::cout << std::endl;
  }
}
