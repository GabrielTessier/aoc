
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define VIDE -1
#define SYMBOLE -2

bool is_number(int a) {
    if (a>='0' && a<='9') return true;
    return false;
}

bool is_sym(int a) {
    if (is_number(a) == false && a != '.') return true;
    return false;
}

bool touch(int* grid, int pos, int width, int height) {
    if (pos%width != 0 && grid[pos-1] == SYMBOLE) return true;
    else if (pos%width != width-1 && grid[pos+1] == SYMBOLE) return true;
    else if ((int) pos/width != 0 && grid[pos-width] == SYMBOLE) return true;
    else if ((int) pos/width != height-1 && grid[pos+width] == SYMBOLE) return true;
    else if (pos%width != 0 && (int) pos/width != 0 && grid[pos-1-width] == SYMBOLE) return true;
    else if (pos%width != 0 && (int) pos/width != height-1 && grid[pos-1+width] == SYMBOLE) return true;
    else if (pos%width != width-1 && (int) pos/width != 0 && grid[pos+1-width] == SYMBOLE) return true;
    else if (pos%width != width-1 && (int) pos/width != height-1 && grid[pos+1+width] == SYMBOLE) return true;
    else return false;
}

void touch_star(int* grid, int* grid_gear, int* grid_ratio, bool* grid_touch, int nb, int pos, int width, int height) {
    if (pos%width != 0 && grid[pos-1] == SYMBOLE) {
        int k = pos-1;
        if (grid_touch[k] == false) {
            grid_touch[k] = true;
            grid_gear[k] = grid_gear[k]+1;
            grid_ratio[k] = grid_ratio[k]*nb;
            printf("touch (pos : %d)", k);
        }
    } 
    if (pos%width != width-1 && grid[pos+1] == SYMBOLE) {
        int k = pos+1;
        if (grid_touch[k] == false) {
            grid_touch[k] = true;
            grid_gear[k] = grid_gear[k]+1;
            grid_ratio[k] = grid_ratio[k]*nb;
            printf("touch (pos : %d)", k);
        }
    } 
    if ((int) pos/width != 0 && grid[pos-width] == SYMBOLE) {
        int k = pos-width;
        if (grid_touch[k] == false) {
            grid_touch[k] = true;
            grid_gear[k] = grid_gear[k]+1;
            grid_ratio[k] = grid_ratio[k]*nb;
            printf("touch (pos : %d)", k);
        }
    } 
    if ((int) pos/width != height-1 && grid[pos+width] == SYMBOLE) {
        int k = pos+width;
        if (grid_touch[k] == false) {
            grid_touch[k] = true;
            grid_gear[k] = grid_gear[k]+1;
            grid_ratio[k] = grid_ratio[k]*nb;
            printf("touch (pos : %d)", k);
        }
    } 
    if (pos%width != 0 && (int) pos/width != 0 && grid[pos-1-width] == SYMBOLE) {
        int k = pos-1-width;
        if (grid_touch[k] == false) {
            grid_touch[k] = true;
            grid_gear[k] = grid_gear[k]+1;
            grid_ratio[k] = grid_ratio[k]*nb;
            printf("touch (pos : %d)", k);
        }
    } 
    if (pos%width != 0 && (int) pos/width != height-1 && grid[pos-1+width] == SYMBOLE) {
        int k = pos-1+width;
        if (grid_touch[k] == false) {
            grid_touch[k] = true;
            grid_gear[k] = grid_gear[k]+1;
            grid_ratio[k] = grid_ratio[k]*nb;
            printf("touch (pos : %d)", k);
        }
    } 
    if (pos%width != width-1 && (int) pos/width != 0 && grid[pos+1-width] == SYMBOLE) {
        int k = pos+1-width;
        if (grid_touch[k] == false) {
            grid_touch[k] = true;
            grid_gear[k] = grid_gear[k]+1;
            grid_ratio[k] = grid_ratio[k]*nb;
            printf("touch (pos : %d)", k);
        }
    } 
    if (pos%width != width-1 && (int) pos/width != height-1 && grid[pos+1+width] == SYMBOLE) {
        int k = pos+1+width;
        if (grid_touch[k] == false) {
            grid_touch[k] = true;
            grid_gear[k] = grid_gear[k]+1;
            grid_ratio[k] = grid_ratio[k]*nb;
            printf("touch (pos : %d)", k);
        }
    }
}

int main() {
    FILE* input = fopen("input.txt", "r");
    char* str = (char*) malloc(sizeof(char)*1000);
    int height = 140;
    int width = 140;
    int* grid = (int*) malloc(sizeof(int)*height*width);
    int* grid_gear = (int*) malloc(sizeof(int)*height*width);
    int* grid_ratio = (int*) malloc(sizeof(int)*height*width);
    for (int i=0; i<height*width; i++) {
        grid[i] = VIDE;
        grid_gear[i] = 0;
        grid_ratio[i] = 1;
    }
    int i = 0;
    while (fscanf(input, "%[^\n]\n", str) != EOF) {
        for (int j=0; j<width; j++) {
            if (str[j]>='0' && str[j]<='9') grid[j+i*width] = str[j]-'0';
            else if (str[j]=='*') grid[j+i*width] = SYMBOLE;
            else grid[j+i*width] = VIDE;
        }
        i++;
    }
    free(str);
    int sum = 0;
    i = 0;
    while (i<height*width) {
        if (grid[i] >= 0) {
            int j = i;
            int nb = 0;
            while (j%width != width-1 && grid[j] >= 0) {
                nb *= 10;
                nb += grid[j];
                j++;
            }
            if (grid[j]>=0) {
                nb *= 10;
                nb += grid[j];
                j++;
            }
            j=i;
            printf("nb : %d, ", nb);
            bool* grid_touch = (bool*) malloc(sizeof(bool)*height*width);
            for (int k=0; k<width*height; k++) {
                grid_touch[k] = false;
            }
            while (j%width != width-1 && grid[j] >= 0) {
                touch_star(grid, grid_gear, grid_ratio, grid_touch, nb, j, width, height);
                j++;
            }
            if (grid[j]>=0) {
                touch_star(grid, grid_gear, grid_ratio, grid_touch, nb, j, width, height);
                j++;
            }
            free(grid_touch);
            printf("\n");
            i = j;
        } else {
            i++;
        }
    }
    for (int j=0; j<width*height; j++) {
        if (grid_gear[j]>1) sum += grid_ratio[j];
    }
    printf("sum : %d\n", sum);

    free(grid);
    free(grid_gear);
    free(grid_ratio);
    fclose(input);

    return 0;
}