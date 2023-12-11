
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int main() {
    FILE* input = fopen("input.txt", "r");
    char* str = (char*) malloc(sizeof(char)*1000);
    int res = 0;
    int id = 0;
    while (fscanf(input, "%[^\n]\n", str) != EOF) {
        id++;
        //bool valide = true;
        int max[] = {0,0,0};
        char* line = str;
        while (line[0] != ':') line++;
        line++;
        line++;
        while (line[0] != 0) {
            int nb = 0;
            while (line[0] != ' ') {
                nb *= 10;
                nb += line[0]-'0';
                line++;
            }
            line++;

            char max_fl[] = {'r', 'g', 'b'};
            for (int i=0; i<3; i++) {
                if (line[0] == max_fl[i] && nb > max[i]) max[i] = nb;
            }

            /*if (nb > max(line[0])) {
                valide = false;
            }*/
            while (line[0] != 0 && line[0] != ',' && line[0] != ';') line++;
            if (line[0] != 0) {
                line++;
                line++;
            }
        }
        //if (valide) res += id;
        res += max[0]*max[1]*max[2];
        printf("str : %s\nid : %d, red : %d, green : %d, blue : %d, res : %d\n", str, id, max[0], max[1], max[2], res);
    }
    printf("res : %d\n", res);
    free(str);
    fclose(input);
    return 0;
}