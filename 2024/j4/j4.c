
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/tools.h"

char** read_file_to_array(char* filename, int* size_ret) {
  FILE* input = fopen(filename, "r");
  char str[256];
  int input_size = 0;
  while (fscanf(input, "%[^\n]\n", str) != EOF) input_size++;
  fclose(input);
  char** tab = malloc(sizeof(char*)*input_size);
  input = fopen(filename, "r");
  int i = 0;
  while (fscanf(input, "%[^\n]\n", str) != EOF) {
    size_t line_size = strlen(str);
    tab[i] = malloc(sizeof(char)*(line_size+1));
    strcpy(tab[i], str);
    i++;
  }
  fclose(input);
  if (size_ret != NULL) *size_ret = input_size;
  return tab;
}

bool est_valide1(char** file, int mx, int my, int x, int y, int dx, int dy) {
  char* str = "XMAS";
  while (*str != 0) {
    if (x<0 || x>=mx || y<0 || y>=my || file[x][y] != *str) return false;
    x+=dx;
    y+=dy;
    str++;
  }
  return true;
}

bool est_valide2(char** file, int mx, int my, int x, int y, int dx, int dy) {
  if (x<1 || x>=mx-1 || y<1 || y>=my-1 || file[x][y] != 'A') return false;
  return (file[x+dx][y+dy] == 'M' && file[x-dx][y+dy] == 'M' && file[x+dx][y-dy] == 'S' && file[x-dx][y-dy] == 'S' && file[x][y] == 'A') || 
    (file[x+dx][y+dy] == 'M' && file[x+dx][y-dy] == 'M' && file[x-dx][y+dy] == 'S' && file[x-dx][y-dy] == 'S' && file[x][y] == 'A');
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
  int part1 = 0;
  int part2 = 0;
  for (int y=0; y<input_size; y++) {
    for (int x=0; x<line_size; x++) {
      for (int dx = -1; dx<=1; dx++) for (int dy = -1; dy<=1; dy++) {
          if ((dx != 0 || dy != 0) && est_valide1(str_file, line_size, input_size, x, y, dx, dy)) part1++;
        }
      int vx[2] = {-1, 1};
      int vy[2] = {-1, 1};
      bool f = false;
      for (int i=0; i<2; i++) for (int j=0; j<2; j++) {
          if (est_valide2(str_file, line_size, input_size, x, y, vx[i], vy[j]) && !f) {
            part2++;
            f = true;
          }
        }
    }
  }
  for (int i=0; i<input_size; i++) free(str_file[i]);
  free(str_file);
  printf("Partie 1 : %d\n", part1);
  printf("Partie 2 : %d\n", part2);

  return 0;
}
