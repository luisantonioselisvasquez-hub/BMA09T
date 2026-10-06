#include <stdio.h>

// Función de búsqueda binaria
int busquedaBinaria(int arreglo[], int n, int elemento) {

    int inicio = 0;
    int fin = n - 1;

    while (inicio <= fin) {

        // Calcular la posición central
        int medio = inicio + (fin - inicio) / 2;

        // Si encontramos el elemento
        if (arreglo[medio] == elemento) {
            return medio;
        }

        // Si el elemento buscado es mayor,
        // descartamos la mitad izquierda
        if (arreglo[medio] < elemento) {
            inicio = medio + 1;
        }

        // Si el elemento buscado es menor,
        // descartamos la mitad derecha
        else {
            fin = medio - 1;
        }
    }

    // No encontrado
    return -1;
}

int main() {

    int n;
    int elemento;

    printf("=== BUSQUEDA BINARIA ===\n");

    printf("Ingrese la cantidad de elementos: ");
    scanf("%d", &n);

    int arreglo[n];

    printf("Ingrese los %d elementos en orden ascendente:\n", n);

    for (int i = 0; i < n; i++) {
        scanf("%d", &arreglo[i]);
    }

    printf("\nIngrese el elemento que desea buscar: ");
    scanf("%d", &elemento);

    // Realizar búsqueda
    int posicion = busquedaBinaria(arreglo, n, elemento);

    // Mostrar resultado
    if (posicion != -1) {
        printf("\nElemento encontrado.\n");
        printf("Valor: %d\n", elemento);
        printf("Posicion: %d\n", posicion);
        printf("Posicion humana: %d\n", posicion + 1);
    } else {
        printf("\nElemento no encontrado.\n");
    }

    return 0;
}