#include <stdio.h>
#define V 5
#define E 7

struct Arista { int u, v, w; };
struct Arista a[E] = {
    {0,1,2}, {0,3,6}, {1,2,3}, {1,3,8}, {1,4,5}, {2,4,7}, {3,4,9}
};
int padre[V];

int buscar(int x) {
    while (padre[x] != x) x = padre[x];
    return x;
}

int main() {
    int i, j, ra, rb;
    int total = 0;
    struct Arista t;

    for (i = 0; i < E - 1; i++)
        for (j = 0; j < E - 1 - i; j++)
            if (a[j].w > a[j + 1].w) {
                t = a[j];
                a[j] = a[j + 1];
                a[j + 1] = t;
            }

    for (i = 0; i < V; i++) padre[i] = i;

    printf("Aristas del AEM (Kruskal):\n");
    for (i = 0; i < E; i++) {
        ra = buscar(a[i].u);
        rb = buscar(a[i].v);
        if (ra != rb) {
            printf("%d - %d (peso %d)\n", a[i].u, a[i].v, a[i].w);
            padre[ra] = rb;
            total += a[i].w;
        }
    }
    printf("Peso total: %d\n", total);
    return 0;
}
