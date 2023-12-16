
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "pile_file.h"

struct gr {
    bool* dirs;
};
typedef struct gr gr;

enum dir {RIGHT, LEFT, UP, DOWN};

struct turtle {
    int x;
    int y;
    enum dir dir;
};
typedef struct turtle turtle;

void move(gr*** grid_res, turtle* t) {
    grid_res[t->y][t->x]->dirs[(int)t->dir] = true;
    if (t->dir == LEFT) t->x--;
    else if (t->dir == RIGHT) t->x++;
    else if (t->dir == UP) t->y--;
    else if (t->dir == DOWN) t->y++;
}

bool sadd(gr*** grid_res, int** grid, turtle* t, int size) {
    if (t->x < 0 || t->x >= size || t->y < 0 || t->y >= size || (grid_res[t->y][t->x]->dirs[(int)t->dir] && grid[t->y][t->x] == 0)) return false;
    else return true;
}

int get_size(file* f) {
    int r = 0;
    list* a = f->d;
    while (a != NULL) {
        a = a->post;
        r++;
    }
    return r;
}

int find_nb(int** grid, int size, int sx, int sy, enum dir dir) {
    gr*** grid_res = malloc(sizeof(gr**)*size);
    for (int i=0; i<size; i++) {
        grid_res[i] = malloc(sizeof(gr*)*size);
        for (int j=0; j<size; j++) {
            grid_res[i][j] = malloc(sizeof(gr));
            grid_res[i][j]->dirs = malloc(sizeof(int)*4);
            for (int k = 0; k<4; k++) {
                grid_res[i][j]->dirs[k] = false;
            }
        }
    }

    turtle* tu = malloc(sizeof(turtle));
    tu->dir = dir;
    tu->x = sx;
    tu->y = sy;
    file* f = init_file(tu);

    while ((tu = (turtle*) extract_file(f)) != NULL) {
        if (grid[tu->y][tu->x] == 0) move(grid_res, tu);
        else if (grid[tu->y][tu->x] == 1) {
            if (tu->dir == RIGHT) tu->dir = UP;
            else if (tu->dir == LEFT) tu->dir = DOWN;
            else if (tu->dir == UP) tu->dir = RIGHT;
            else if (tu->dir == DOWN) tu->dir = LEFT;
            move(grid_res, tu);
        } else if (grid[tu->y][tu->x] == 2) {
            if (tu->dir == RIGHT) tu->dir = DOWN;
            else if (tu->dir == LEFT) tu->dir = UP;
            else if (tu->dir == UP) tu->dir = LEFT;
            else if (tu->dir == DOWN) tu->dir = RIGHT;
            move(grid_res, tu);
        } else if (grid[tu->y][tu->x] == 3) {
            if (tu->dir == UP || tu->dir == DOWN) move(grid_res, tu);
            else {
                tu->dir = UP;
                turtle* tt = malloc(sizeof(turtle));
                tt->dir = DOWN;
                tt->x = tu->x;
                tt->y = tu->y;
                move(grid_res, tu);
                move(grid_res, tt);
                if (sadd(grid_res, grid, tt, size)) add_file(f, tt);
                else free(tt);
            }
        } else if (grid[tu->y][tu->x] == 4) {
            if (tu->dir == LEFT || tu->dir == RIGHT) move(grid_res, tu);
            else {
                tu->dir = LEFT;
                turtle* tt = malloc(sizeof(turtle));
                tt->dir = RIGHT;
                tt->x = tu->x;
                tt->y = tu->y;
                move(grid_res, tu);
                move(grid_res, tt);
                if (sadd(grid_res, grid, tt, size)) add_file(f, tt);
                else free(tt);
            }
        }
        if (sadd(grid_res, grid, tu, size)) add_file(f, tu);
        else free(tu);
    }

    int res = 0;
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            if (grid_res[i][j]->dirs[0] || grid_res[i][j]->dirs[1] || grid_res[i][j]->dirs[2] || grid_res[i][j]->dirs[3]) res++;
        }
    }

    free_file(f, free);
    for (int i=0; i<size; i++) {
        for (int j=0; j<size; j++) {
            free(grid_res[i][j]->dirs);
            free(grid_res[i][j]);
        }
        free(grid_res[i]);
    }
    free(grid_res);
    return res;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("%s <input file name>\n", argv[0]);
        return 0;
    }
    
    FILE* input = fopen(argv[1], "r");
    int size = 0;
    char c;
    fscanf(input, "%c", &c);
    while (c != '\n') {
        size++;
        fscanf(input, "%c", &c);
    }
    fclose(input);

    input = fopen(argv[1], "r");
    char* str = malloc(sizeof(char)*(size+1));
    int** grid = malloc(sizeof(int*)*size);
    int i=0;
    while (fscanf(input, "%[^\n]\n", str) != EOF) {
        grid[i] = malloc(sizeof(int)*size);

        int j=0;
        while (str[j] != 0) {
            if (str[j] == '.') grid[i][j] = 0;
            else if (str[j] == '/') grid[i][j] = 1;
            else if (str[j] == '\\') grid[i][j] = 2;
            else if (str[j] == '|') grid[i][j] = 3;
            else if (str[j] == '-') grid[i][j] = 4;
            j++;
        }

        i++;
    }

    
    printf("partie 1 : %d\n", find_nb(grid, size, 0, 0, RIGHT));

    int max = 0;
    for (int i=0; i<size; i++) {
        int m1 = find_nb(grid, size, 0, i, RIGHT);
        int m2 = find_nb(grid, size, i, 0, DOWN);
        int m3 = find_nb(grid, size, size-1, i, LEFT);
        int m4 = find_nb(grid, size, i, size-1, UP);
        max = (m1>max)?m1:max;
        max = (m2>max)?m2:max;
        max = (m3>max)?m3:max;
        max = (m4>max)?m4:max;
    }

    printf("partie 2 : %d\n", max);

    for (int i=0; i<size; i++) {
        free(grid[i]);
    }
    free(grid);
    free(str);
    fclose(input);
}