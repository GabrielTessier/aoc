
#include <stdlib.h>
#include <stdbool.h>

#include "list.h"

maillon_t* init_maillon(void* val) {
    maillon_t* l = (maillon_t*) malloc(sizeof(maillon_t));
    l->post = NULL;
    l->pre = NULL;
    l->val = val;
    return l;
}

list_t* init_list() {
    list_t* p = (list_t*) malloc(sizeof(list_t));
    p->d = NULL;
    p->f = NULL;
    p->size = 0;
    return p;
}

void iter_list(list_t* p, void (*fnc)(void*), enum position start) {
    maillon_t* il = NULL;
    if (start == DEBUT) il = p->d;
    else if (start == FIN) il = p->f;
    while (il != NULL) {
        fnc(il->val);
        if (start == DEBUT) il = il->post;
        else if (start == FIN) il = il->pre;
    }
}

void iteri_list(list_t* p, void (*fnc)(int, void*), enum position start) {
    maillon_t* il = NULL;
    if (start == DEBUT) il = p->d;
    else if (start == FIN) il = p->f;
    int i = 0;
    while (il != NULL) {
        fnc(i, il->val);
        i++;
        if (start == DEBUT) il = il->post;
        else if (start == FIN) il = il->pre;
    }
}

void* map_list(list_t* p, void* (*fnc)(void*), enum position start) {
    list_t* res = init_list();
    maillon_t* il = NULL;
    enum position nstart;
    if (start == DEBUT) {
        nstart = FIN;
        il = p->d;
    } else if (start == FIN) {
        nstart = DEBUT;
        il = p->f;
    }
    while (il != NULL) {
        add_list(res, fnc(il->val), nstart);
        if (start == DEBUT) il = il->post;
        else if (start == FIN) il = il->pre;
    }
    return res;
}

void* mapi_list(list_t* p, void* (*fnc)(int, void*), enum position start) {
    list_t* res = init_list();
    maillon_t* il = NULL;
    enum position nstart;
    if (start == DEBUT) {
        nstart = FIN;
        il = p->d;
    } else if (start == FIN) {
        nstart = DEBUT;
        il = p->f;
    }
    int i = 0;
    while (il != NULL) {
        add_list(res, fnc(i, il->val), nstart);
        i++;
        if (start == DEBUT) il = il->post;
        else if (start == FIN) il = il->pre;
    }
    return res;
}

void add_list(list_t* p, void* val, enum position pos) {
    if (p->d == NULL) {
        maillon_t* il = init_maillon(val);
        p->d = il;
        p->f = il;
    } else {
        if (pos == DEBUT) {
            maillon_t* il = init_maillon(val);
            il->post = p->d;
            p->d->pre = il;
            p->d = il;
        } else if (pos == FIN) {
            maillon_t* il = init_maillon(val);
            il->pre = p->f;
            p->f->post = il;
            p->f = il;
        }
    }
    p->size++;
}

void* extract_list(list_t* p, enum position pos) {
    if (pos == DEBUT && p->d != NULL) {
        void* val = p->d->val;
        maillon_t* d = p->d;
        if (p->d->post != NULL) {
            p->d = p->d->post;
            p->d->pre = NULL;
            if (p->d == p->f) p->f->pre = NULL;
        } else {
            p->d = NULL;
            p->f = NULL;
        }
        p->size--;
        free(d);
        return val;
    } else if (pos == FIN && p->f != NULL) {
        void* val = p->f->val;
        maillon_t* f = p->f;
        if (p->f->pre != NULL) {
            p->f = p->f->pre;
            p->f->post = NULL;
            if (p->d == p->f) p->d->post = NULL;
        } else {
            p->d = NULL;
            p->f = NULL;
        }
        free(f);
        p->size--;
        return val;
    }
    return NULL;
}

void filter_list(list_t* p, bool (*fnc)(void*), void (*free_data)(void*)) {
  maillon_t* m = p->d;
  while (m != NULL) {
    maillon_t* tmp = m->post;
    bool a_free = fnc(m->val) == false;
    if (a_free) {
      if (m->pre != NULL) m->pre->post = m->post;
      else p->d = m->post;
      if (m->post != NULL) m->post->pre = m->pre;
      else p->f = m->pre;
      if (free_data != NULL) free_data(m->val);
      free(m);
      p->size--;
    }
    m = tmp;
  }
}

bool in_list(list_t* p, void* val) {
  maillon_t* m = p->d;
  while (m != NULL) {
    if (m->val == val) return true;
    m = m->post;
  }
  return false;
}

void free_list(list_t* p, void (*free_data)(void*)) {
    while (p->d != NULL) {
        void* data = extract_list(p, FIN);
        if (free_data != NULL) free_data(data);
    }
    free(p);
}

bool list_vide(list_t* p) {
    return p->d == NULL;
}


file* init_file() {
    file* f = (file*) malloc(sizeof(file));
    f->d = NULL;
    f->f = NULL;
    return f;
}

void add_file(file* f, void* val) {
    add_list(f, val, DEBUT);
}

void* extract_file(file* f) {
    return extract_list(f, FIN);
}

void free_file(file* f, void (*free_data)(void*)) {
    free_list(f, (*free_data));
}

bool file_vide(file* f) {
    return list_vide(f);
}

void iter_file(file* f, void (*fnc)(void*)) {
    iter_list(f, (*fnc), DEBUT);
}
void iteri_file(file* f, void (*fnc)(int, void*)) {
    iteri_list(f, (*fnc), DEBUT);
}
void* map_file(file* f, void* (*fnc)(void*)) {
    return map_list(f, (*fnc), DEBUT);
}
void* mapi_file(file* f, void* (*fnc)(int, void*)) {
    return mapi_list(f, (*fnc), DEBUT);
}

pile* init_pile() {
    pile* p = (pile*) malloc(sizeof(pile));
    p->d = NULL;
    p->f = NULL;
    return p;
}

void add_pile(pile* p, void* val) {
    add_list(p, val, DEBUT);
}

void* extract_pile(pile* p) {
    return extract_list(p, DEBUT);
}

void free_pile(pile* p, void (*free_data)(void*)) {
    free_list(p, (*free_data));
}

bool pile_vide(pile* p) {
    return list_vide(p);
}

void iter_pile(pile* p, void (*fnc)(void*)) {
    iter_list(p, (*fnc), FIN);
}
void iteri_pile(pile* p, void (*fnc)(int, void*)) {
    iteri_list(p, (*fnc), FIN);
}
void* map_pile(pile* p, void* (*fnc)(void*)) {
    return map_list(p, (*fnc), FIN);
}
void* mapi_pile(pile* p, void* (*fnc)(int, void*)) {
    return mapi_list(p, (*fnc), FIN);
}
