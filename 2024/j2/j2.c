
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
  int* int_line_p1 = malloc(sizeof(int)*(nb_int+1));
  for (int i=0; i<nb_int; i++) int_line_p1[i] = int_line[i];
  int_line_p1[nb_int] = -1;
  free(int_line);
  return (void*) int_line_p1;
}

void* est_valide(void* data_v) {
  int* data = (int*) data_v;
  int size = 0;
  while (data[size] != -1) size++;
  int dir = 0;
  int val = 1;
  int prev = -1;
  for (int i=0; i<size; i++) {
    if (prev != -1) {
      if (dir==0) dir = (data[i]>prev)?1:-1;
      if ((dir == 1 && prev >= data[i]) || (dir == -1 && prev <= data[i]) || (abs(prev-data[i]) > 3 || prev == data[i])) val = 0;
    }
    prev = data[i];
  }
  return (void*) (intptr_t) val;
}

void* est_valide_2(void* data_v) {
  int* data = (int*) data_v;
  int size = 0;
  while (data[size] != -1) size++;
  for (int r=-1; r<size; r++) {
    int dir = 0;
    int val = 1;
    int prev = -1;
    for (int i=0; i<size; i++) {
      if (i==r) continue;
      if (prev != -1) {
        if (dir==0) dir = (data[i]>prev)?1:-1;
        if ((dir == 1 && prev >= data[i]) || (dir == -1 && prev <= data[i]) || (abs(prev-data[i]) > 3 || prev == data[i])) val = 0;
      }
      prev = data[i];
    }
    if (val==1) return (void*)(intptr_t) 1;
  }
  return NULL;
}

int** get_data(char* filename, int* size_ret) {
  int input_size;
  char** input = read_file_to_array(filename, &input_size);
  int** data = map_tab(input, input_size, sizeof(char*), split_to_int, sizeof(int*));

  for(int i=0; i<input_size; i++) {
    free(input[i]);
  }
  free(input);
  if (size_ret != NULL) *size_ret = input_size;
  return data;
}

int part1(char* filename) {
  int input_size = 0;
  int** data = get_data(filename, &input_size);
  int* valide = map_tab(data, input_size, sizeof(int*), est_valide, sizeof(int));
  int somme = 0;
  for(int i=0; i<input_size; i++) {
    somme += valide[i];
    free(data[i]);
  }
  free(data);
  free(valide);
  return somme;
}

int part2(char* filename) {
  int input_size = 0;
  int** data = get_data(filename, &input_size);
  int* valide = map_tab(data, input_size, sizeof(int*), est_valide_2, sizeof(int));
  int somme = 0;
  for(int i=0; i<input_size; i++) {
    somme += valide[i];
    free(data[i]);
  }
  free(data);
  free(valide);
  return somme;
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
