
#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

struct maillon_s {
    void* val;
    struct maillon_s* pre;
    struct maillon_s* post;
};
typedef struct maillon_s maillon_t;

struct list_s {
    int size;
    maillon_t* d;
    maillon_t* f;
};
typedef struct list_s list_t;

typedef list_t file;
typedef list_t pile;

enum position {DEBUT, FIN};

/* Initialise un maillon avec pour valeur `val` */
maillon_t* init_maillon(void* val);

/* Initialise une liste doublement chaînée */
list_t* init_list();

/* Applique la fonction `fnc` à chaque élément de la liste `p` en partant de la position `start` */
void iter_list(list_t* p, void (*fnc)(void*), enum position start);

/* Applique la fonction `fnc` à chaque élément de la liste `p` en partant de la position `start` */
void iteri_list(list_t* p, void (*fnc)(int, void*), enum position start);

/* Créer une nouvelle liste dont chaque élément est le résultat de `fnc` sur un élément de `p` */
void* map_list(list_t* p, void* (*fnc)(void*), enum position start);

/* Créer une nouvelle liste dont chaque élément est le résultat de `fnc` sur un élément de `p` */
void* mapi_list(list_t* p, void* (*fnc)(int, void*), enum position start);

/* Ajoute un maillon de valeur `val` en position `pos` à la liste `p` */
void add_list(list_t* p, void* val, enum position pos);

/* Extrait la valeur du maillon en position `pos` de la liste `p` */
void* extract_list(list_t* p, enum position pos);

/* Ne garde que les éléments validant la fonction `fnc` */
void filter_list(list_t* p, bool (*fnc)(void*), void (*free_data)(void*));

/* Vrai si `p` contient `val` */
bool in_list(list_t* p, void* val);

/* Libère la liste `p` et libère chaque maillon

   Pour libérer un maillon la fonction `free_data` est utilisée pour libérer la donner du maillon

   Si les données des maillons ne doit pas être libérer alors mettre `NULL` pour `free_data` */
void free_list(list_t* p, void (*free_data)(void*));

/* Renvoie si la liste est vide ou non */
bool list_vide(list_t* p);


/* Initialise une liste */
file* init_file();

/* Ajoute un maillon de valeur `val` à la liste `p` */
void add_file(file* f, void* val);

/* Extrait la valeur du premier maillon de la liste `p` */
void* extract_file(file* f);

/* Libère la liste `p` et libère chaque maillon

   Pour libérer un maillon la fonction `free_data` est utilisée pour libérer la donner du maillon

   Si les données des maillons ne doit pas être libérer alors mettre `NULL` pour `free_data` */
void free_file(file* f, void (*free_data)(void*));

/* Renvoie si la liste est vide ou non */
bool file_vide(file* f);

/* Applique la fonction `fnc` à chaque élément de la liste `p` */
void iter_file(file* f, void (*fnc)(void*));

/* Applique la fonction `fnc` à chaque élément de la liste `p` */
void iteri_file(file* f, void (*fnc)(int, void*));

/* Créer une nouvelle liste dont chaque élément est le résultat de `fnc` sur un élément de `p` */
void* map_file(file* f, void* (*fnc)(void*));

/* Créer une nouvelle liste dont chaque élément est le résultat de `fnc` sur un élément de `p` */
void* mapi_file(file* f, void* (*fnc)(int, void*));


/* Initialise une pile */
pile* init_pile();

/* Ajoute un maillon de valeur `val` à la pile `p` */
void add_pile(pile* p, void* val);

/* Extrait la valeur du premier maillon de la pile `p` */
void* extract_pile(pile* p);

/* Libère la pile `p` et libère chaque maillon

   Pour libérer un maillon la fonction `free_data` est utilisée pour libérer la donner du maillon

   Si les données des maillons ne doit pas être libérer alors mettre `NULL` pour `free_data` */
void free_pile(pile* p, void (*free_data)(void*));

/* Renvoie si la pile est vide ou non */
bool pile_vide(pile* p);

/* Applique la fonction `fnc` à chaque élément de la pile `p` */
void iter_pile(pile* p, void (*fnc)(void*));

/* Applique la fonction `fnc` à chaque élément de la pile `p` */
void iteri_pile(pile* p, void (*fnc)(int, void*));

/* Créer une nouvelle pile dont chaque élément est le résultat de `fnc` sur un élément de `p` */
void* map_pile(pile* p, void* (*fnc)(void*));

/* Créer une nouvelle pile dont chaque élément est le résultat de `fnc` sur un élément de `p` */
void* mapi_pile(pile* p, void* (*fnc)(int, void*));


#endif // LIST_H
