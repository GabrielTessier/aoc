
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

int main() {
    FILE* input = fopen("input.txt", "r");
    char* str = (char*) malloc(sizeof(char)*1000);

    bool partie1 = false;

    int size = 0;
    int tab_size = 0;
    int** tab = NULL;

    int res = 0;

    while (fscanf(input, "%[^\n]\n", str) != EOF) {
        size = strlen(str);
        int** tab2 = add_line(tab, tab_size);
        free(tab);
        tab = tab2;
        tab_size++;
        tab[tab_size-1] = (int*) malloc(sizeof(int)*size);
        for (int i=0; i<size; i++) {
            if (str[i]=='O') tab[tab_size-1][i] = 1;
            else if (str[i]=='#') tab[tab_size-1][i] = 2;
            else if (str[i]=='.') tab[tab_size-1][i] = 3;
        }
    }

    if (partie1 == false) {
        for (int a=0; a<1000000000; a++) {
            printf("%d\n", a);
            for (int i=0; i<tab_size; i++) {
                for (int j=0; j<size; j++) {
                    if (tab[i][j] == 1) {
                        int k = i-1;
                        tab[i][j] = 3;
                        while (k>=0 && tab[k][j] == 3) k--;
                        k++;
                        tab[k][j] = 1;
                    }
                }
            }

            for (int i=0; i<size; i++) {
                for (int j=0; j<tab_size; j++) {
                    if (tab[j][i] == 1) {
                        int k = i-1;
                        tab[j][i] = 3;
                        while (k>=0 && tab[j][k] == 3) k--;
                        k++;
                        tab[j][k] = 1;
                    }
                }
            }

            for (int i=0; i<tab_size; i++) {
                for (int j=0; j<size; j++) {
                    if (tab[i][j] == 1) {
                        int k = i+1;
                        tab[i][j] = 3;
                        while (k<tab_size && tab[k][j] == 3) k++;
                        k--;
                        tab[k][j] = 1;
                    }
                }
            }

            for (int i=0; i<size; i++) {
                for (int j=0; j<tab_size; j++) {
                    if (tab[j][i] == 1) {
                        int k = i+1;
                        tab[j][i] = 3;
                        while (k<size && tab[j][k] == 3) k++;
                        k--;
                        tab[j][k] = 1;
                    }
                }
            }
        }
    }

    for (int i=0; i<tab_size; i++) {
        for (int j=0; j<size; j++) {
            if (tab[i][j] == 1) {
                int k = i-1;
                tab[i][j] = 3;
                while (k>=0 && tab[k][j] == 3) {
                    k--;
                }
                k++;
                tab[k][j] = 1;
                res += tab_size-k;
            }
        }
    }

    printf("partie %s : %d\n", (partie1)?"1":"2", res);

    for (int i=0; i<tab_size; i++) {
        free(tab[i]);
    }
    free(tab);

    fclose(input);
    free(str);

    return 0;
}