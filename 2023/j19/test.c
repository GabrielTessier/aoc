
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "pile_file.h"

enum op {G,L};

struct link {
    char val;
    enum op op;
    int c;
    char* next;
};
typedef struct link link;

struct rule {
    char* name;
    link** ls;
    int ls_size;
    char* d;
};
typedef struct rule rule;

struct dict {
    int (*hash)(char*, int);
    file** tab;
};
typedef struct dict dict;

int hash(char* str, int size) {
    int v = 0;
    for (int i=0; i<size; i++) {
        //printf("key : %s, i : %d\n", str, i);
        v += str[i];
        v = v%100;
    }
    return v;
}

void add_dict(dict* d, char* key, rule* r) {
    add_file(d->tab[(d->hash)(key, (int)strlen(key))], (void*) r);
}

rule* get_dict(dict* d, char* key) {
    int i = (d->hash)(key, (int) strlen(key));
    list* a = d->tab[i]->d;
    while (a != NULL) {
        if (strcmp(((rule*)a->val)->name, key) == 0) {
            return (rule*) a->val;
        }
        a = a->post;
    }
    return NULL;
}

int get_val(char c, int x, int m, int a, int s) {
    if (c=='x') return x;
    else if (c=='m') return m;
    else if (c=='a') return a;
    else if (c=='s') return s;
    else return 0;
}

void free_rule(void* v) {
    rule* r = (rule*) v;
    free(r->d);
    for (int i=0; i<r->ls_size; i++) {
        free(r->ls[i]->next);
        free(r->ls[i]);
    }
    free(r->ls);
    free(r->name);
    free(r);
}

void part1(char* filename) {
    FILE* input = fopen(filename, "r");
    char* str = (char*) malloc(sizeof(char)*100);

    int nbrules = 0;
    int nbset = 0;
    char c;
    int nbr = 0;
    int p = 0;
    while (fscanf(input, "%c", &c) != EOF) {
        if (c == '\n') {
            nbr++;
            if (p==0) nbrules ++;
            else nbset ++;
        } else nbr = 0;
        if (nbr == 2) p = 1;
    }
    nbrules--;
    fclose(input);
    input = fopen(filename, "r");

    dict* rule_dict = (dict*) malloc(sizeof(dict));
    rule_dict->hash = hash;
    rule_dict->tab = (file**) malloc(sizeof(file*)*100);
    for (int i=0; i<100; i++) rule_dict->tab[i] = init_file(NULL);

    printf("nbrule : %d\n", nbrules);

    for (int i=0; i<nbrules; i++) {
        fscanf(input, "%[^\n]\n", str);
        // tnx{a>2206:R,s>1050:R,a>2105:R,R}
        int name_size = 0;
        while (str[name_size] != '{') name_size++;
        char* name = (char*) malloc(sizeof(char)*(name_size+1));
        for (int i=0; i<name_size; i++) name[i] = str[i];
        name[name_size] = 0;

        int nbl = 0;
        int i = name_size+1;
        while (str[i] != '}') {
            if (str[i] == ',') nbl++;
            i++;
        }
        link** ls = (link**) malloc(sizeof(link*)*nbl);
        i = name_size;
        for (int l=0; l<nbl; l++) {
            ls[l] = (link*) malloc(sizeof(link));
            i++;
            char val = str[i];
            enum op op = (str[i+1]=='<')?L:G;
            i += 2;
            int c = 0;
            while (str[i] != ':') {
                //printf("i:%d, str : %s\n", i, str);
                c *= 10;
                c += str[i]-'0';
                i++;
            }
            i++;
            int next_size = 0;
            while (str[i+next_size] != ',') next_size++;
            char* next = (char*) malloc(sizeof(char)*(next_size+1));
            for (int j=0; j<next_size; j++) {
                next[j] = str[i+j];
            }
            next[next_size] = 0;
            i += next_size;
            ls[l]->c = c;
            ls[l]->op = op;
            ls[l]->val = val;
            ls[l]->next = next;
        }
        rule* r = (rule*) malloc(sizeof(rule));
        r->ls = ls;
        r->name = name;
        r->ls_size = nbl;
        int d_size = 0;
        i++;
        while (str[i+d_size] != '}') d_size++;
        char* d = (char*) malloc(sizeof(char)*(d_size+1));
        for (int j=0; j<d_size; j++) {
            d[j] = str[i+j];
        }
        d[d_size] = 0;
        r->d = d;
        add_dict(rule_dict, r->name, r);
    }

    for (int i=0; i<100; i++) extract_file(rule_dict->tab[i]);

    int res = 0;

    while (fscanf(input, "%[^\n]\n", str) != EOF) {
        // {x=787,m=2655,a=1222,s=2876}
        int x=0;
        int m=0;
        int a=0;
        int s=0;

        int i = 3;
        while (str[i] != ',') {
            x *= 10;
            x += str[i] - '0';
            i++;
        } i+=3;
        while (str[i] != ',') {
            m *= 10;
            m += str[i] - '0';
            i++;
        } i+=3;
        while (str[i] != ',') {
            a *= 10;
            a += str[i] - '0';
            i++;
        } i+=3;
        while (str[i] != '}') {
            s *= 10;
            s += str[i] - '0';
            i++;
        }

        int ix=x;
        int im=m;
        int ia=a;
        int is=s;

        //printf("str : %s, K : {x=%d,m=%d,a=%d,s=%d}\n", str, x, m, a, s);

        rule* r = get_dict(rule_dict, "in");
        bool acc = false;
        bool rej = false;
        while (acc == false && rej == false) {
            bool find_next = false;
            int i = 0;
            while (find_next == false && i<r->ls_size) {
                link* l = r->ls[i];
                if ((l->op == L && get_val(l->val,x,m,a,s)<l->c) || (l->op == G && get_val(l->val,x,m,a,s)>l->c)) {
                    find_next = true;
                    i--;
                }
                i++;
            }
            if (find_next) {
                if (r->ls[i]->next[0] == 'A') acc = true;
                if (r->ls[i]->next[0] == 'R') rej = true;
                r = get_dict(rule_dict, r->ls[i]->next);
            } else {
                if (r->d[0] == 'A') acc = true;
                if (r->d[0] == 'R') rej = true;
                r = get_dict(rule_dict, r->d);
            }
        }
        if (acc) {
            res += ix+im+ia+is;
        }
    }
    printf("partie 1 : %d\n", res);

    fclose(input);
    free(str);
    for (int i=0; i<100; i++) free_file(rule_dict->tab[i], free_rule);
    free(rule_dict->tab);
    free(rule_dict);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("%s <input filename>\n", argv[0]);
        return 0;
    }

    part1(argv[1]);
    printf("partie 2 pas fait.\n");
    //part2(argv[1]);

    return 0;
}