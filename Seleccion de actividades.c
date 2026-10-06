#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int inicio;
    int fin;
} Actividad;

// Comparador para qsort: ordena por tiempo de finalización (fin)
int compararActividades(const void *a, const void *b) {
    Actividad *actA = (Actividad *)a;
    Actividad *actB = (Actividad *)b;
    if (actA->fin < actB->fin) return -1;
    if (actA->fin > actB->fin) return 1;
    return 0;
}

void seleccionActividades(Actividad actividades[], int n) {
    qsort(actividades, n, sizeof(Actividad), compararActividades);

    printf("\n=========================================\n");
    printf("     ACTIVIDADES SELECCIONADAS\n");
    printf("=========================================\n");
    printf("  Inicio |   Fin\n");
    printf("---------+--------\n");

    int ultimoFin = actividades[0].fin;
    printf("   %4d  |  %4d\n", actividades[0].inicio, actividades[0].fin);
    int contador = 1;
	int i;
    for (i = 1; i < n; i++) {
        if (actividades[i].inicio >= ultimoFin) {
            printf("   %4d  |  %4d\n", actividades[i].inicio, actividades[i].fin);
            ultimoFin = actividades[i].fin;
            contador++;
        }
    }

    printf("=========================================\n");
    printf("  Total de actividades: %d\n", contador);
    printf("=========================================\n");
}

int main() {
    int n;
	int i;
    printf("=========================================\n");
    printf("   SELECCION DE ACTIVIDADES\n");
    printf("=========================================\n");
    printf("Ingrese la cantidad de actividades: ");
    scanf("%d", &n);

    if (n <= 0) {
        printf("No hay actividades para seleccionar.\n");
        return 0;
    }

    Actividad *actividades = (Actividad *)malloc(n * sizeof(Actividad));
    if (actividades == NULL) {
        printf("Error: no se pudo reservar memoria.\n");
        return 1;
    }

    printf("\nIngrese el inicio y fin de cada actividad:\n");
    printf("(Los tiempos deben ser mayores o iguales a 0 y el inicio <= fin)\n\n");

    for (i= 0; i < n; i++) {
        int valido = 0;
        while (!valido) {
            printf("Actividad %d\n", i + 1);
            printf("   Inicio: ");
            scanf("%d", &actividades[i].inicio);
            printf("   Fin:    ");
            scanf("%d", &actividades[i].fin);

            if (actividades[i].inicio < 0 || actividades[i].fin < 0) {
                printf("   Error: los tiempos no pueden ser negativos.\n");
                printf("   Intente de nuevo.\n\n");
            } else if (actividades[i].inicio > actividades[i].fin) {
                printf("   Error: el inicio (%d) no puede ser mayor que el fin (%d).\n",
                       actividades[i].inicio, actividades[i].fin);
                printf("   Intente de nuevo.\n\n");
            } else {
                valido = 1;
                printf("\n");
            }
        }
    }

    seleccionActividades(actividades, n);

    free(actividades);
    return 0;
}
