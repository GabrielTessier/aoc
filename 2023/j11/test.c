
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "pile_file.h"

struct coord {
    long x;
    long y;
    long rx;
    long ry;
};
typedef struct coord coord;

bool contient_x(file* p, long x) {
    list* a = p->d;
    while (a != NULL) {
        coord* c = (coord*) a->val;
        if (c->x == x) return true;
        a = a->post;
    }
    return false;
}
bool contient_y(file* p, long y) {
    list* a = p->d;
    while (a != NULL) {
        coord* c = (coord*) a->val;
        if (c->y == y) return true;
        a = a->post;
    }
    return false;
}

long labs(long a) {
    if (a>0) return a;
    else return -a;
}

int main() {
    FILE* input = fopen("input.txt", "r");
    char* str = (char*) malloc(sizeof(char)*1000);
    
    file* point = init_file(NULL);

    long size = 0;

    long y = 0;
    while (fscanf(input, "%[^\n]\n", str) != EOF) {
        long x = 0;
        while (str[x] != 0) {
            if (str[x] == '#') {
                coord* c = (coord*) malloc(sizeof(coord));
                c->x = x;
                c->y = y;
                c->rx = x;
                c->ry = y;
                add_file(point, c);
            }
            x++;
        }
        size = x;
        y++;
    }
    extract_file(point);

    printf("size : %ld\n", size);

    for (long i=0; i<size; i++) {
        if (contient_x(point, i) == false) {
            list* a = point->d;
            while (a != NULL) {
                coord* c = (coord*) a->val;
                if (c->x > i) c->rx = c->rx+1000000-1; // 2-1 pour la partie 1
                a = a->post;
            }
        }
        if (contient_y(point, i) == false) {
            list* a = point->d;
            while (a != NULL) {
                coord* c = (coord*) a->val;
                if (c->y > i) c->ry = c->ry+1000000-1; // 2-1 pour la partie 1
                a = a->post;
            }
        }
    }

    long sum = 0;

    list* a = point->d;
    while (a != NULL) {
        printf("x : %ld, y : %ld\n", ((coord*) a->val)->rx, ((coord*) a->val)->ry);
        list* b = a->post;
        while (b != NULL) {
            sum += labs(((coord*) a->val)->rx - ((coord*) b->val)->rx) + labs(((coord*) a->val)->ry - ((coord*) b->val)->ry);
            b = b->post;
        }
        a = a->post;
    }

    printf("partie 2 : %ld\n", sum);

    free_file(point, free);
    free(str);
    fclose(input);
}