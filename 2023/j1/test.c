
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

int get_val(char* str, int pos) {
    char* nb[] = {"one", "two", "three", "four", "five", "six", "seven", "eight", "nine"};
    for (int i=0; i<9; i++) {
        int size = strlen(nb[i]);
        if (pos+size <= strlen(str)) {
            bool valide = true;
            for (int j=0; j<size; j++) {
                if (str[pos+j] != nb[i][j]) valide = false;
            }
            if (valide) return i+1;
        }
    }
    return -1;
}

int main() {
    FILE* input = fopen("input.txt", "r");
    char* str = (char*) malloc(sizeof(char)*100);
    int res = 0;
    while (fscanf(input, "%[^\n]\n", str) != EOF) {
        int i=0;
        int s = -1;
        int f = -1;
        while (str[i] != 0) {
            int val = -1;
            if (str[i] >= '0' && str[i] <= '9') val = str[i] - '0';
            else val = get_val(str, i);
            if (val != -1) {
                f = val;
                if (s == -1) s = val;
            }
            i++;
        }
        res += s*10+f;
    }
    printf("val : %d\n", res);
    fclose(input);
    free(str);
    return 0;
}