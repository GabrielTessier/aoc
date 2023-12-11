
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "pile_file.h"

void* itv(int i) { // int to void*
    int* a = (int*) malloc(sizeof(int));
    *a = i;
    return (void*) a;
}

int get_ind(int i, int j, int size) {
    if (i<0 || j<0 || i>=size || j>=size) return -1;
    else if (i*size+j >= 0 && i*size+j < size*size) return i*size+j;
    else return -1;
}

int abs(int a) {
    if (a>0) return a;
    else return -a;
}

int parcour_largeur(int** g, int s1, int size) {
    bool* vue = (bool*) malloc(sizeof(bool)*size*size);
    for (int i=0; i<size*size; i++) {
        vue[i] = false;
    }
    file* sac = (file*) malloc(sizeof(file));
    list* il = init_list(itv(s1));
    sac->d = il;
    sac->f = il;

    int dist = 0;
    while (sac->d != NULL) {
        int* ps = (int*) extract_file(sac);
        int s = *ps;
        if (vue[s] == false) {
            vue[s] = true;
            dist++;
            if (g[s][0] != -1 && g[s][0] != s) add_file(sac, itv(g[s][0]));
            if (g[s][1] != -1 && g[s][1] != s) add_file(sac, itv(g[s][1]));

            g[s][3] = 1;
        }
        free(ps);
    }
    free_pf(sac, free);
    free(vue);
    return dist;
}

int main() {
    FILE* input = fopen("input.txt", "r");
    char* str = (char*) malloc(sizeof(char)*1000);
    int size = 140;
    char S = '|';

    int** grid = (int**) malloc(sizeof(int*)*size*size);
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            grid[i*size+j] = (int*) malloc(sizeof(int)*4);
            grid[i*size+j][0] = -1;
            grid[i*size+j][1] = -1;
            grid[i*size+j][2] = 0;
            grid[i*size+j][3] = 0;
        }
    }
    
    int si = -1;
    int sj = -1;
    int i = 0;
    while (fscanf(input, "%[^\n]\n", str) != EOF) {
        int j = 0;
        while (str[j] != 0) {
            if (str[j] == 'S') {
                si = i;
                sj = j;
                str[j] = S;
            }

            if (str[j] == '|') {
                grid[i*size+j][0] = get_ind(i+1, j, size);
                grid[i*size+j][1] = get_ind(i-1, j, size);
                grid[i*size+j][2] = 2;
            } else if (str[j] == '-') {
                grid[i*size+j][0] = get_ind(i, j+1, size);
                grid[i*size+j][1] = get_ind(i, j-1, size);
            } else if (str[j] == 'L') {
                grid[i*size+j][0] = get_ind(i-1, j, size);
                grid[i*size+j][1] = get_ind(i, j+1, size);
                grid[i*size+j][2] = 1;
            } else if (str[j] == 'J') {
                grid[i*size+j][0] = get_ind(i-1, j, size);
                grid[i*size+j][1] = get_ind(i, j-1, size);
                grid[i*size+j][2] = -1;
            } else if (str[j] == '7') {
                grid[i*size+j][0] = get_ind(i+1, j, size);
                grid[i*size+j][1] = get_ind(i, j-1, size);
                grid[i*size+j][2] = 1;
            } else if (str[j] == 'F') {
                grid[i*size+j][0] = get_ind(i+1, j, size);
                grid[i*size+j][1] = get_ind(i, j+1, size);
                grid[i*size+j][2] = -1;
            }
            j++;
        }
        i++;
    }

    fclose(input);
    free(str);

    int dist = parcour_largeur(grid, get_ind(si,sj,size), size);
    if (dist%2 == 0) dist /= 2;
    else dist = (dist+1)/2;
    printf("partie 1 : %d\n", dist);

    int part2 = 0;
    for (int i=0; i<size*size; i++) {
        if (grid[i][3] == 0) {
            int nb = 0;
            int j = i+1;
            while (j%size != 0) {
                if (grid[j][3] == 1) nb += grid[j][2];
                j++;
            }
            if (abs(nb/2)%2 == 1) part2++;
        }
    }
    printf("partie 2 : %d\n", part2);

    for (int i=0; i<size*size; i++) {
        free(grid[i]);
    }
    free(grid);
    
    return 0;
}