
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include "tools.h"

void* atoi_void(void* str) {
  return (void*) (intptr_t) atoi((char*) str);
}

void* constante(void*) {
  return (void*) (intptr_t) 8;
}

int main(int argc, char** argv) {
  for (int i=0; i<argc; i++) printf("param %d : %s\n", i, argv[i]);

  int nb_int = 0;
  char** argv_split = split(argv[1], ' ', &nb_int);

  int* ntab = map_tab((void*) argv_split, nb_int, sizeof(char*), atoi_void, sizeof(int));
  for (int i=0; i<nb_int; i++) printf("%d, ", ntab[i]);
  printf("\n");
  free(ntab);
}
