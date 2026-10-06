#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 200
#define TAM 256

struct Nodo {
    char caracter;
    int frecuencia;
    struct Nodo *izq;
    struct Nodo *der;
};

struct Nodo* crearNodo(char caracter, int frecuencia) {
    struct Nodo *nuevo;

    nuevo = (struct Nodo*)malloc(sizeof(struct Nodo));

    nuevo->caracter = caracter;
    nuevo->frecuencia = frecuencia;
    nuevo->izq = NULL;
    nuevo->der = NULL;

    return nuevo;
}

void intercambiar(struct Nodo *heap[], int a, int b) {
    struct Nodo *temp;

    temp = heap[a];
    heap[a] = heap[b];
    heap[b] = temp;
}

/* Mantener la propiedad del Min-Heap hacia arriba */
void subir(struct Nodo *heap[], int pos) {
    int padre;

    while (pos > 0) {

        padre = (pos - 1) / 2;

        if (heap[padre]->frecuencia <= heap[pos]->frecuencia)
            break;

        intercambiar(heap, padre, pos);

        pos = padre;
    }
}

/* Mantener la propiedad del Min-Heap hacia abajo */
void bajar(struct Nodo *heap[], int n, int pos) {
    int menor;
    int izquierdo;
    int derecho;

    while (1) {

        izquierdo = 2 * pos + 1;
        derecho = 2 * pos + 2;

        menor = pos;

        if (izquierdo < n &&
            heap[izquierdo]->frecuencia < heap[menor]->frecuencia) {

            menor = izquierdo;
        }

        if (derecho < n &&
            heap[derecho]->frecuencia < heap[menor]->frecuencia) {

            menor = derecho;
        }

        if (menor == pos)
            break;

        intercambiar(heap, pos, menor);

        pos = menor;
    }
}

/* Insertar un nodo en el Min-Heap */
void insertar(struct Nodo *heap[], int *n, struct Nodo *nuevo) {

    heap[*n] = nuevo;

    subir(heap, *n);

    (*n)++;
}

/* Extraer el nodo con menor frecuencia */
struct Nodo* extraerMin(struct Nodo *heap[], int *n) {

    struct Nodo *minimo;

    minimo = heap[0];

    (*n)--;

    if (*n > 0) {
        heap[0] = heap[*n];
        bajar(heap, *n, 0);
    }

    return minimo;
}

/* Generar codigo de Huffman */
void generarCodigo(struct Nodo *raiz, char codigo[], int posicion) {

    if (raiz == NULL)
        return;

    if (raiz->izq == NULL && raiz->der == NULL) {

        codigo[posicion] = '\0';

        if (raiz->caracter == ' ')
            printf("' ' = %s\n", codigo);
        else
            printf("%c = %s\n", raiz->caracter, codigo);

        return;
    }

    /* Izquierda = 0 */
    codigo[posicion] = '0';
    generarCodigo(raiz->izq, codigo, posicion + 1);

    /* Derecha = 1 */
    codigo[posicion] = '1';
    generarCodigo(raiz->der, codigo, posicion + 1);
}

int main() {

    char texto[MAX];
    char codigo[MAX];

    int frecuencia[TAM] = {0};

    struct Nodo *heap[TAM];
    struct Nodo *izq;
    struct Nodo *der;
    struct Nodo *padre;
    struct Nodo *raiz;

    int n = 0;
    int i;

    printf("Ingrese el texto: ");
    fgets(texto, MAX, stdin);

    texto[strcspn(texto, "\n")] = '\0';

    /* Contar frecuencia de cada caracter */
    for (i = 0; texto[i] != '\0'; i++) {
        frecuencia[(unsigned char)texto[i]]++;
    }

    /*
       CREAR EL MIN-HEAP

       Cada caracter con frecuencia mayor que cero
       se inserta en el Min-Heap.
    */
    for (i = 0; i < TAM; i++) {

        if (frecuencia[i] > 0) {

            insertar(
                heap,
                &n,
                crearNodo((char)i, frecuencia[i])
            );
        }
    }

    if (n == 0) {
        printf("No se ingreso ningun texto.\n");
        return 0;
    }

    /* Caso de un solo caracter */
    if (n == 1) {

        printf("\nCodigo de Huffman:\n");
        printf("%c = 0\n", heap[0]->caracter);

        return 0;
    }

    /*
       GREEDY DE HUFFMAN

       Siempre selecciona los dos nodos
       con menor frecuencia.
    */
    while (n > 1) {

        izq = extraerMin(heap, &n);
        der = extraerMin(heap, &n);

        padre = crearNodo(
            '*',
            izq->frecuencia + der->frecuencia
        );

        padre->izq = izq;
        padre->der = der;

        insertar(heap, &n, padre);
    }

    raiz = heap[0];

    printf("\nCODIGOS DE HUFFMAN\n");
    printf("------------------\n");

    generarCodigo(raiz, codigo, 0);

    return 0;
}
