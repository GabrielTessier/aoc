
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/tools.h"
#include "../utils/list.h"

int nb9(char** grid, int x, int y, int mx, int my, list_t* trouver) {
  if (grid[y][x] == '9' && !in_list(trouver, (void*) (intptr_t) (x+y*mx))) {
    add_list(trouver, (void*) (intptr_t) (x+y*mx), FIN);
    return 1;
  }
  int val = 0;
  if (x!=0 && grid[y][x-1] == grid[y][x]+1) val += nb9(grid, x-1, y, mx, my, trouver);
  if (x!=mx-1 && grid[y][x+1] == grid[y][x]+1) val += nb9(grid, x+1, y, mx, my, trouver);
  if (y!=0 && grid[y-1][x] == grid[y][x]+1) val += nb9(grid, x, y-1, mx, my, trouver);
  if (y!=my-1 && grid[y+1][x] == grid[y][x]+1) val += nb9(grid, x, y+1, mx, my, trouver);
  return val;
}

typedef struct {
  int* path;
  int size;
} path_t;

bool contient(list_t* trouver, path_t* path) {
  maillon_t* m = trouver->d;
  while (m!=NULL) {
    path_t* v = m->val;
    if (v->size == path->size) {
      bool eq = true;
      for (int i=0; i<path->size; i++) {
        if (v->path[i] != path->path[i]) {
          eq = false;
          break;
        }
      }
      if (eq) return true;
    }
    m = m->post;
  }
  return false;
}

void free_path(path_t* path) {
  free(path->path);
  free(path);
}

int score(char** grid, int x, int y, int mx, int my, list_t* trouver, path_t* path) {
  if (grid[y][x] == '9' && !contient(trouver, path)) {
    path_t* cpath = (path_t*) malloc(sizeof(path_t));
    cpath->path = (int*) malloc(sizeof(int)*(path->size));
    for (int i=0; i<path->size; i++) cpath->path[i] = path->path[i];
    cpath->size = path->size;
    add_list(trouver, (void*) cpath, FIN);
    return 1;
  }
  int val = 0;
  for (int i=0; i<4; i++) {
    int nx = x;
    int ny = y;
    switch (i) {
    case 0: nx--;break;
    case 1: nx++;break;
    case 2: ny--;break;
    case 3: ny++;break;
    }
    if (nx>=0 && nx<mx && ny>=0 && ny<my && grid[ny][nx] == grid[y][x]+1) {
      path_t* npath = (path_t*) malloc(sizeof(path_t));
      npath->path = (int*) malloc(sizeof(int)*(path->size+1));
      for (int i=0; i<path->size; i++) npath->path[i] = path->path[i];
      npath->path[path->size] = nx+ny*mx;
      npath->size = path->size+1;
      val += score(grid, nx, ny, mx, my, trouver, npath);
      free_path(npath);
    }
  }
  return val;
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

  // PARTIE 1

  unsigned long part1 = 0;
  for (int y=0; y<input_size; y++) {
    for (int x=0; x<line_size; x++) {
      if (str_file[y][x] == '0') {
        list_t* trouver = init_list();
        part1 += nb9(str_file, x, y, line_size, input_size, trouver);
        free_list(trouver, NULL);
      }
    }
  }
  printf("Partie 1 : %lu\n", part1);

  // PARTIE 2
  unsigned long part2 = 0;
  for (int y=0; y<input_size; y++) {
    for (int x=0; x<line_size; x++) {
      if (str_file[y][x] == '0') {
        list_t* trouver = init_list();
        path_t* p = (path_t*) malloc(sizeof(path_t));
        p->path = NULL;
        p->size = 0;
        part2 += score(str_file, x, y, line_size, input_size, trouver, p);
        free_path(p);
        free_list(trouver, (void (*) (void*))free_path);
      }
    }
  }
  printf("Partie 2 : %lu\n", part2);


  for (int i=0; i<input_size; i++) free(str_file[i]);
  free(str_file);

  return 0;
}
