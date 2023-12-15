
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "pile_file.h"

struct elem {
    int val;
    char* str;
};
typedef struct elem elem;

void free_elem(void* a) {
    elem* el = (elem*) a;
    free(el->str);
    free(el);
}

void remove_val(file* box, char* str) {
    list* a = box->d;
    while (a != NULL && strcmp(((elem*)a->val)->str, str) != 0) {
        a = a->post;
    }
    if (a != NULL) {
        if (a->post == NULL && a->pre == NULL) {
            extract_file(box);
        } else {
            if (a->pre != NULL) a->pre->post = a->post;
            else box->d = a->post;
            if (a->post != NULL) a->post->pre = a->pre;
            else box->f = a->pre;
        }
        free_elem(a->val);
        free(a);
    }
}

void change_or_add(file* box, char* str, int val) {
    list* a = box->d;
    while (a != NULL && strcmp(((elem*)a->val)->str, str) != 0) {
        a = a->post;
    }
    if (a != NULL) {
        ((elem*) a->val)->val = val;
    } else {
        elem* el = (elem*) malloc(sizeof(elem));
        el->str = malloc(sizeof(char)*(strlen(str)+1));
        strcpy(el->str, str);
        el->val = val;
        add_file(box, el);
    }
}

int main() {
    FILE* input = fopen("input.txt", "r");
    char c;

    int res = 0;
    int hash = 0;
    while (fscanf(input, "%c", &c) != EOF) {
        if (c == ',') {
            res += hash;
            hash = 0;
        } else {
            hash += c;
            hash *= 17;
            hash = hash%256;
        }
    }
    res += hash;
    hash = 0;
    printf("partie 1 : %d\n", res);

    fclose(input);
    input = fopen("input.txt", "r");

    file** boxs = malloc(sizeof(file*)*256);
    elem* el = (elem*) malloc(sizeof(elem));
    el->str = malloc(sizeof(char)*2);
    el->str[0] = '1';
    el->str[1] = 0;
    el->val = 0;
    for (int i=0; i<256; i++) {
        boxs[i] = init_file(el);
    }

    hash = 0;
    char* str = malloc(sizeof(char)*100);
    int str_length = 0;
    while (fscanf(input, "%c", &c) != EOF) {
        if (c == ',') {
            hash = 0;
            str[0] = 0;
            str_length = 0;
        } else if (c == '=') {
            str[str_length] = 0;
            char val_c;
            fscanf(input, "%c", &val_c);
            change_or_add(boxs[hash], str, val_c-'0');
        } else if (c == '-') {
            str[str_length] = 0;
            //printf("%s\n", str);
            remove_val(boxs[hash], str);
        } else {
            hash += c;
            hash *= 17;
            hash = hash%256;
            str[str_length] = c;
            str_length++;
        }
    }

    free(str);

    for (int i=0; i<256; i++) {
        extract_file(boxs[i]);
    }

    free_elem(el);

    long res_part2 = 0;

    for (int i=0; i<256; i++) {
        list* a = boxs[i]->f;
        int j = 1;
        while (a != NULL) {
            elem* el = (elem*) a->val;
            int add = (i+1)*j*el->val;
            //printf("%s : %d\n", el->str, add);
            res_part2 += add;
            j++;
            a = a->pre;
        }
        free_file(boxs[i], free_elem);
    }

    printf("partie 2 : %ld\n", res_part2);


    return 0;
}