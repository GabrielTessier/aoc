
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/tools.h"
#include "../utils/list.h"

typedef struct {
  long int val;
  long int nb;
} stone_t;

void add_arr(list_t* arr, long int val, long int nb) {
  maillon_t* m = arr->d;
  while (m!=NULL) {
    stone_t* stone = (stone_t*) m->val;
    if (stone->val == val) {
      stone->nb += nb;
      return;
    }
    m = m->post;
  }
  stone_t* stone = (stone_t*) malloc(sizeof(stone_t));
  stone->val = val;
  stone->nb = nb;
  add_list(arr, stone, FIN);
}

long int somme(list_t* arr) {
  long int s = 0;
  maillon_t* m = arr->d;
  while (m!=NULL) {
    stone_t* stone = (stone_t*) m->val;
    s += stone->nb;
    m = m->post;
  }
  return s;
}

list_t* blink(list_t* arr) {
  list_t* ret_arr = init_list();

  maillon_t* m = arr->d;
  while (m!=NULL) {
    maillon_t* pm = m->post;
    stone_t* stone = (stone_t*) m->val;
    int nbc = 0;
    long int tmp_v = stone->val;
    while (tmp_v != 0) {
      nbc++;
      tmp_v = (tmp_v-tmp_v%10)/10;
    }
    if (stone->val == 0) add_arr(ret_arr, 1, stone->nb);
    else if (nbc%2 == 0) {
      long int tmp_v = stone->val;
      long int n_val = 0;
      long int puissance = 1;
      for (int i=0; i<nbc/2; i++) {
        n_val += (tmp_v%10)*puissance;
        puissance *= 10;
        tmp_v = (tmp_v-tmp_v%10)/10;
      }
      long int m_val = 0;
      puissance = 1;
      for (int i=0; i<nbc/2; i++) {
        m_val += (tmp_v%10)*puissance;
        puissance *= 10;
        tmp_v = (tmp_v-tmp_v%10)/10;
      }
      add_arr(ret_arr, n_val, stone->nb);
      add_arr(ret_arr, m_val, stone->nb);
    } else add_arr(ret_arr, stone->val*2024, stone->nb);
    m = pm;
  }

  return ret_arr;
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
  int nb_int = 0;
  char** line = split(str_file[0], ' ', &nb_int);
  int* vals = (int*) map_tab(line, nb_int, sizeof(char*), atoi_void, sizeof(int));

  list_t* arr = init_list();
  for (int i=0; i<nb_int; i++) {
    free(line[i]);
    add_arr(arr, vals[i], 1);
  }
  free(line);
  free(vals);
  for (int i=0; i<input_size; i++) free(str_file[i]);
  free(str_file);

  // PARTIE 1

  for (int i=0; i<25; i++) {
    list_t* narr = blink(arr);
    free_list(arr, free);
    arr = narr;
  }
  printf("Partie 1 : %ld\n", somme(arr));

  // PARTIE 2

  for (int i=0; i<50; i++) {
    list_t* narr = blink(arr);
    free_list(arr, free);
    arr = narr;
  }
  printf("Partie 2 : %ld\n", somme(arr));

  free_list(arr, free);

  return 0;
}
