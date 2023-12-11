
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "pile_file.h"

struct node_s {
    int ind;
    int left;
    int right;
};
typedef struct node_s node_t;

bool end(file* ens) {
    bool valide = true;
    list* a = ens->d;
    while (a != NULL && valide) {
        if (((node_t*)a->val)->ind % 26 != 25) valide = false;
        a = a->post;
    }
    return valide;
}

int main() {
    FILE* input = fopen("input.txt", "r");
    char* str = (char*) malloc(sizeof(char)*1000);

    int* path = (int*) malloc(sizeof(int)*1000);
    int path_length = 0;
    fscanf(input, "%[^\n]\n", str);
    char* line = str;
    while (line[0] != 0) {
        path[path_length] = (line[0]=='L')?-1:1;
        line++;
        path_length++;
    }
    //fscanf(input, "%[^\n]\n", str);

    node_t** graph = malloc(sizeof(node_t*)*26*26*26);
    for (int i=0; i<26*26*26; i++) {
        graph[i] = NULL;
    }

    
    while (fscanf(input, "%[^\n]\n", str) != EOF) {
        line = str;
        int nb = (line[0]-'A')*26*26 + (line[1]-'A')*26 + (line[2]-'A');
        int left = (line[7]-'A')*26*26 + (line[8]-'A')*26 + (line[9]-'A');
        int right = (line[12]-'A')*26*26 + (line[13]-'A')*26 + (line[14]-'A');
        node_t* a = malloc(sizeof(node_t));
        a->ind = nb;
        a->left = left;
        a->right = right;
        printf("nb : %d, l : %d, r : %d\n", nb, left, right);
        graph[nb] = a;
    }

    file* ens = init_file(NULL);

    for (int i=0; i<26*26*26-1; i++) {
        if (i % 26 == 0 && graph[i] != NULL) {
            add_file(ens, graph[i]);
            printf("i : %d\n", i);
        }
    }
    extract_file(ens);

    int length = 0;
    int pathi = 0;
    while (end(ens) == false) {
        if (pathi == path_length) pathi = 0;
        list* a = ens->d;
        printf("len : %d\n", length);
        while (a != NULL) {
            if (path[pathi] == -1) {
                a->val = graph[((node_t*) a->val)->left];
            } else {
                a->val = graph[((node_t*) a->val)->right];
            }
            a = a->post;
        }
        pathi++;
        length++;
    }
    printf("length : %d\n", length);
}