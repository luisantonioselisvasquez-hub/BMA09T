#include <stdio.h>
#include <string.h>

#define MAX_TEXTO 500
#define MAX_PATRON 100

void construirLPS(char patron[], int m, int lps[]) {
    int longitud = 0;
    int i = 1;

    lps[0] = 0;

    while (i < m) {

        if (patron[i] == patron[longitud]) {
            longitud++;
            lps[i] = longitud;
            i++;
        }
        else {
            if (longitud != 0) {
                longitud = lps[longitud - 1];
            }
            else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

void KMP(char texto[], char patron[]) {

    int n = strlen(texto);
    int m = strlen(patron);

    int lps[MAX_PATRON];

    int i = 0;
    int j = 0;
    int encontrado = 0;
    int k;

    construirLPS(patron, m, lps);

    printf("\nTabla LPS:\n");

    for (k = 0; k < m; k++) {
        printf("%d ", lps[k]);
    }

    printf("\n\nPosiciones donde aparece el patron:\n");

    while (i < n) {

        if (texto[i] == patron[j]) {
            i++;
            j++;
        }

        if (j == m) {

            printf("%d ", i - j);

            encontrado = 1;

            j = lps[j - 1];
        }

        else if (i < n && texto[i] != patron[j]) {

            if (j != 0) {
                j = lps[j - 1];
            }
            else {
                i++;
            }
        }
    }

    if (encontrado == 0) {
        printf("No se encontro el patron.");
    }

    printf("\n");
}

int main() {

    char texto[MAX_TEXTO];
    char patron[MAX_PATRON];

    printf("Ingrese el texto:\n");
    fgets(texto, MAX_TEXTO, stdin);

    printf("Ingrese el patron que desea buscar:\n");
    fgets(patron, MAX_PATRON, stdin);

    texto[strcspn(texto, "\n")] = '\0';
    patron[strcspn(patron, "\n")] = '\0';

    KMP(texto, patron);

    return 0;
}
