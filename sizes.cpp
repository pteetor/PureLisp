//
// Print sizes of several ConsVM data structures
//

#include <stddef.h>
#include "stdio.h"
#include "stdint.h"

#include "consVM.h"

void cell_offsets()
{
    Cell x;
    char* base = (char *) &x;
    int type_offset = (int) ((char*) &x.type - base);
    int flags_offset = (int) ((char*) &x.flags - base);

    printf("\n");
    printf("Cell:\n");
    printf("   type offset = %d\n", type_offset);
    printf("   flags offset = %d\n", flags_offset);
}

void atom_offsets()
{
    Atom x;
    char* base = (char *) &x;
    int type_offset = (int) ((char*) &x.type - base);
    int flags_offset = (int) ((char*) &x.flags - base);
    int next_offset = (int) ((char*) &x.next - base);
    int n_char_offset = (int) ((char*) &x.n_char - base);
    int string_offset = (int) ((char*) &x.string - base);

    printf("\n");
    printf("Atom:\n");
    printf("   type offset = %d\n", type_offset);
    printf("   flags offset = %d\n", flags_offset);
    printf("   next offset = %d\n", next_offset);
    printf("   n_char offset = %d\n", n_char_offset);
    printf("   string offset = %d\n", string_offset);
}

int main()
{
  printf("sizeof(int) = %d\n", (int) sizeof(int));
  printf("sizeof(void*) = %d\n", (int) sizeof(void*));
  printf("sizeof(Cell) = %d\n", (int) sizeof(Cell));
  printf("sizeof(Atom) = %d\n", (int) sizeof(Atom));
  printf("sizeof(Cons) = %d\n", (int) sizeof(Cons));
  printf("sizeof(String) = %d\n", (int) sizeof(String));

  cell_offsets();
  atom_offsets();
}
