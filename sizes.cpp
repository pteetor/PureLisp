//
// Print sizes of several ConsVM data structures
//

#include <stddef.h>
#include "stdio.h"
#include "stdint.h"

#include "consVM.h"

int main()
{
  printf("sizeof(int) = %d\n", (int) sizeof(int));
  printf("sizeof(void*) = %d\n", (int) sizeof(void*));
  printf("sizeof(Cell) = %d\n", (int) sizeof(Cell));
  printf("sizeof(Cons) = %d\n", (int) sizeof(Cons));
  printf("sizeof(Atom) = %d\n", (int) sizeof(Atom));
}
