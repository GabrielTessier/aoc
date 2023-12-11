
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct couple_s {
    int a;
    int b;
    int c;
};
typedef struct couple_s couple_t;

int get_val(char l) {
    char ll[] = {'J', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'Q', 'K', 'A'};
    for (int i=0; i<13; i++) {
        if (l == ll[i]) return i;
    }
    return -1;
}

void sort(couple_t** data, int size) {
    for (int i=0; i<size; i++) {
        int min = i;
        for (int j=i; j<size; j++) {
            printf("a : %d, b : %d, c : %d\n", data[j]->a, data[j]->b, data[j]->c);
            if (data[j]->a < data[min]->a || (data[j]->a == data[min]->a && data[j]->b < data[min]->b)) {
                min = j;
            }
        }
        couple_t* tmp = data[min];
        data[min] = data[i];
        data[i] = tmp;
    }
}

int main() {
    FILE* input = fopen("input.txt", "r");
    int size = 1000;
    char* str = (char*) malloc(sizeof(char)*1000);

    couple_t** data = malloc(sizeof(couple_t*)*size);

    int ind = 0;
    
    while (fscanf(input, "%[^\n]\n", str) != EOF) {
        char* line = str;
        int nb = 0;
        int* nb_v = malloc(sizeof(int)*13);
        for (int i=0; i<13; i++) nb_v[i] = 0;
        int nb_l[] = {0,0,0,0};
        while (line[0] != ' ') {
            int v = get_val(line[0]);
            nb *= 13;
            nb += v;
            nb_v[v] ++;
            if (v != 0) {
                if (nb_v[v] == 2) nb_l[0]++;
                if (nb_v[v] == 3) {
                    nb_l[1]++;
                    nb_l[0]--;
                }
                if (nb_v[v] == 4) {
                    nb_l[2]++;
                    nb_l[1]--;
                }
                if (nb_v[v] == 5) {
                    nb_l[3]++;
                    nb_l[2]--;
                }
            }
            line++;
        }
        line++;
        int c = 0;
        while (line[0] != 0) {
            printf("cc : %c\n", line[0]);
            c *= 10;
            c += line[0]-'0';
            line++;
        }
        int sc = 0;
        while (nb_v[0]>0) {
            if (nb_l[2] != 0) {
                nb_l[2]--;
                nb_l[3]++;
            } else if (nb_l[1] != 0) {
                nb_l[1]--;
                nb_l[2]++;
            } else if (nb_l[0] != 0) {
                nb_l[0]--;
                nb_l[1]++;
            } else {
                nb_l[0]++;
            }
            nb_v[0]--;
        }
        if (nb_l[3] == 1) sc = 6;
        else if (nb_l[2] == 1) sc = 5;
        else if (nb_l[1] == 1 && nb_l[0] == 1) sc = 4;
        else if (nb_l[1] == 1 && nb_l[0] == 0) sc = 3;
        else if (nb_l[0] == 2) sc = 2;
        else if (nb_l[0] == 1) sc = 1;
        else sc = 0;
        couple_t* cc = malloc(sizeof(couple_t));
        cc->a = sc;
        cc->b = nb;
        cc->c = c;
        data[ind] = cc;
        ind++;
    }

    for (int i=0; i<size; i++) {
        printf("data[i] : a : %d, b : %d, c : %d\n", data[i]->a, data[i]->b, data[i]->c);
    } 

    sort(data, size);

    int res = 0;
    for (int i=0; i<size; i++) {
        printf("data[i] : a : %d, b : %d, c : %d\n", data[i]->a, data[i]->b, data[i]->c);
        res += (i+1)*data[i]->c;
    } 
    printf("res : %d\n", res);

}