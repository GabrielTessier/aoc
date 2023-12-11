
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct mallion {
    int i;
    struct mallion* next;
};
typedef struct mallion mallion;

struct list {
    mallion* d;
};
typedef struct list list;

void free_mail(mallion* a) {
    if (a != NULL) {
        free_mail(a->next);
        free(a);
    }
}

void free_list(list* l) {
    free_mail(l->d);
    free(l);
}

int predicte(list* pf) {
    mallion* a = pf->d;
    mallion* b = pf->d->next;
    list* n = malloc(sizeof(list));
    n->d = NULL;
    mallion* last = NULL;
    bool end = true;
    while (b != NULL) {
        if (last == NULL) {
            n->d = malloc(sizeof(mallion));
            n->d->i = b->i - a->i;
            n->d->next = NULL;
            last = n->d;
        } else {
            last->next = malloc(sizeof(mallion));
            last->next->i = b->i - a->i;
            last->next->next = NULL;
            last = last->next;
        }
        if (last->i != 0) end = false;
        a = a->next;
        b = b->next;
    }
    printf("int : ");
    last = n->d;
    while (last != NULL) {
        printf("%d, ", last->i);
        last = last->next;
    }
    printf("\n");
    if (end) {
        printf(" res : %d\n", pf->d->i);
        int r = pf->d->i;
        free_list(n);
        return r;
    } else {
        int next = predicte(n);
        printf(" res : %d-%d\n", pf->d->i, next);
        int r = pf->d->i;
        free_list(n);
        return r - next;
    }
}

int main() {
    FILE* input = fopen("input.txt", "r");
    char* str = (char*) malloc(sizeof(char)*1000);

    long res = 0;
    
    while (fscanf(input, "%[^\n]\n", str) != EOF) {
        list* pf = malloc(sizeof(list));
        pf->d = NULL;
        mallion* last = NULL;
        char* line = str;
        int nb = 0;
        bool neg = false;
        while (line[0] != 0) {
            if (line[0] == ' ') {
                if (neg) nb *= -1;
                if (last == NULL) {
                    pf->d = malloc(sizeof(mallion));
                    pf->d->i = nb;
                    pf->d->next = NULL;
                    last = pf->d;
                } else {
                    last->next = malloc(sizeof(mallion));
                    last->next->i = nb;
                    last->next->next = NULL;
                    last = last->next;
                }
                nb = 0;
                neg = false;
            } else {
                if (line[0] == '-') neg = true;
                else {
                    nb *= 10;
                    nb += line[0]-'0';
                }
            }
            line++;
        }
        if (neg) nb *= -1;
        last->next = malloc(sizeof(mallion));
        last->next->i = nb;
        last->next->next = NULL;
        printf("nex : ");
        last = pf->d;
        while (last != NULL) {
            printf("%d, ", last->i);
            last = last->next;
        }
        printf("\n");
        int a = predicte(pf);
        res += a;
        free_list(pf);
    }
    free(str);
    fclose(input);

    printf("res : %ld\n", res);
}