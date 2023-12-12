
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool valide(int* l, bool* b_list, int* val, int size, int val_size) {
    int j = 0;
    int b = 0;
    int s = 0;
    for (int i=0; i<size; i++) {
        //if (j >= val_size) return false;
        if (l[i] == 1) s++;
        else if (l[i] == 2) {
            if (b_list[b]) s++;
            else {
                if (s != 0) {
                    if (val[j] != s) return false;
                    j++;
                }
                s = 0;
            }
            b++;
        } else if (l[i] == 0) {
            if (s != 0) {
                if (val[j] != s) return false;
                j++;
            }
            s = 0;
        }
    }
    if (s != 0) {
        if (val[j] != s) return false;
        j++;
    }
    s = 0;
    for (int i=j; i<val_size; i++) {
        if (val[i] != 0) return false;
    }
    return true;
}

int get_left_size(char* str) {
    int i = 0;
    while (str[i] != ' ') i++;
    return i;
}

int get_val_size(char* str, int i) {
    int j = 0;
    while (str[i] != 0) {
        if (str[i] == ',') j++;
        i++;
    }
    return j+1;
}

void next_conf(bool* b_list, int nbpi, int i) {
    int n = nbpi-i;
    if (n<0) return ;
    while (n>=0 && b_list[n] == 0) n--;
    if (n<0) return;
    if (n < nbpi-i) {
        b_list[n] = 0;
        b_list[n+1] = 1;
    } else {
        next_conf(b_list, nbpi, i+1);
        n--;
        while (n>=0 && b_list[n] == 0) n--;
        if (n<0) return;
        b_list[nbpi-i] = 0;
        b_list[n+1] = 1;
    }
}

long parmi(long p, long n) {
    long binom;
    long k;
    if (p > n-p) p = n-p;
    for (binom = 1, k = 1; k <= p; k++) binom = (binom * (n-p+k)) / k;
    return binom;
}

int main() {
    FILE* input = fopen("input.txt", "r");

    printf("PARTIE 2 NE MARCHE PAS !!!\n");
    
    // Partie 2 ne marche pas !!!
    bool part2 = false; // mettre false pour partie 1 et true pour partie 2

    char* str = (char*) malloc(sizeof(char)*1000);

    long res = 0;

    int pos = 1;
    
    while (fscanf(input, "%[^\n]\n", str) != EOF) {
        int init_left_size = get_left_size(str);
        int left_size = (part2)?init_left_size*5+4:init_left_size;
        int* l = malloc(sizeof(int)*left_size);
        for (int k=0; k<left_size; k++) l[k] = 0;
        int i = 0;
        int nbpi = 0;
        while (str[i] != ' ') {
            if (str[i] == '.') l[i] = 0;
            else if (str[i] == '#') l[i] = 1;
            else if (str[i] == '?') {
                l[i] = 2;
                nbpi++;
            }
            i++;
        }
        if (part2) {
            for (int j=1; j<5; j++) {
                l[j*(init_left_size+1)-1] = 2;
                for (int k=0; k<init_left_size; k++) {
                    l[j*(init_left_size+1)+k] = l[k];
                }
            }
        }
        //printf("str : %s\n", str);
        //printf("nbpi : %d\n", nbpi);
        //for (int k=0; k<(int) left_size; k++) printf("%d, ", l[k]);
        //printf("\n");
        i++;
        int init_val_size = get_val_size(str, i);
        int val_size = ((part2)?init_val_size*5:init_val_size)+1;
        int* val = (int*) malloc(sizeof(int)*val_size);
        for (int j=0; j<val_size; j++) val[j] = 0;
        int v = 0;
        while (str[i] != 0) {
            if (str[i] == ',') {
                v++;
            } else {
                val[v] *= 10;
                val[v] += str[i]-'0';
            }
            i++;
        }
        if (part2) {
            for (int j=1; j<5; j++) {
                for (int k=0; k<init_val_size; k++) {
                    val[j*init_val_size+k] = val[k];
                }
            }
            nbpi  = nbpi*5+4;
        }

        int val_sum = 0;
        for (int k=0; k<val_size; k++) val_sum += val[k];
        int nb1 = 0;
        for (int k=0; k<left_size; k++) nb1 += (l[k]==1)?1:0;
        int nb2 = 0;
        for (int k=0; k<left_size; k++) nb2 += (l[k]==2)?1:0;

        //for (int k=0; k<val_size; k++) printf("%d, ", val[k]);
        //printf("\n");
        int nbconf = parmi(val_sum-nb1, nb2);
        //for (int k=0; k<val_sum-nb1; k++) nbconf *= nb2-k;
        bool* b_list = malloc(sizeof(bool)*nbpi);
        for (int k=0; k<nbpi; k++) b_list[k] = (k<val_sum-nb1)?1:0;
        long nb = 0;
        for (int j=0; j<nbconf; j++) {
            //for (int k=0; k<nbpi; k++) printf("%s, ", b_list[k]?"1":"0");
            if (valide(l, b_list, val, left_size, val_size)) {
                //printf("valide\n");
                nb++;
            }
            //else printf("\n");
            next_conf(b_list, nbpi, 1);
        }
        res += nb;
        //printf("pos : %d, nb : %ld\n", pos, nb);

        free(l);
        free(val);
        free(b_list);
        pos++;
    }

    printf("partie %d : %ld\n", (part2)?2:1, res);

    free(str);
    fclose(input);

    return 0;
}