#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;
    double valor;
    double peso;
    double relacion;
    double fraccion;
} Objeto;

// Comparador para qsort: ordena por relación valor/peso de mayor a menor
int compararObjetos(const void *a, const void *b) {
    Objeto *objA = (Objeto *)a;
    Objeto *objB = (Objeto *)b;
    if (objA->relacion > objB->relacion) return -1;
    if (objA->relacion < objB->relacion) return 1;
    return 0;
}

// Imprime la tabla de resultados
void imprimirResultado(Objeto objetos[], int n, double valorTotal) {
    printf("\n=========================================\n");
    printf("     MOCHILA FRACCIONARIA - RESULTADO\n");
    printf("=========================================\n");
    printf("  ID  | Relacion |  Peso  |  Valor  | Fraccion | Peso en mochila\n");
    printf("------+----------+--------+---------+----------+----------------\n");
	int i;
    for (i = 0; i < n; i++) {
        if (objetos[i].fraccion > 0) {
            printf("  %3d |  %6.2f  | %6.2f | %7.2f |  %5.2f   |     %6.2f\n",
                   objetos[i].id,
                   objetos[i].relacion,
                   objetos[i].peso,
                   objetos[i].valor,
                   objetos[i].fraccion,
                   objetos[i].peso * objetos[i].fraccion);
        }
    }

    printf("=========================================\n");
    printf("  VALOR TOTAL LOGRADO: %.2f\n", valorTotal);
    printf("=========================================\n");
}

int main() {
    int i,n;
    double capacidad;

    printf("=========================================\n");
    printf("   PROBLEMA DE LA MOCHILA FRACCIONARIA\n");
    printf("=========================================\n");

    // Validar capacidad
    do {
        printf("Ingrese la capacidad de la mochila: ");
        scanf("%lf", &capacidad);
        if (capacidad <= 0) {
            printf("Error: la capacidad debe ser mayor que 0.\n");
        }
    } while (capacidad <= 0);

    // Validar cantidad de objetos
    do {
        printf("Ingrese la cantidad de objetos: ");
        scanf("%d", &n);
        if (n <= 0) {
            printf("Error: debe haber al menos 1 objeto.\n");
        }
    } while (n <= 0);

    Objeto *objetos = (Objeto *)malloc(n * sizeof(Objeto));
    if (objetos == NULL) {
        printf("Error: no se pudo reservar memoria.\n");
        return 1;
    }

    double sumaPesos = 0.0;

    // Leer objetos con validación
    for (i = 0; i < n; i++) {
        objetos[i].id = i + 1;

        do {
            printf("\nObjeto %d - Valor: ", i + 1);
            scanf("%lf", &objetos[i].valor);
            if (objetos[i].valor < 0) {
                printf("Error: el valor no puede ser negativo.\n");
            }
        } while (objetos[i].valor < 0);

        do {
            printf("Objeto %d - Peso: ", i + 1);
            scanf("%lf", &objetos[i].peso);
            if (objetos[i].peso <= 0) {
                printf("Error: el peso debe ser mayor que 0.\n");
            }
        } while (objetos[i].peso <= 0);

        objetos[i].relacion = objetos[i].valor / objetos[i].peso;
        objetos[i].fraccion = 0.0;
        sumaPesos += objetos[i].peso;
    }

    // Si todo cabe, tomar todo completo
    if (sumaPesos <= capacidad) {
        double valorTotal = 0.0;
        for (i = 0; i < n; i++) {
            objetos[i].fraccion = 1.0;
            valorTotal += objetos[i].valor;
        }
        printf("\nTodos los objetos caben en la mochila.\n");
        imprimirResultado(objetos, n, valorTotal);
        free(objetos);
        return 0;
    }

    // Ordenar por relación valor/peso descendente
    qsort(objetos, n, sizeof(Objeto), compararObjetos);

    double pesoActual = 0.0;
    double valorTotal = 0.0;

    // Llenar la mochila
    for (i= 0; i < n; i++) {
        if (pesoActual + objetos[i].peso <= capacidad) {
            // Cabe completo
            objetos[i].fraccion = 1.0;
            pesoActual += objetos[i].peso;
            valorTotal += objetos[i].valor;
        } else {
            // Tomar fracción
            double espacioRestante = capacidad - pesoActual;
            objetos[i].fraccion = espacioRestante / objetos[i].peso;
            valorTotal += objetos[i].valor * objetos[i].fraccion;
            pesoActual = capacidad;
            break;
        }
    }

    imprimirResultado(objetos, n, valorTotal);

    free(objetos);
    return 0;
}
