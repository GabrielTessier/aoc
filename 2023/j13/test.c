
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
IL FAUT AJOUTER DES 'a' SUR CHAQUE LIGNE VIDE DANS L'INPUT
EXEMPLE :

#.##..##.
..#.##.#.
##......#
##......#
..#.##.#.
..##..##.
#.#.##.#.

#...##..#
#....#..#
..##..###
#####.##.
#####.##.
..##..###
#....#..#

DEVIENT :

#.##..##.
..#.##.#.
##......#
##......#
..#.##.#.
..##..##.
#.#.##.#.
a
#...##..#
#....#..#
..##..###
#####.##.
#####.##.
..##..###
#....#..#
a

*/

int** add_line(int** tab, int tab_size) {
    if (tab_size == 0) {
        return (int**) malloc(sizeof(int*));
    } else {
        int** tab2 = (int**) malloc(sizeof(int*)*(tab_size+1));
        for (int i=0; i<tab_size; i++) {
            tab2[i] = tab[i];
        }
        return tab2;
    }
}

bool is_vert_sym(int** tab, int size, int tab_size, int i, bool partie1) {
    int d = 0;
    bool find_diff = partie1;
    while (i-d>=0 && i+d+1<size) {
        for (int j=0; j<tab_size; j++) {
            if (tab[j][i-d] != tab[j][i+d+1]) {
                if (find_diff == false) find_diff = true;
                else return false;
            }
        }
        d++;
    }
    return find_diff;
}

bool is_hor_sym(int** tab, int size, int tab_size, int i, bool partie1) {
    int d = 0;
    bool find_diff = partie1;
    while (i-d>=0 && i+d+1<tab_size) {
        for (int j=0; j<size; j++) {
            if (tab[i-d][j] != tab[i+d+1][j]) {
                if (find_diff == false) find_diff = true;
                else return false;
            }
        }
        d++;
    }
    return find_diff;
}

int main() {
    FILE* input = fopen("input.txt", "r");
    char* str = (char*) malloc(sizeof(char)*1000);

    bool partie1 = false;

    int size = 0;
    int tab_size = 0;
    int** tab = NULL;

    int res = 0;

    while (fscanf(input, "%[^\n]\n", str) != EOF) {
        //printf("strlen : %ld, str : %s\n", strlen(str), str);
        if (strlen(str) == 1) {
            int find = false;
            int i=0;
            while (find == false && i<size-1) {
                if (is_vert_sym(tab, size, tab_size, i, partie1)) {
                    find = true;
                    res += i+1;
                }
                i++;
            }
            i=0;
            while (find == false && i<tab_size-1) {
                if (is_hor_sym(tab, size, tab_size, i, partie1)) {
                    find = true;
                    res += (i+1)*100;
                }
                i++;
            }
            for (int j=0; j<tab_size; j++) free(tab[j]);
            free(tab);
            tab = NULL;
            tab_size = 0;
            size = 0;
        } else {
            size = strlen(str);
            int** tab2 = add_line(tab, tab_size);
            free(tab);
            tab = tab2;
            tab_size++;
            tab[tab_size-1] = (int*) malloc(sizeof(int)*size);
            for (int i=0; i<size; i++) {
                tab[tab_size-1][i] = (str[i]=='#')?1:0; 
            }
        }
    }

    printf("partie %s : %d\n", (partie1)?"1":"2", res);

    fclose(input);
    free(str);

    return 0;
}