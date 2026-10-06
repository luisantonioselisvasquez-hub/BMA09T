#include <stdio.h>
#define N 5

int g[N][N] = {
    {0, 2, 0, 6, 0},
    {2, 0, 3, 8, 5},
    {0, 3, 0, 0, 7},
    {6, 8, 0, 0, 9},
    {0, 5, 7, 9, 0}
};
int visitado[N];

void dfs(int u) {
    int v;
    visitado[u] = 1;
    for (v = 0; v < N; v++)
        if (g[u][v] != 0 && !visitado[v]) {
            printf("%d - %d\n", u, v);
            dfs(v);
        }
}

int main() {
    printf("Aristas del arbol de expansion:\n");
    dfs(0);
    return 0;
}
