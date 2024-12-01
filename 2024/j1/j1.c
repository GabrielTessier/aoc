
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
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

void* split_to_int(void* line_v) {
  char* line = (char*) line_v;
  if (line[0] == 0) return NULL;
  int nb_int = 0;
  char** split_line = split(line, ' ', &nb_int);
  int* int_line = map_tab(split_line, nb_int, sizeof(char*), atoi_void, sizeof(int));
  for (int i=0; i<nb_int; i++) free(split_line[i]);
  free(split_line);
  return (void*) int_line;
}

void* diff(void* tab) {
  int* vals = (int*) tab;
  return (void*) (intptr_t) abs(vals[0] - vals[1]);
}

void get_input_sort(char* filename, int** t1_ret, int** t2_ret, int* input_size_ret) {
  int input_size = 0;
  char** input = read_file_to_array(filename, &input_size);
  int** data = map_tab(input, input_size, sizeof(char*), split_to_int, sizeof(int*));
  int* tab1 = malloc(sizeof(int)*input_size);
  int* tab2 = malloc(sizeof(int)*input_size);
  for (int i=0; i<input_size; i++) {
    tab1[i] = data[i][0];
    tab2[i] = data[i][1];
  }

  for (int i=0; i<input_size; i++) {
    free(input[i]);
    free(data[i]);
  }
  free(input);
  free(data);

  qsort(tab1, input_size, sizeof(int), int_comparator);
  qsort(tab2, input_size, sizeof(int), int_comparator);

  if (t1_ret != NULL) *t1_ret = tab1;
  if (t2_ret != NULL) *t2_ret = tab2;
  if (input_size_ret != NULL) *input_size_ret = input_size;
}

int part1(char* filename) {
  int *tab1, *tab2, input_size;
  get_input_sort(filename, &tab1, &tab2, &input_size);

  int** zip = malloc(sizeof(int*)*input_size);
  for (int i=0; i<input_size; i++) {
    zip[i] = malloc(sizeof(int)*2);
    zip[i][0] = tab1[i];
    zip[i][1] = tab2[i];
  }

  free(tab1);
  free(tab2);

  int* dist_tab = map_tab(zip, input_size, sizeof(int*), diff, sizeof(int));
  int somme = 0;
  for (int i=0; i<input_size; i++) somme += dist_tab[i];

  for(int i=0; i<input_size; i++) free(zip[i]);
  free(zip);
  free(dist_tab);

  return somme;
}

int nb_val_in_sort_tab(int* tab, size_t size, int val) {
  int nb = 0;
  for (size_t i=0; i<size; i++) {
    if (tab[i] == val) nb++;
    if (tab[i] > val) break;
  }
  return nb;
}

int part2(char* filename) {
  int *tab1, *tab2, input_size;
  get_input_sort(filename, &tab1, &tab2, &input_size);

  int res = 0;
  int i1 = 0;
  int i2 = 0;
  while (i1<input_size) {
    int v = tab1[i1];
    int nb1 = nb_val_in_sort_tab(tab1+i1, input_size-i1, v);
    int nb2 = nb_val_in_sort_tab(tab2+i2, input_size-i2, v);
    res += v*nb2*nb1;
    i1 += nb1;
    i2 += nb2;
  }

  free(tab1);
  free(tab2);

  return res;
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

  printf("partie 1 : %d\n", part1(argv[1]));
  printf("partie 2 : %d\n", part2(argv[1]));

  return 0;
}
