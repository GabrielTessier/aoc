
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "tools.h"

int int_comparator(const void* first, const void* second) {
    int firstInt = * (const int *) first;
    int secondInt = * (const int *) second;
    return firstInt - secondInt;
}

int abs(const int a) {
  if (a>0) return a;
  return -a;
}

void* map_tab(void* tab, size_t nb_elem, size_t size_old, void* (*map_fnc)(void*), size_t size_new) {
  void* n_tab = malloc(nb_elem*size_new);
  for (size_t b=0; b<nb_elem*size_new; b++) {
    ((unsigned char *)(n_tab))[b] = 0;
  }
  for (size_t i=0; i<nb_elem; i++) {
    void* val = map_fnc(*((void**)(tab+i*size_old)));
    for (size_t b=0; b<size_new*8; b++) {
      unsigned char bit = (((unsigned long int)val)&(((unsigned long)1)<<b))>>(b-b%8);
      ((unsigned char *)(n_tab+i*size_new))[b/CHAR_BIT] |= bit;
    }
  }
  return n_tab;
}

void* atoi_void(void* str) {
  return (void*) (intptr_t) atoi((char*) str);
}

char** split(char* str, char del, int* nb_sub_ret) {
  size_t size = strlen(str);
  int nb_sub = 0;
  for (size_t i=0; i<size-1; i++) {
    if (str[i] != del && str[i+1] == del) nb_sub++;
  }
  if (str[size-1] != del) nb_sub++;
  char** sub_str = malloc(sizeof(char**)*nb_sub);
  int pos = 0;
  for (int i=0; i<nb_sub; i++) {
    while (str[pos] == del) pos++;
    int s = pos;
    while ((size_t)s < size && str[s] != del) s++;
    s -= pos;
    s++;
    sub_str[i] = malloc(sizeof(char*)*s);
    for (int j=0; j<s; j++) {
      sub_str[i][j] = str[pos+j];
    }
    sub_str[i][s-1] = 0;
    pos += s;
  }
  if (nb_sub_ret != NULL) *nb_sub_ret = nb_sub;
  return sub_str;
}

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
