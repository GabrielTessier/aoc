
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/tools.h"

void free_grid(void** grid, int size) {
  for (int i=0; i<size; i++) free(grid[i]);
  free(grid);
}

struct case_s{
  bool mur;
  bool dir[4];
};
typedef struct case_s case_t;

int get_dir(int dx, int dy) {
  if (dx==-1) return 0;
  if (dx==1) return 1;
  if (dy==-1) return 2;
  if (dy==1) return 3;
  return -1;
}

bool loop(char** grid, int input_size, int line_size, int depx, int depy) {
  case_t** str_file = (case_t**) malloc(sizeof(case_t*)*input_size);
  for (int i=0; i<input_size; i++) str_file[i] = (case_t*) malloc(sizeof(case_t)*line_size);
  for (int i=0; i<input_size; i++) for (int j=0; j<line_size; j++) {
      if (grid[i][j] == '#') {
        str_file[i][j] = (case_t) {true, {false, false, false, false}};
      } else {
        str_file[i][j] = (case_t) {false, {false, false, false, false}};
      }
    }
  int dx = 0;
  int dy = -1;
  int x = depx;
  int y = depy;
  while (x>=0 && x<line_size && y>=0 && y<input_size) {
    if (str_file[y][x].mur) {
      x -= dx;
      y -= dy;
      int tmp = dx;
      dx = -dy;
      dy = tmp;
      str_file[y][x].dir[get_dir(dx,dy)] = true;
      x += dx;
      y += dy;
    } else {
      if (str_file[y][x].dir[get_dir(dx,dy)]) {
        free_grid((void**)str_file, input_size);
        return true;
      } else {
        str_file[y][x].dir[get_dir(dx,dy)] = true;
      }
      x += dx;
      y += dy;
    }
  }
  free_grid((void**)str_file, input_size);
  return false;
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
  char** grid = read_file_to_array(argv[1], &input_size);
  int line_size = strlen(grid[0]);
  char** str_file = (char**) malloc(sizeof(char*)*input_size);
  for (int i=0; i<input_size; i++) str_file[i] = (char*) malloc(sizeof(char)*line_size);
  for (int i=0; i<input_size; i++) for (int j=0; j<line_size; j++) str_file[i][j] = grid[i][j];
  int depx = -1;
  int depy = -1;
  for (int i=0; i<input_size; i++) {
    for (int j=0; j<line_size; j++) {
      if (str_file[i][j] == '^') {
        depy = i;
        depx = j;
        str_file[i][j] = '.';
        break;
      }
    }
    if (depx != -1) break;
  }

  int part1 = 0;
  int dx = 0;
  int dy = -1;
  int x = depx;
  int y = depy;
  while (x>=0 && x<line_size && y>=0 && y<input_size) {
    if (str_file[y][x] == '#') {
      x -= dx;
      y -= dy;
      int tmp = dx;
      dx = -dy;
      dy = tmp;
      x += dx;
      y += dy;
    } else {
      if (str_file[y][x] == '.') part1++;
      str_file[y][x] = 'X';
      x += dx;
      y += dy;
    }
  }
  free_grid((void**)str_file, input_size);

  int part2 = 0;
  for (int mx=0; mx<line_size; mx++) {
    for (int my=0; my<input_size; my++) {
      if (grid[my][mx] == '.') {
        grid[my][mx] = '#';
        if (loop(grid, input_size, line_size, depx, depy)) {
          part2++;
        }
        grid[my][mx] = '.';
      }
    }
  }
  free_grid((void**)grid, input_size);

  printf("Partie 1 : %d\n", part1);
  printf("Partie 2 : %d\n", part2);

  return 0;
}
