
#include <stdio.h>
#include <stdlib.h>

#include "pile_file.h"


list* init_list(void* val) {
    list* il = (list*) malloc(sizeof(list));
    il->val = val;
    il->post = NULL;
    il->pre = NULL;
    return il;
}

void add_pf(pf* p, void* val, enum position pos) {
    if (p->d == NULL) {
        list* il = init_list(val);
        p->d = il;
        p->f = il;
    } else {
        if (pos == DEBUT) {
            list* il = init_list(val);
            il->post = p->d;
            p->d->pre = il;
            p->d = il;
        } else if (pos == FIN) {
            list* il = init_list(val);
            il->pre = p->f;
            p->f->post = il;
            p->f = il;
        }
    }
}

void* extract_pf(pf* p, enum position pos) {
    if (pos == DEBUT && p->d != NULL) {
        void* val = p->d->val;
        list* d = p->d;
        if (p->d->post != NULL) {
            p->d = p->d->post;
            p->d->pre = NULL;
            if (p->d == p->f) p->f->pre = NULL;
        } else {
            p->d = NULL;
            p->f = NULL;
        }
        free(d);
        return val;
    } else if (pos == FIN && p->f != NULL) {
        void* val = p->f->val;
        list* f = p->f;
        if (p->f->pre != NULL) {
            p->f = p->f->pre;
            p->f->post = NULL;
            if (p->d == p->f) p->d->post = NULL;
        } else {
            p->d = NULL;
            p->f = NULL;
        }
        free(f);
        return val;
    }
    return NULL;
}

void free_pf(pf* p, void (*free_data)(void*)) {
    while (p->d != NULL) {
        free_data(extract_pf(p, FIN));
    }
    free(p);
}

file* init_file(void* val) {
    file* f = (file*) malloc(sizeof(file));
    list* il = init_list(val);
    f->d = il;
    f->f = il;
    return f;
}

void add_file(file* f, void* val) {
    add_pf(f, val, DEBUT);
}

void* extract_file(file* f) {
    return extract_pf(f, FIN);
}

void print_file(file* f, void (*print_function)(void*)) {
    list* c = f->f;
    while (c != NULL) {
        (*print_function) (c->val);
        printf(", ");
        c = c->pre;
    }
    printf("\n");
}

void free_file(file* f, void (*free_data)(void*)) {
    free_pf(f, (*free_data));
}

pile* init_pile(void* val) {
    pile* p = (pile*) malloc(sizeof(pile));
    list* il = init_list(val);
    p->d = il;
    p->f = il;
    return p;
}

void add_pile(pile* p, void* val) {
    add_pf(p, val, DEBUT);
}
void* extract_pile(pile* p) {
    return extract_pf(p, DEBUT);
}
void free_pile(pile* p, void (*free_data)(void*)) {
    free_pf(p, (*free_data));
}
