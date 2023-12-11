
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main() {
    FILE* input = fopen("input.txt", "r");
    char* str = (char*) malloc(sizeof(char)*1000);
    int left_size = 10;
    int right_size = 25;
    int cart_nb = 220;
    int* nb_cart = (int*) malloc(sizeof(int)*cart_nb);
    for (int i=0; i<cart_nb; i++) nb_cart[i] = 1;
    int c_cart = 0;
    while (fscanf(input, "%[^\n]\n", str) != EOF) {
        int* left = malloc(sizeof(int)*left_size);
        for (int i=0; i<left_size; i++) left[i] = 0;
        char* line = str;
        while (line[0] != ':') line++;
        line++;
        while (line[0] == ' ') line++;
        int i = 0;
        while (line[0] != '|') {
            if (line[0] != ' ') {
                printf("i : %d, ", i);
                printf("val : %d\n", left[i]);
                left[i] *= 10;
                left[i] += line[0]-'0';
            }
            if (line[0] == ' ' && line[1] != ' ') i++;
            line++;
        }
        line++;
        while (line[0] == ' ') line++;
        int* right = malloc(sizeof(int)*right_size);
        for (int i=0; i<right_size; i++) right[i] = 0;
        i = 0;
        while (line[0] != 0) {
            if (line[0] != ' ') {
                right[i] *= 10;
                right[i] += line[0]-'0';
            }
            if (line[0] == ' ' && line[1] != ' ') i++;
            line++;
        }
        int nb = 0;
        for (int j=0; j<right_size; j++) {
            for (int k=0; k<left_size; k++) {
                if (left[k] == right[j]) {
                    nb++;
                }
            }
        }
        printf("c_crat : %d, mm : %d\n", c_cart, c_cart+nb);
        for (int j=c_cart+1; j<=c_cart+nb; j++) {
            nb_cart[j] += nb_cart[c_cart];
        }
        //sum += nb;
        free(left);
        free(right);
        c_cart++;
    }

    int sum = 0;
    for (int i=0; i<cart_nb; i++) sum += nb_cart[i];
    printf("sum : %d\n", sum);

    free(nb_cart);
    free(str);
    fclose(input);

    return 0;
}