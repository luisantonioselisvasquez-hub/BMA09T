#include <stdio.h>
#define V 5
#define INF 9999

int g[V][V] = {
    {0, 2, 0, 6, 0},
    {2, 0, 3, 8, 5},
    {0, 3, 0, 0, 7},
    {6, 8, 0, 0, 9},
    {0, 5, 7, 9, 0}
};

int main() {
    int enArbol[V] = {0};
    int total = 0;
    int k, i, j, min, x, y;

    enArbol[0] = 1;                   
    printf("Aristas del AEM (Prim):\n");

    for (k = 0; k < V - 1; k++) {
        min = INF;
        x = -1;
        y = -1;
        for (i = 0; i < V; i++)
            if (enArbol[i])
                for (j = 0; j < V; j++)
                    if (!enArbol[j] && g[i][j] != 0 && g[i][j] < min) {
                        min = g[i][j];
                        x = i;
                        y = j;
                    }
        printf("%d - %d (peso %d)\n", x, y, min);
        enArbol[y] = 1;
        total += min;
    }
    printf("Peso total: %d\n", total);
    return 0;
}
