
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/tools.h"
#include "../utils/list.h"

void print_disk(int* disk, int nb_block) {
  for (int i=0; i<nb_block; i++) {
    printf("%d, ", disk[i]);
  }
  printf("\n");
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "%s <file_name>\n", argv[0]);
    return 1;
  }
  FILE* file = fopen(argv[1], "r");
  if (file == NULL) {
    fprintf(stderr, "%s n'existe pas\n", argv[0]);
    return 1;
  }
  fclose(file);

  char line[20000];
  FILE* input = fopen(argv[1], "r");
  fscanf(input, "%[^\n]\n", line);
  fclose(input);
  int size = strlen(line);

  unsigned long part1 = 0;
  unsigned long part2 = 0;

  int nb_block = 0;
  for (int i=0; i<size; i++) nb_block += line[i] - '0';

  int* disk = (int*) malloc(sizeof(int)*nb_block);
  int dp = 0;
  int di = 0;
  bool use = true;
  for (int i=0; i<size; i++) {
    for (int j=0; j<line[i]-'0'; j++) {
      if (use) disk[dp] = di;
      else disk[dp] = -1;
      dp++;
    }
    if (use) di++;
    use = !use;
  }
  int* disk2 = (int*) malloc(sizeof(int)*nb_block);
  for (int i=0; i<nb_block; i++) disk2[i] = disk[i];

  int* last = &(disk[nb_block-1]);
  int* first = disk;
  while (*last == -1) last--;
  while (*first != -1) first++;
  while (last > first) {
    *first = *last;
    *last = -1;
    while (*last == -1) last--;
    while (*first != -1) first++;
  }

  int i = 0;
  while (disk[i] != -1) {
    part1 += i*disk[i];
    i++;
  }
  free(disk);

  printf("Partie 1 : %lu\n", part1);

  //print_disk(disk2, nb_block);

  last = &(disk2[nb_block-1]);
  while (*last == -1) last--;
  int pvm = nb_block;
  while (last >= disk2) {
    int v = *last;
    int size_v = 0;
    while (*last == v) {
      size_v++;
      last--;
      if (last < disk2) break;
    }
    last++;
    if (v >= pvm) goto _next;
    int* first = disk2;
_first:
    if (first > last) goto _next;
    while (*first != -1) first++;
    int size_p = 0;
    while (first+size_p < last && first[size_p] == -1) {
      size_p++;
    }
    //if (first+size_p >= last) goto _next;
    if (size_p < size_v) {
      first += size_p+1;
      goto _first;
    } else {
      for (int i=0; i<size_v; i++) {
        first[i] = v;
        last[i] = -1;
      }
    }
_next:
    last--;
    pvm = v;
    if (last < disk2) break;
    while (*last == -1) {
      last--;
      if (last < disk2) break;
    }

    //print_disk(disk2, nb_block);
    //printf("f : %ld, l : %ld\n", first-disk2, last-disk2);
  }

  //print_disk(disk2, nb_block);

  for (int i=0; i<nb_block; i++) {
    if (disk2[i] != -1) part2 += i*disk2[i];
  }
  free(disk2);

  printf("Partie 2 : %lu\n", part2);

  return 0;
}
