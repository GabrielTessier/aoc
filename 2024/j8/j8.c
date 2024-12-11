
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/tools.h"
#include "../utils/list.h"

typedef struct {
  int x;
  int y;
} couple_t;

bool contient(list_t* list, couple_t* an) {
  maillon_t* m = list->d;
  while (m != NULL) {
    couple_t* v = (couple_t*) m->val;
    if (v->x == an->x && v->y == an->y) return true;
    m = m->post;
  }
  return false;
}

bool in_grid(int mx, int my, couple_t* v) {
  return v->x >= 0 && v->x < mx && v->y >= 0 && v->y < my;
}

couple_t* add_c(couple_t* c1, couple_t* c2) {
  couple_t* r = (couple_t*) malloc(sizeof(couple_t));
  r->x = c1->x + c2->x;
  r->y = c1->y + c2->y;
  return r;
}
couple_t* sub_c(couple_t* c1, couple_t* c2) {
  couple_t* r = (couple_t*) malloc(sizeof(couple_t));
  r->x = c1->x - c2->x;
  r->y = c1->y - c2->y;
  return r;
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

  int nb_case = input_size*line_size;

  int part1 = 0;
  int part2 = 0;

  list_t* an_part1 = init_list();
  list_t* an_part2 = init_list();

  for (int i=0; i<nb_case; i++) {
    couple_t pos1;
    pos1.x = i % line_size;
    pos1.y = (i-pos1.x)/line_size;
    if (str_file[pos1.y][pos1.x] != '.') {
      for (int j=i+1; j<nb_case; j++) {
        couple_t pos2;
        pos2.x = j % line_size;
        pos2.y = (j-pos2.x)/line_size;
        if (str_file[pos1.y][pos1.x] == str_file[pos2.y][pos2.x]) {
          couple_t* an1 = (couple_t*) malloc(sizeof(couple_t));
          couple_t diff;
          diff.x = pos1.x - pos2.x;
          diff.y = pos1.y - pos2.y;

          an1->x = 2*pos1.x-pos2.x;
          an1->y = 2*pos1.y-pos2.y;
          if (in_grid(line_size, input_size, an1) && contient(an_part1, an1) == false) {
            add_list(an_part1, an1, FIN);
            part1++;
          } else {
            free(an1);
          }

          couple_t* an2 = (couple_t*) malloc(sizeof(couple_t));
          an2->x = 2*pos2.x-pos1.x;
          an2->y = 2*pos2.y-pos1.y;
          if (in_grid(line_size, input_size, an2) && contient(an_part1, an2) == false) {
            add_list(an_part1, an2, FIN);
            part1++;
          } else {
            free(an2);
          }

          couple_t* an = (couple_t*) malloc(sizeof(couple_t));
          an->x = pos1.x;
          an->y = pos1.y;
          while (in_grid(line_size, input_size, an)) {
            if (!contient(an_part2, an)) {
              add_list(an_part2, an, FIN);
              part2++;
              an = add_c(an, &diff);
            } else {
              couple_t* an_t = add_c(an, &diff);
              free(an);
              an = an_t;
            }
          }
          free(an);
          an = (couple_t*) malloc(sizeof(couple_t));
          an->x = pos2.x;
          an->y = pos2.y;
          while (in_grid(line_size, input_size, an)) {
            if (!contient(an_part2, an)) {
              add_list(an_part2, an, FIN);
              part2++;
              an = sub_c(an, &diff);
            } else {
              couple_t* an_t = sub_c(an, &diff);
              free(an);
              an = an_t;
            }
          }
          free(an);
        }
      }
    }
  }

  free_list(an_part1, free);
  free_list(an_part2, free);

  for (int i=0; i<input_size; i++) free(str_file[i]);
  free(str_file);

  printf("Partie 1 : %d\n", part1);
  printf("Partie 2 : %d\n", part2);

  return 0;
}
