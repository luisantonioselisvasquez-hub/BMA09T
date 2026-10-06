#include <stdio.h>
#define V 5
#define E 7

int u[E] = {0, 0, 1, 1, 1, 2, 3};
int v[E] = {1, 3, 2, 3, 4, 4, 4};
int w[E] = {2, 6, 3, 8, 5, 7, 9};

int buscar(int p[], int x) {
    while (p[x] != x) x = p[x];
    return x;
}

int sinCiclos(int mascara) {
    int p[V];
    int i, a, b;
    for (i = 0; i < V; i++) p[i] = i;
    for (i = 0; i < E; i++)
        if (mascara & (1 << i)) {
            a = buscar(p, u[i]);
            b = buscar(p, v[i]);
            if (a == b) return 0;
            p[a] = b;
        }
    return 1;
}

int main() {
    int m, i, aristas, peso;
    int mejor = -1, pesoMin = 9999, cuantos = 0;

    for (m = 0; m < (1 << E); m++) {
        aristas = 0;
        peso = 0;
        for (i = 0; i < E; i++)
            if (m & (1 << i)) {
                aristas++;
                peso += w[i];
            }
        if (aristas == V - 1 && sinCiclos(m)) {
            cuantos++;
            if (peso < pesoMin) {
                pesoMin = peso;
                mejor = m;
            }
        }
    }

    printf("Arboles de expansion encontrados: %d\n", cuantos);
    printf("Arbol de expansion MINIMA:\n");
    for (i = 0; i < E; i++)
        if (mejor & (1 << i))
            printf("%d - %d (peso %d)\n", u[i], v[i], w[i]);
    printf("Peso total: %d\n", pesoMin);
    return 0;
}
