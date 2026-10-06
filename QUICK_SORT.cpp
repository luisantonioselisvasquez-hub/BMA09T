#include <stdio.h>

// Función para intercambiar dos elementos
void intercambiar(int *a, int *b) {
    int temporal = *a;
    *a = *b;
    *b = temporal;
}

// Función de partición
int particion(int arreglo[], int inicio, int fin) {
    int pivote = arreglo[fin];

    int i = inicio - 1;

    for (int j = inicio; j < fin; j++) {

        if (arreglo[j] < pivote) {
            i++;
            intercambiar(&arreglo[i], &arreglo[j]);
        }
    }

    // Colocar el pivote en su posición correcta
    intercambiar(&arreglo[i + 1], &arreglo[fin]);

    return i + 1;
}

// Función principal de Quick Sort
void quickSort(int arreglo[], int inicio, int fin) {

    if (inicio < fin) {

        // Obtener posición del pivote
        int posicionPivote = particion(arreglo, inicio, fin);

        // Ordenar la parte izquierda
        quickSort(arreglo, inicio, posicionPivote - 1);

        // Ordenar la parte derecha
        quickSort(arreglo, posicionPivote + 1, fin);
    }
}

int main() {
    int n;

    printf("=== QUICK SORT ===\n");

    printf("Ingrese la cantidad de elementos: ");
    scanf("%d", &n);

    int arreglo[n];

    printf("Ingrese los %d elementos:\n", n);

    for (int i = 0; i < n; i++) {
        scanf("%d", &arreglo[i]);
    }

    printf("\nArreglo original:\n");

    for (int i = 0; i < n; i++) {
        printf("%d ", arreglo[i]);
    }

    // Ordenar
    quickSort(arreglo, 0, n - 1);

    printf("\n\nArreglo ordenado mediante Quick Sort:\n");

    for (int i = 0; i < n; i++) {
        printf("%d ", arreglo[i]);
    }

    printf("\n");

    return 0;
}