#include <stdio.h>

// Función de Búsqueda Ternaria
int busquedaTernaria(int arreglo[], int n, int elemento) {

    int izquierda = 0;
    int derecha = n - 1;

    while (izquierda <= derecha) {

        // Primer punto intermedio
        int tercio1 = izquierda + (derecha - izquierda) / 3;

        // Segundo punto intermedio
        int tercio2 = derecha - (derecha - izquierda) / 3;

        // Comprobar el primer punto
        if (arreglo[tercio1] == elemento) {
            return tercio1;
        }

        // Comprobar el segundo punto
        if (arreglo[tercio2] == elemento) {
            return tercio2;
        }

        // El elemento está en la primera parte
        if (elemento < arreglo[tercio1]) {

            derecha = tercio1 - 1;
        }

        // El elemento está en la tercera parte
        else if (elemento > arreglo[tercio2]) {

            izquierda = tercio2 + 1;
        }

        // El elemento está en la parte central
        else {

            izquierda = tercio1 + 1;
            derecha = tercio2 - 1;
        }
    }

    // Elemento no encontrado
    return -1;
}

int main() {

    int n;
    int elemento;

    printf("=== BUSQUEDA TERNARIA ===\n");

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
    int posicion = busquedaTernaria(arreglo, n, elemento);

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