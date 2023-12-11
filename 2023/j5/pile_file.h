
#ifndef PILE_FILE_H
#define PILE_FILE_H

struct list {
    void* val;
    struct list* pre;
    struct list* post;
};
typedef struct list list;

struct pf {
    list* d;
    list* f;
};
typedef struct pf pf;

typedef pf file;
typedef pf pile;

enum position {DEBUT, FIN};


list* init_list(void* val);

void add_pf(pf* p, void* val, enum position pos);
void* extract_pf(pf* p, enum position pos);
void free_pf(pf* p, void (*free_data)(void*));

file* init_file(void* val);
void add_file(file* f, void* val);
void* extract_file(file* f);
void print_file(file* f, void (*print_function)(void*));
void free_file(file* f, void (*free_data)(void*));

pile* init_pile(void* val);
void add_pile(pile* p, void* val);
void* extract_pile(pile* p);
void free_pile(pile* p, void (*free_data)(void*));



#endif // PILE_FILE_H