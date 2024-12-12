
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/tools.h"
#include "../utils/list.h"

typedef struct {
  char chr;
  bool vue;
} case_t;

bool in_grid(int x, int y, int mx, int my) {
  return x>=0 && x<mx && y>=0 && y<my;
}

void price(case_t*** cases, int x, int y, int mx, int my, unsigned long *bor, unsigned long *size) {
  if (cases[y][x]->vue) return;
  cases[y][x]->vue = true;
  (*size)++;
  if (in_grid(x-1, y, mx, my)) {
    if (cases[y][x-1]->chr != cases[y][x]->chr) (*bor)++;
    else price(cases, x-1, y, mx, my, bor, size);
  } else (*bor)++;
  if (in_grid(x+1, y, mx, my)) {
    if (cases[y][x+1]->chr != cases[y][x]->chr) (*bor)++;
    else price(cases, x+1, y, mx, my, bor, size);
  } else (*bor)++;
  if (in_grid(x, y-1, mx, my)) {
    if (cases[y-1][x]->chr != cases[y][x]->chr) (*bor)++;
    else price(cases, x, y-1, mx, my, bor, size);
  } else (*bor)++;
  if (in_grid(x, y+1, mx, my)) {
    if (cases[y+1][x]->chr != cases[y][x]->chr) (*bor)++;
    else price(cases, x, y+1, mx, my, bor, size);
  } else (*bor)++;
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

  int input_size;
  char** str_file = read_file_to_array(argv[1], &input_size);
  int line_size = strlen(str_file[0]);

  case_t*** cases = (case_t***) malloc(sizeof(case_t**)*input_size);
  for (int i=0; i<input_size; i++) {
    cases[i] = (case_t**) malloc(sizeof(case_t*)*line_size);
    for (int j=0; j<line_size; j++) {
      cases[i][j] = (case_t*) malloc(sizeof(case_t));
      cases[i][j]->chr = str_file[i][j];
      cases[i][j]->vue = false;
    }
  }

  for (int i=0; i<input_size; i++) free(str_file[i]);
  free(str_file);

  unsigned long part1 = 0;
  for (int i=0; i<input_size; i++) {
    for (int j=0; j<line_size; j++) {
      unsigned long bor = 0;
      unsigned long size = 0;
      price(cases, j, i, line_size, input_size, &bor, &size);
      part1 += bor*size;
    }
  }

  printf("Partie 1 : %lu\n", part1);

  return 0;
}
