#include <stdio.h>

// Función para combinar dos subarreglos ordenados
void merge(int arreglo[], int inicio, int medio, int fin) {
    int i = inicio;
    int j = medio + 1;
    int k = 0;

    int temporal[fin - inicio + 1];

    // Comparar elementos de ambas partes
    while (i <= medio && j <= fin) {
        if (arreglo[i] <= arreglo[j]) {
            temporal[k] = arreglo[i];
            i++;
        } else {
            temporal[k] = arreglo[j];
            j++;
        }
        k++;
    }

    // Copiar elementos restantes de la primera mitad
    while (i <= medio) {
        temporal[k] = arreglo[i];
        i++;
        k++;
    }

    // Copiar elementos restantes de la segunda mitad
    while (j <= fin) {
        temporal[k] = arreglo[j];
        j++;
        k++;
    }

    // Copiar los elementos ordenados al arreglo original
    for (i = inicio, k = 0; i <= fin; i++, k++) {
        arreglo[i] = temporal[k];
    }
}

// Función principal de Merge Sort
void mergeSort(int arreglo[], int inicio, int fin) {
    if (inicio < fin) {
        int medio = inicio + (fin - inicio) / 2;

        // Dividir
        mergeSort(arreglo, inicio, medio);
        mergeSort(arreglo, medio + 1, fin);

        // Combinar
        merge(arreglo, inicio, medio, fin);
    }
}

int main() {
    int n;

    printf("=== MERGE SORT ===\n");

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
    mergeSort(arreglo, 0, n - 1);

    printf("\n\nArreglo ordenado mediante Merge Sort:\n");

    for (int i = 0; i < n; i++) {
        printf("%d ", arreglo[i]);
    }

    printf("\n");

    return 0;
}