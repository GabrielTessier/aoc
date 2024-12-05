
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/tools.h"

bool valide(int** rules, int* line, int size) {
  for (int i=0; i<size; i++) {
    for (int j=i+1; j<size; j++) {
      bool in = false;
      for (int k=1; k<=rules[line[i]][0]; k++) {
        if (rules[line[i]][k] == line[j]) in = true;
      }
      if (in == false) return false;
    }
  }
  return true;
}

void corrige(int** rules, int* line, int size) {
  for (int i=0; i<size; i++) {
    for (int j=i+1; j<size; j++) {
      bool in = false;
      for (int k=1; k<=rules[line[i]][0]; k++) {
        if (rules[line[i]][k] == line[j]) in = true;
      }
      if (in == false) {
        int tmp = line[i];
        line[i] = line[j];
        line[j] = tmp;
      }
    }
  }
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
  int nb_rules = 0;
  while (str_file[nb_rules][2] == '|') nb_rules++;
  int** rules = malloc(sizeof(int*)*100);
  for (int i=0; i<100; i++) {
    rules[i] = malloc(sizeof(int)*(nb_rules+1));
    rules[i][0] = 0;
  }
  for (int i=0; i<nb_rules; i++) {
    char* str = str_file[i];
    char** spl = split(str, '|', NULL);
    int* vals = map_tab(spl, 2, sizeof(char*), atoi_void, sizeof(int));
    rules[vals[0]][0]++;
    rules[vals[0]][rules[vals[0]][0]] = vals[1];
    free(spl[0]);
    free(spl[1]);
    free(spl);
    free(vals);
  }
  int part1 = 0;
  int part2 = 0;
  for (int i=nb_rules; i<input_size; i++) {
    int nb_int = 0;
    char** spl = split(str_file[i], ',', &nb_int);
    int* vals = map_tab(spl, nb_int, sizeof(char*), atoi_void, sizeof(int));
    if (valide(rules, vals, nb_int)) {
      part1 += vals[(nb_int-1)/2];
    } else {
      corrige(rules, vals, nb_int);
      part2 += vals[(nb_int-1)/2];
    }
    free(vals);
    for (int j=0; j<nb_int; j++) free(spl[j]);
    free(spl);
  }
  for(int i=0; i<100; i++) free(rules[i]);
  free(rules);
  for(int i=0; i<input_size; i++) free(str_file[i]);
  free(str_file);

  printf("Partie 1 : %d\n", part1);
  printf("Partie 2 : %d\n", part2);

  return 0;
}
