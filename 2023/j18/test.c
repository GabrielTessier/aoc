
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

struct segment{
    int dx;
    int dy;
    int fx;
    int fy;
};
typedef struct segment segment;

int nbg(int i, int j, segment** seg, int nbseg) {
    int nb = 0;
    for (int k=0; k<nbseg; k++) {
        if (seg[k]->dx == seg[k]->fx && seg[k]->dx > i) {
            if ((seg[k]->dy < j && seg[k]->fy > j) || (seg[k]->dy > j && seg[k]->fy < j)) nb+=2;
            else if (seg[k]->dy < j && seg[k]->fy == j) nb++;
            else if (seg[k]->dy == j && seg[k]->fy > j) nb++;
            else if (seg[k]->dy > j && seg[k]->fy == j) nb--;
            else if (seg[k]->dy == j && seg[k]->fy < j) nb--;
        }
    }
    return (int) nb/2;
}

bool appartient(int i, int j, segment** seg, int nbseg) {
    for (int k=0; k<nbseg; k++) {
        if (seg[k]->dx == seg[k]->fx && seg[k]->dx == i && ((seg[k]->dy <= j && seg[k]->fy >= j) || (seg[k]->dy >= j && seg[k]->fy <= j))) return true;
        if (seg[k]->dy == seg[k]->fy && seg[k]->dy == j && ((seg[k]->dx <= i && seg[k]->fx >= i) || (seg[k]->dx >= i && seg[k]->fx <= i))) return true;
    }
    return false;
}

int hexToDec(char* hex, int ind) {
    int n = 0;
    int p=1;
    for (int i=4; i>=0; i--) {
        int v = (hex[i+ind]>='0' && hex[i+ind]<='9')?hex[i+ind]-'0':hex[i+ind]-'a'+10;
        n += v*p;
        p*=16;
    }
    return n;
}

void part1(char* filename) {
    FILE* input = fopen(filename, "r");
    char* str = (char*) malloc(sizeof(char)*100);

    int size = 0;
    while (fscanf(input, "%[^\n]\n", str) != EOF) size++;
    fclose(input);
    input = fopen(filename, "r");

    segment** seg = (segment**) malloc(sizeof(segment*)*size);

    int posx = 0;
    int posy = 0;

    int i=0;

    int minx = 0;
    int miny = 0;
    int maxx = 0;
    int maxy = 0;

    while (fscanf(input, "%[^\n]\n", str) != EOF) {
        int val = 0;
        int j = 2;
        while (str[j] != ' ') {
            val *= 10;
            val += str[j]-'0';
            j++;
        }
        segment* s = (segment*) malloc(sizeof(segment));
        s->dx = posx;
        s->dy = posy;
        if (str[0] == 'U') posy -= val;
        else if (str[0] == 'D') posy += val;
        else if (str[0] == 'R') posx += val;
        else if (str[0] == 'L') posx -= val;
        s->fx = posx;
        s->fy = posy;
        seg[i] = s;
        i++;
        if (posx < minx) minx = posx;
        if (posx > maxx) maxx = posx;
        if (posy < miny) miny = posy;
        if (posy > maxy) maxy = posy;
    }
    fclose(input);

    int nb = 0;
    for (int i=minx; i<=maxx; i++) {
        for (int j=miny; j<=maxy; j++) {
            bool in = nbg(i,j,seg, size)%2 == 1;
            bool ap = appartient(i,j,seg, size);
            if (in || ap) nb++;
        }
    }

    printf("partie 1 : %d\n", nb);

    for (int i=0; i<size; i++) {
        //printf("%d, %d, %d, %d\n", seg[i]->dx, seg[i]->dy, seg[i]->fx, seg[i]->fy);
        free(seg[i]);
    }
    free(seg);
    free(str);
}

void part2(char* filename) {
    FILE* input = fopen(filename, "r");
    char* str = (char*) malloc(sizeof(char)*100);

    int size = 0;
    while (fscanf(input, "%[^\n]\n", str) != EOF) size++;
    fclose(input);
    input = fopen(filename, "r");

    segment** seg = (segment**) malloc(sizeof(segment*)*size);

    int posx = 0;
    int posy = 0;

    int i=0;

    int minx = 0;
    int miny = 0;
    int maxx = 0;
    int maxy = 0;

    while (fscanf(input, "%[^\n]\n", str) != EOF) {
        int j=0;
        while (str[j] != '#') {
            j++;
        }
        int val = hexToDec(str, j+1);
        int last=0;
        while (str[j] != ')') {
            last = str[j]-'0';
            j++;
        }
        segment* s = (segment*) malloc(sizeof(segment));
        s->dx = posx;
        s->dy = posy;
        if (last == 3) posy -= val;
        else if (last == 1) posy += val;
        else if (last == 0) posx += val;
        else if (last == 2) posx -= val;
        s->fx = posx;
        s->fy = posy;
        seg[i] = s;
        i++;
        if (posx < minx) minx = posx;
        if (posx > maxx) maxx = posx;
        if (posy < miny) miny = posy;
        if (posy > maxy) maxy = posy;
    }
    fclose(input);

    printf("minx : %d, maxx : %d, miny : %d, maxy : %d\n", minx, maxx, miny, maxy);
    long nb = 0;
    for (int i=minx; i<=maxx; i++) {
        for (int j=miny; j<=maxy; j++) {
            bool in = nbg(i,j,seg, size)%2 == 1;
            bool ap = appartient(i,j,seg, size);
            if (in || ap) nb++;
        }
    }

    printf("partie 2 : %ld\n", nb);

    for (int i=0; i<size; i++) {
        //printf("%d, %d, %d, %d\n", seg[i]->dx, seg[i]->dy, seg[i]->fx, seg[i]->fy);
        free(seg[i]);
    }
    free(seg);
    free(str);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("%s <input filename>\n", argv[0]);
        return 0;
    }

    part1(argv[1]);
    printf("partie 2 ne termine pas.\n");
    //part2(argv[1]);

    return 0;
}