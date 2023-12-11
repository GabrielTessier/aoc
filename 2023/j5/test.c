
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "pile_file.h"

struct elem_s {
    long unsigned int src;
    long unsigned int dest;
    long unsigned int range;
    bool vu;
};
typedef struct elem_s elem_t;

elem_t* find_min_dest(list* list) {
    elem_t* min = NULL;
    while (list != NULL) {
        if (min == NULL && ((elem_t*) list->val)->vu == false) {
            min = list->val;
        } else if (min != NULL && ((elem_t*) list->val)->vu == false && ((elem_t*) list->val)->dest < min->dest) {
            min = list->val;
        }
        list = list->post;
    }
    return min;
}

elem_t* find_min_src(list* list) {
    elem_t* min = NULL;
    while (list != NULL) {
        if (min == NULL && ((elem_t*) list->val)->vu == false) {
            min = list->val;
        } else if (min != NULL && ((elem_t*) list->val)->vu == false && ((elem_t*) list->val)->src < min->src) {
            min = list->val;
        }
        list = list->post;
    }
    return min;
}

long unsigned int max_lui(long unsigned int a, long unsigned int b) {
    if (a<b) return b;
    else return a;
}

long unsigned int min_lui(long unsigned int a, long unsigned int b) {
    if (a<b) return a;
    else return b;
}

void set_vu_false(list* b) {
    while (b != NULL) {
        ((elem_t*) b->val)->vu = false;
        b = b->post;
    }
}

bool inter(elem_t* a, elem_t* b) {
    if (b->src >= a->dest && b->src < a->dest+a->range) return true;
    if (b->src + b->range >= a->dest && b->src + b->range < a->dest+a->range) return true;
    if (b->src < a->dest && b->src + b->range >= a->dest+a->range) return true;
    return false;
}

file* intersect(list* a, list* b) {
    file* res = init_file(NULL);
    list* l = a;
    while (l != NULL) {
        set_vu_false(b);
        elem_t* ela = l->val;
        elem_t* elb = find_min_src(b);
        long unsigned int max_min = ela->dest;
        printf("maxmin : %ld\n", max_min);
        while (elb != NULL) {
            if (inter(ela, elb)) {
                elem_t* add = malloc(sizeof(elem_t));
                add->src = max_lui(max_min, elb->src);
                add->range = min_lui(ela->dest+ela->range - add->src, elb->src+elb->range - add->src);
                add->dest = elb->dest + add->src - elb->src;
                add->vu = false;
                //if (add->range > 0) {
                    add_file(res, add);
                    //if (add->src != elb->src) {
                        elem_t* add2 = malloc(sizeof(elem_t));
                        add2->src = max_min;
                        add2->range = add->src - max_min;
                        printf("max_min : %ld, add->src : %ld, range : %ld \n", max_min, add->src, add2->range);
                        add2->dest = add2->src;
                        add2->vu = false;
                        if (add2->range > 0) add_file(res, add2);
                        else free(add2);
                    //}
                    max_min = add->src + add->range;
                //}
            }
            elb->vu = true;
            if (elb->src >= ela->dest+ela->range) {
                elb = NULL;
            } else {
                elb = find_min_src(b);
            }
        }
        elem_t* add2 = malloc(sizeof(elem_t));
        add2->src = max_min;
        add2->range = ela->dest + ela->range - max_min;
        add2->dest = max_min;
        add2->vu = false;
        if (add2->range > 0) add_file(res, add2);
        else free(add2);
        l = l->pre;
    }
    extract_file(res);
    return res;
}

file* sort_dest(list* a) {
    file* res = init_file(NULL);
    set_vu_false(a);
    elem_t* m = find_min_dest(a);
    while (m != NULL) {
        add_file(res, m);
        m->vu = true;
        m = find_min_dest(a);
    }
    set_vu_false(a);
    extract_file(res);
    return res;
}

void print_elem(void* aa) {
    elem_t* a = aa;
    if (a != NULL) printf("[src : %ld, range : %ld, dest : %ld, vu : %s]", a->src, a->range, a->dest, (a->vu)?"oui":"non");
    else printf("[NULL]\n");
}

int main() {
    FILE* input = fopen("input.txt", "r");
    char* str = (char*) malloc(sizeof(char)*1000);
    int nb_seed = 20;
    fscanf(input, "%[^\n]\n", str);
    char* line = str;
    while (line[0] != ':') line++;
    line+=2;
    long unsigned int* seeds = (long unsigned int*) malloc(sizeof(long unsigned int)*nb_seed);
    for (int i=0; i<nb_seed; i++) seeds[i] = 0;
    int i = 0;
    while (line[0] != 0) {
        if (line[0] != ' ') {
            seeds[i] *= 10;
            seeds[i] += line[0]-'0';
        }
        if (line[0] == ' ' && line[1] != ' ') {
            i++;
        }
        line++;
    }
    for (int i=0; i<nb_seed; i++) printf("%ld, ", seeds[i]);
    printf("\n");

    fscanf(input, "%[^\n]\n", str);
    //fscanf(input, "%[^\n]\n", str);
    file* dict[] = {init_file(NULL), init_file(NULL), init_file(NULL), init_file(NULL), init_file(NULL), init_file(NULL), init_file(NULL)};
    int state = 0;
    while (fscanf(input, "%[^\n]\n", str) != EOF) {
        printf("start : %d\n", state);
        printf("str : %s\n", str);
        if (str[0] < '0' || str[0] > '9') {
            printf("state++\n");
            if (str[0] != 0) state++;
        } else {
            printf("new dat\n");
            char* line = str;
            long unsigned int vals[] = {0,0,0}; // dest, src, range
            i = 0;
            bool q = false;
            while (q == false) {
                if (line[0] != ' ') {
                    vals[i] *= 10;
                    vals[i] += line[0]-'0';
                    printf("val[%d] = %ld\n", i, vals[i]);
                }
                if (line[0] == ' ') {
                    i++;
                    printf("i++\n");
                }
                printf("line[0] = %d\n", line[0]);
                line++;
                printf("line++\n");
                printf("line[0] = %d\n", line[0]);
                q = (line[0] == 0);
            }
            printf("range : %ld\n", vals[2]);
            elem_t* e = malloc(sizeof(elem_t));
            e->src = vals[1];
            e->dest = vals[0];
            e->range = vals[2];
            e->vu = false;
            add_file(dict[state], e);
        }
    }
    for (int i=0; i<7; i++) {
        extract_file(dict[i]);
    }

    file* next = init_file(NULL);
    for (int i=0; i<nb_seed; i+=2) {
        elem_t* val = malloc(sizeof(elem_t));
        val->dest = seeds[i];
        val->range = seeds[i+1];
        val->src = 0;
        val->vu = false;
        add_file(next, val);
    }
    extract_file(next);

    printf("next : ");
    print_file(next, print_elem);
    printf("dict[0] : ");
    print_file(dict[0], print_elem);

    file* in1 = intersect(next->f, dict[0]->d);
    file* in11 = sort_dest(in1->d);
    printf("in0 : ");
    print_file(in1, print_elem);
    printf("in00 : ");
    print_file(in11, print_elem);

    for (int i=1; i<7; i++) {
        file* in2 = intersect(in11->f, dict[i]->d);
        file* in22 = sort_dest(in2->d);
        free_file(in1, free);
        free(in11);
        in1 = in2;
        in11 = in22;
        printf("dict[%d] : ", i);
        print_file(dict[i], print_elem);
        printf("in%d : ", i);
        print_file(in1, print_elem);
        printf("in%d%d : ", i, i);
        print_file(in11, print_elem);
    }

    elem_t* m = extract_file(in11);
    while (m == NULL) {
        m = extract_file(in11);
    }
    printf("min : %ld\n", m->dest);

        free_file(in1, free);
        free(in11);
        free_file(next, free);


    free(seeds);
    free(str);
    for (int j=0; j<7; j++) {
        free_file(dict[j], free);
    }

    return 0;
}