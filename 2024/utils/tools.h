
#ifndef TOOLS_H
#define TOOLS_H

int int_comparator(const void* first, const void* second);
int abs(const int a);
void* map_tab(void* tab, size_t nb_elem, size_t size_old, void* (*map_fnc)(void*), size_t size_new);
void* atoi_void(void* str);
char** split(char* str, char del, int* nb_sub);
char** read_file_to_array(char* filename, int* size_ret);

#endif
