/* ============================================================
 * caminos_minimos.c
 * ------------------------------------------------------------
 * Proyecto: Caminos Mínimos en Grafos
 * Alumno:   [Tu nombre aquí]
 * Curso:    Análisis de Algoritmos
 * Fecha:    Octubre 2026
 *
 * Implementa los tres algoritmos clásicos de caminos mínimos:
 *   - Dijkstra        (origen único, pesos no negativos)
 *   - Bellman-Ford    (origen único, admite negativos, detecta ciclos)
 *   - Floyd-Warshall  (todos los pares, admite negativos)
 *
 * Todo metido en un solo archivo para que sea más fácil de
 * entregar. Compilar con:
 *     gcc -Wall -Wextra -O2 -std=c11 -o caminos_minimos caminos_minimos.c
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

/* Uso INT_MAX como "infinito". No es infinito real, pero para
   los tamaños que manejamos aquí funciona perfecto. */
#define INF INT_MAX

/* ============================================================
 *  ESTRUCTURAS
 * ============================================================ */

/* ---------- Grafo con matriz de adyacencia ---------- */
typedef struct {
    int V;            // cuántos vértices tiene
    int E;            // cuántas aristas hay
    int **matriz;     // la matriz de pesos
} GrafoMatriz;

/* ---------- Grafo con lista de aristas (para Bellman-Ford) ---------- */
typedef struct {
    int u, v, peso;
} Arista;

typedef struct {
    int V;
    int E;
    Arista *aristas;
} GrafoAristas;

/* ---------- Heap para Dijkstra ---------- */
typedef struct {
    int vertice;
    int distancia;
} NodoHeap;

typedef struct {
    NodoHeap *datos;
    int *pos;         // dónde está cada vértice dentro del heap
    int tamano;
    int capacidad;
} MinHeap;

/* ============================================================
 *  GRAFO CON MATRIZ DE ADYACENCIA
 * ============================================================ */
GrafoMatriz *crearGrafoMatriz(int V) {
    GrafoMatriz *g = (GrafoMatriz *)malloc(sizeof(GrafoMatriz));
    if (!g) {
        printf("Ups... no hay memoria suficiente.\n");
        exit(1);
    }
    g->V = V;
    g->E = 0;
    g->matriz = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++) {
        g->matriz[i] = (int *)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) {
            /* La diagonal es 0 (distancia de un nodo a sí mismo).
               Todo lo demás empieza como "infinito". */
            g->matriz[i][j] = (i == j) ? 0 : INF;
        }
    }
    return g;
}

void agregarAristaMatriz(GrafoMatriz *g, int u, int v, int peso, bool dirigido) {
    if (u < 0 || u >= g->V || v < 0 || v >= g->V) return;
    g->matriz[u][v] = peso;
    if (!dirigido) g->matriz[v][u] = peso;
    g->E++;
}

void imprimirGrafoMatriz(GrafoMatriz *g) {
    printf("\n   Asi se ve el grafo (matriz de adyacencia):\n\n");
    printf("        ");
    for (int j = 0; j < g->V; j++) printf("%6d ", j);
    printf("\n      ");
    for (int j = 0; j < g->V; j++) printf("-------");
    printf("\n");
    for (int i = 0; i < g->V; i++) {
        printf("  %3d |", i);
        for (int j = 0; j < g->V; j++) {
            if (g->matriz[i][j] == INF) printf("%6s ", "oo");
            else                        printf("%6d ", g->matriz[i][j]);
        }
        printf("\n");
    }
}

void liberarGrafoMatriz(GrafoMatriz *g) {
    if (!g) return;
    for (int i = 0; i < g->V; i++) free(g->matriz[i]);
    free(g->matriz);
    free(g);
}

/* ============================================================
 *  GRAFO CON LISTA DE ARISTAS
 * ============================================================ */
GrafoAristas *crearGrafoAristas(int V, int E) {
    GrafoAristas *g = (GrafoAristas *)malloc(sizeof(GrafoAristas));
    g->V = V;
    g->E = 0;
    g->aristas = (Arista *)malloc(E * sizeof(Arista));
    return g;
}

void liberarGrafoAristas(GrafoAristas *g) {
    if (!g) return;
    free(g->aristas);
    free(g);
}

/* ============================================================
 *  RELAJACIÓN DE ARISTAS
 * ------------------------------------------------------------
 *  Esta es LA operación clave. Básicamente pregunta:
 *  "¿me conviene llegar a v pasando por u?"
 *
 *  Si el camino actual hacia v es más caro que ir a u + la
 *  arista (u,v), entonces actualizo.
 *
 *  Devuelvo true cuando efectivamente mejoré la distancia.
 * ============================================================ */
bool relajar(int *d, int *pi, int u, int v, int peso) {
    if (d[u] == INF) return false;   /* si ni siquiera llego a u, no sirve */
    if (d[v] > d[u] + peso) {
        d[v]  = d[u] + peso;
        pi[v] = u;
        return true;
    }
    return false;
}

/* ============================================================
 *  MIN-HEAP (para Dijkstra)
 *  Lo implementé a mano porque C no trae cola de prioridad.
 * ============================================================ */
MinHeap *crearMinHeap(int capacidad) {
    MinHeap *h = (MinHeap *)malloc(sizeof(MinHeap));
    h->datos = (NodoHeap *)malloc(capacidad * sizeof(NodoHeap));
    h->pos   = (int *)malloc(capacidad * sizeof(int));
    h->tamano = 0;
    h->capacidad = capacidad;
    for (int i = 0; i < capacidad; i++) h->pos[i] = -1;
    return h;
}

void liberarMinHeap(MinHeap *h) {
    free(h->datos);
    free(h->pos);
    free(h);
}

static void intercambiar(NodoHeap *a, NodoHeap *b) {
    NodoHeap t = *a; *a = *b; *b = t;
}

static void subir(MinHeap *h, int i) {
    while (i > 0) {
        int padre = (i - 1) / 2;
        if (h->datos[padre].distancia <= h->datos[i].distancia) break;
        h->pos[h->datos[padre].vertice] = i;
        h->pos[h->datos[i].vertice]     = padre;
        intercambiar(&h->datos[padre], &h->datos[i]);
        i = padre;
    }
}

static void bajar(MinHeap *h, int i) {
    while (1) {
        int izq = 2 * i + 1;
        int der = 2 * i + 2;
        int menor = i;
        if (izq < h->tamano && h->datos[izq].distancia < h->datos[menor].distancia)
            menor = izq;
        if (der < h->tamano && h->datos[der].distancia < h->datos[menor].distancia)
            menor = der;
        if (menor == i) break;
        h->pos[h->datos[menor].vertice] = i;
        h->pos[h->datos[i].vertice]     = menor;
        intercambiar(&h->datos[i], &h->datos[menor]);
        i = menor;
    }
}

void insertarHeap(MinHeap *h, int vertice, int distancia) {
    if (h->tamano == h->capacidad) return;
    int i = h->tamano++;
    h->datos[i].vertice   = vertice;
    h->datos[i].distancia = distancia;
    h->pos[vertice] = i;
    subir(h, i);
}

NodoHeap extraerMin(MinHeap *h) {
    NodoHeap raiz   = h->datos[0];
    NodoHeap ultimo = h->datos[--h->tamano];
    h->pos[raiz.vertice] = -1;
    if (h->tamano > 0) {
        h->datos[0] = ultimo;
        h->pos[ultimo.vertice] = 0;
        bajar(h, 0);
    }
    return raiz;
}

void disminuirClave(MinHeap *h, int vertice, int nuevaDist) {
    int i = h->pos[vertice];
    if (i < 0) {
        insertarHeap(h, vertice, nuevaDist);
        return;
    }
    if (nuevaDist < h->datos[i].distancia) {
        h->datos[i].distancia = nuevaDist;
        subir(h, i);
    }
}

bool estaVacioHeap(MinHeap *h) {
    return h->tamano == 0;
}

/* ============================================================
 *  DIJKSTRA
 * ------------------------------------------------------------
 *  Idea general (por si luego se me olvida):
 *   1. d[origen] = 0, todo lo demás = infinito.
 *   2. Meto el origen en la cola de prioridad.
 *   3. Saco el vértice con menor distancia conocida, lo marco
 *      como visitado y relajo todas sus aristas salientes.
 *   4. Repito hasta que la cola quede vacía.
 * ============================================================ */
void dijkstra(GrafoMatriz *g, int origen, int *d, int *pi) {
    int V = g->V;
    bool *visitado = (bool *)calloc(V, sizeof(bool));

    for (int i = 0; i < V; i++) {
        d[i]  = INF;
        pi[i] = -1;
    }
    d[origen] = 0;

    MinHeap *h = crearMinHeap(V);
    insertarHeap(h, origen, 0);

    while (!estaVacioHeap(h)) {
        NodoHeap actual = extraerMin(h);
        int u = actual.vertice;

        if (visitado[u]) continue;
        visitado[u] = true;

        for (int v = 0; v < V; v++) {
            if (g->matriz[u][v] != INF && u != v && !visitado[v]) {
                if (relajar(d, pi, u, v, g->matriz[u][v])) {
                    disminuirClave(h, v, d[v]);
                }
            }
        }
    }

    free(visitado);
    liberarMinHeap(h);
}

/* ============================================================
 *  Imprime el camino origen -> destino usando el arreglo pi.
 *  Es recursiva porque me pareció la forma más limpia.
 * ============================================================ */
void imprimirCamino(int *pi, int origen, int destino) {
    if (destino == origen) {
        printf("%d", origen);
        return;
    }
    if (pi[destino] == -1) {
        printf("(no hay forma de llegar)");
        return;
    }
    imprimirCamino(pi, origen, pi[destino]);
    printf(" -> %d", destino);
}

/* ============================================================
 *  BELLMAN-FORD
 * ------------------------------------------------------------
 *  Sirve cuando hay pesos negativos y además es el único que
 *  detecta ciclos negativos.
 *
 *  La idea: relajar TODAS las aristas V-1 veces. Con eso
 *  garantizo haber encontrado el óptimo (siempre que no haya
 *  ciclos negativos alcanzables). Después hago una pasada
 *  extra: si todavía puedo relajar algo, hay ciclo negativo.
 * ============================================================ */
bool bellmanFord(GrafoAristas *g, int origen, int *d, int *pi) {
    int V = g->V;

    for (int i = 0; i < V; i++) {
        d[i]  = INF;
        pi[i] = -1;
    }
    d[origen] = 0;

    /* --- V-1 pasadas --- */
    for (int i = 1; i <= V - 1; i++) {
        bool cambio = false;
        for (int j = 0; j < g->E; j++) {
            int u = g->aristas[j].u;
            int v = g->aristas[j].v;
            int w = g->aristas[j].peso;
            if (relajar(d, pi, u, v, w))
                cambio = true;
        }
        /* Si en una pasada no cambió nada, ya no va a cambiar.
           Corto antes para no perder tiempo. */
        if (!cambio) break;
    }

    /* --- Pasada de detección de ciclos negativos --- */
    for (int j = 0; j < g->E; j++) {
        int u = g->aristas[j].u;
        int v = g->aristas[j].v;
        int w = g->aristas[j].peso;
        if (d[u] != INF && d[v] > d[u] + w)
            return false;   /* encontré un ciclo negativo */
    }

    return true;   /* todo bien, no hay ciclos negativos */
}

void imprimirCaminoBF(int *pi, int origen, int destino) {
    /* Reutilizo la misma función de Dijkstra, total hace lo mismo. */
    imprimirCamino(pi, origen, destino);
}

/* ============================================================
 *  FLOYD-WARSHALL
 * ------------------------------------------------------------
 *  Caminos mínimos entre TODOS los pares.
 *  Es programación dinámica pura. La recurrencia es:
 *
 *     D[i][j] = min( D[i][j], D[i][k] + D[k][j] )
 *
 *  La idea es: "¿me conviene pasar por k entre i y j, o me
 *  quedo con lo que ya tenía?"
 *
 *  También funciona con pesos negativos (siempre que no haya
 *  ciclos negativos).
 * ============================================================ */
void floydWarshall(GrafoMatriz *g, int ***D_out, int ***P_out) {
    int V = g->V;

    /* Reservo las matrices D (distancias) y P (predecesores). */
    int **D = (int **)malloc(V * sizeof(int *));
    int **P = (int **)malloc(V * sizeof(int *));
    for (int i = 0; i < V; i++) {
        D[i] = (int *)malloc(V * sizeof(int));
        P[i] = (int *)malloc(V * sizeof(int));
        for (int j = 0; j < V; j++) {
            D[i][j] = g->matriz[i][j];
            /* El predecesor inicial: si hay arista i->j, entonces
               el predecesor de j pasando por i es i mismo. */
            if (i == j || g->matriz[i][j] == INF)
                P[i][j] = -1;
            else
                P[i][j] = i;
        }
    }

    /* El famoso triple bucle. Sí, es O(V^3), pero para grafos
       pequeños o medianos va perfecto. */
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (D[i][k] != INF && D[k][j] != INF &&
                    D[i][k] + D[k][j] < D[i][j]) {
                    D[i][j] = D[i][k] + D[k][j];
                    P[i][j] = P[k][j];
                }
            }
        }
    }

    *D_out = D;
    *P_out = P;
}

void imprimirMatrizFW(int **M, int V, const char *titulo) {
    printf("\n   %s\n\n", titulo);
    printf("        ");
    for (int j = 0; j < V; j++) printf("%6d ", j);
    printf("\n      ");
    for (int j = 0; j < V; j++) printf("-------");
    printf("\n");
    for (int i = 0; i < V; i++) {
        printf("  %3d |", i);
        for (int j = 0; j < V; j++) {
            if (M[i][j] == INF) printf("%6s ", "oo");
            else                printf("%6d ", M[i][j]);
        }
        printf("\n");
    }
}

/* Reconstruyo el camino recursivamente. Truco clásico. */
void reconstruirCaminoFW(int **P, int i, int j) {
    if (i == j) { printf("%d", i); return; }
    if (P[i][j] == -1) { printf("(no hay ruta)"); return; }
    reconstruirCaminoFW(P, i, P[i][j]);
    printf(" -> %d", j);
}

/* ============================================================
 *  GRAFOS DE EJEMPLO
 * ============================================================ */

/* Ejemplo 1: grafo de ciudades con pesos positivos.
   Este es el típico que sale en los libros (el de Dijkstra). */
GrafoMatriz *grafoCiudades(void) {
    GrafoMatriz *g = crearGrafoMatriz(6);
    agregarAristaMatriz(g, 0, 1,  7, true);
    agregarAristaMatriz(g, 0, 2,  9, true);
    agregarAristaMatriz(g, 0, 5, 14, true);
    agregarAristaMatriz(g, 1, 2, 10, true);
    agregarAristaMatriz(g, 1, 3, 15, true);
    agregarAristaMatriz(g, 2, 3, 11, true);
    agregarAristaMatriz(g, 2, 5,  2, true);
    agregarAristaMatriz(g, 3, 4,  6, true);
    agregarAristaMatriz(g, 4, 5,  9, true);
    return g;
}

/* Ejemplo 2: grafo con pesos negativos (pero SIN ciclos).
   Aquí Dijkstra fallaría, así que toca usar Bellman-Ford. */
GrafoAristas *grafoConNegativos(void) {
    int V = 5;
    GrafoAristas *g = crearGrafoAristas(V, 8);
    g->V = V;
    g->aristas[0] = (Arista){0, 1,  6};
    g->aristas[1] = (Arista){0, 2,  7};
    g->aristas[2] = (Arista){1, 3,  5};
    g->aristas[3] = (Arista){1, 2,  8};
    g->aristas[4] = (Arista){1, 4, -4};
    g->aristas[5] = (Arista){2, 3, -3};
    g->aristas[6] = (Arista){2, 4,  9};
    g->aristas[7] = (Arista){3, 1, -2};
    g->E = 8;
    return g;
}

/* Ejemplo 3: grafo CON ciclo negativo (a propósito).
   Sirve para que Bellman-Ford demuestre que lo detecta. */
GrafoAristas *grafoCicloNegativo(void) {
    int V = 3;
    GrafoAristas *g = crearGrafoAristas(V, 3);
    g->V = V;
    g->aristas[0] = (Arista){0, 1,  1};
    g->aristas[1] = (Arista){1, 2, -3};
    g->aristas[2] = (Arista){2, 0,  1};   /* cierra el ciclo: 1 - 3 + 1 = -1 */
    g->E = 3;
    return g;
}

/* ============================================================
 *  DEMO 1: relajación de aristas
 * ------------------------------------------------------------
 *  Quiero mostrar cómo cambia "d" paso a paso. Es lo más
 *  didáctico de todo el proyecto.
 * ============================================================ */
void demoRelajacion(void) {
    printf("\n");
    printf(" +--------------------------------------------------+\n");
    printf(" |   RELAJACION DE ARISTAS  (paso a paso)           |\n");
    printf(" +--------------------------------------------------+\n");

    int d[4]  = {0, INF, INF, INF};
    int pi[4] = {-1, -1, -1, -1};

    printf("\n  Empezamos: d = [0, oo, oo, oo]\n");
    printf("  (el origen es el nodo 0 y vale 0)\n");

    printf("\n  Relajamos la arista (0 -> 1) con peso 4...\n");
    relajar(d, pi, 0, 1, 4);
    printf("  Ahora d = [%d, %d, %s, %s]\n",
           d[0], d[1],
           d[2] == INF ? "oo" : "?",
           d[3] == INF ? "oo" : "?");

    printf("\n  Relajamos la arista (1 -> 2) con peso 3...\n");
    relajar(d, pi, 1, 2, 3);
    printf("  Ahora d = [%d, %d, %d, %s]\n",
           d[0], d[1], d[2],
           d[3] == INF ? "oo" : "?");

    printf("\n  Intentamos relajar (0 -> 2) con peso 10...\n");
    bool mejoro = relajar(d, pi, 0, 2, 10);
    printf("  %s (0 + 10 no es mejor que %d)\n",
           mejoro ? "MEJORO" : "no mejoro, se queda igual", d[2]);

    printf("\n  Relajamos la arista (2 -> 3) con peso 2...\n");
    relajar(d, pi, 2, 3, 2);
    printf("  Ahora d = [%d, %d, %d, %d]\n", d[0], d[1], d[2], d[3]);

    printf("\n  Listo. Asi funciona la relajacion.\n");
}

/* ============================================================
 *  DEMO 2: Dijkstra
 * ============================================================ */
void demoDijkstra(void) {
    printf("\n");
    printf(" +--------------------------------------------------+\n");
    printf(" |   DIJKSTRA  (pesos no negativos)                 |\n");
    printf(" +--------------------------------------------------+\n");

    GrafoMatriz *g = grafoCiudades();
    imprimirGrafoMatriz(g);

    int origen = 0;
    int *d  = (int *)malloc(g->V * sizeof(int));
    int *pi = (int *)malloc(g->V * sizeof(int));

    printf("\n  Calculando distancias desde el nodo %d...\n", origen);
    dijkstra(g, origen, d, pi);

    printf("\n  Resultados:\n");
    for (int i = 0; i < g->V; i++) {
        printf("    Nodo %d -> ", i);
        if (d[i] == INF) {
            printf("inalcanzable\n");
        } else {
            printf("costo %2d  |  camino: ", d[i]);
            imprimirCamino(pi, origen, i);
            printf("\n");
        }
    }

    free(d); free(pi);
    liberarGrafoMatriz(g);
}

/* ============================================================
 *  DEMO 3: Bellman-Ford sin ciclo negativo
 * ============================================================ */
void demoBellmanFord(void) {
    printf("\n");
    printf(" +--------------------------------------------------+\n");
    printf(" |   BELLMAN-FORD  (pesos negativos)                |\n");
    printf(" +--------------------------------------------------+\n");

    GrafoAristas *g = grafoConNegativos();
    int origen = 0;

    int *d  = (int *)malloc(g->V * sizeof(int));
    int *pi = (int *)malloc(g->V * sizeof(int));

    printf("\n  Este grafo tiene aristas con peso negativo.\n");
    printf("  Dijkstra aqui NO serviria. Vamos con Bellman-Ford...\n");

    bool ok = bellmanFord(g, origen, d, pi);

    if (!ok) {
        printf("\n  Se detecto un ciclo negativo alcanzable.\n");
    } else {
        printf("\n  Resultados desde el nodo %d:\n", origen);
        for (int i = 0; i < g->V; i++) {
            printf("    Nodo %d -> ", i);
            if (d[i] == INF) {
                printf("inalcanzable\n");
            } else {
                printf("costo %2d  |  camino: ", d[i]);
                imprimirCaminoBF(pi, origen, i);
                printf("\n");
            }
        }
    }

    free(d); free(pi);
    liberarGrafoAristas(g);
}

/* ============================================================
 *  DEMO 4: detección de ciclos negativos
 * ============================================================ */
void demoCicloNegativo(void) {
    printf("\n");
    printf(" +--------------------------------------------------+\n");
    printf(" |   DETECCION DE CICLOS NEGATIVOS                  |\n");
    printf(" +--------------------------------------------------+\n");

    GrafoAristas *g = grafoCicloNegativo();
    int origen = 0;

    int *d  = (int *)malloc(g->V * sizeof(int));
    int *pi = (int *)malloc(g->V * sizeof(int));

    printf("\n  Armo un grafo con un ciclo que suma -1 (a proposito).\n");
    printf("  Si Bellman-Ford funciona bien, tiene que avisarme.\n\n");

    bool ok = bellmanFord(g, origen, d, pi);

    if (!ok) {
        printf("  >>> ALERTA: hay un ciclo negativo alcanzable desde %d.\n", origen);
        printf("  >>> Las distancias NO estan bien definidas (pueden\n");
        printf("  >>> seguir bajando indefinidamente).\n");
    } else {
        printf("  No hay ciclos negativos. Todo tranquilo.\n");
    }

    free(d); free(pi);
    liberarGrafoAristas(g);
}

/* ============================================================
 *  DEMO 5: Floyd-Warshall
 * ============================================================ */
void demoFloydWarshall(void) {
    printf("\n");
    printf(" +--------------------------------------------------+\n");
    printf(" |   FLOYD-WARSHALL  (todos los pares)              |\n");
    printf(" +--------------------------------------------------+\n");

    GrafoMatriz *g = grafoCiudades();
    int V = g->V;

    int **D, **P;
    floydWarshall(g, &D, &P);

    imprimirMatrizFW(D, V, "Matriz de distancias minimas (D)");
    imprimirMatrizFW(P, V, "Matriz de predecesores (P)");

    printf("\n  Algunos caminos reconstruidos a partir de P:\n");
    int pares[][2] = { {0,4}, {0,5}, {1,4}, {2,3} };
    for (int k = 0; k < 4; k++) {
        int i = pares[k][0], j = pares[k][1];
        printf("    %d -> %d  (costo %2d):  ", i, j, D[i][j]);
        reconstruirCaminoFW(P, i, j);
        printf("\n");
    }

    printf("\n  Revision de ciclos negativos (diagonal de D):\n");
    bool hayCiclo = false;
    for (int i = 0; i < V; i++) {
        if (D[i][i] < 0) hayCiclo = true;
    }
    printf("    %s\n", hayCiclo ? ">>> SI hay ciclos negativos"
                                : "  No hay ciclos negativos, todo bien.");

    for (int i = 0; i < V; i++) { free(D[i]); free(P[i]); }
    free(D); free(P);
    liberarGrafoMatriz(g);
}

/* ============================================================
 *  MENÚ PRINCIPAL
 * ============================================================ */
int main(void) {
    int opcion;

    printf("\n");
    printf(" ####################################################\n");
    printf(" #                                                  #\n");
    printf(" #   CAMINOS MINIMOS EN GRAFOS                      #\n");
    printf(" #   Proyecto - Analisis de Algoritmos              #\n");
    printf(" #                                                  #\n");
    printf(" ####################################################\n");

    do {
        printf("\n");
        printf("  Que quieres ver?\n");
        printf("\n");
        printf("    1) Relajacion de aristas (paso a paso)\n");
        printf("    2) Dijkstra (pesos no negativos)\n");
        printf("    3) Bellman-Ford (con pesos negativos)\n");
        printf("    4) Bellman-Ford (detectar ciclo negativo)\n");
        printf("    5) Floyd-Warshall (todos los pares)\n");
        printf("    6) Correr TODAS las demos\n");
        printf("    0) Salir\n");
        printf("\n  Tu opcion: ");

        if (scanf("%d", &opcion) != 1) {
            opcion = -1;
            while (getchar() != '\n');
        }

        switch (opcion) {
            case 1: demoRelajacion();      break;
            case 2: demoDijkstra();        break;
            case 3: demoBellmanFord();     break;
            case 4: demoCicloNegativo();   break;
            case 5: demoFloydWarshall();   break;
            case 6:
                demoRelajacion();
                demoDijkstra();
                demoBellmanFord();
                demoCicloNegativo();
                demoFloydWarshall();
                break;
            case 0:
                printf("\n  Nos vemos. Suerte con el proyecto!\n\n");
                break;
            default:
                printf("\n  Esa opcion no existe, intenta otra vez.\n");
        }
    } while (opcion != 0);

    return 0;
}
