#include <stdio.h>
#include <string.h>

#define BASE 256
#define MOD 101

void rabinKarp(char texto[], char patron[]) {

    int n = strlen(texto);
    int m = strlen(patron);

    int hashPatron = 0;
    int hashTexto = 0;
    int h = 1;
    int encontrado = 0;

    if (m > n) {
        printf("El patron es mas largo que el texto.\n");
        return;
    }
	int i,j;
    // Calculamos BASE^(m-1) modulo MOD
    for (i = 0; i < m - 1; i++) {
        h = (h * BASE) % MOD;
    }

    // Hash inicial del patron y de la primera ventana
    for (i = 0; i < m; i++) {
        hashPatron = (BASE * hashPatron + patron[i]) % MOD;
        hashTexto = (BASE * hashTexto + texto[i]) % MOD;
    }

    printf("\nBuscando el patron...\n");

    for (i = 0; i <= n - m; i++) {

        if (hashPatron == hashTexto) {

            int j;

            for (j = 0; j < m; j++) {
                if (texto[i + j] != patron[j]) {
                    break;
                }
            }

            if (j == m) {
                printf("Patron encontrado en la posicion: %d\n", i);
                encontrado++;
            }
        }

        // Calculamos el hash de la siguiente ventana
        if (i < n - m) {

            hashTexto =
                (BASE * (hashTexto - texto[i] * h)
                + texto[i + m]) % MOD;

            if (hashTexto < 0) {
                hashTexto += MOD;
            }
        }
    }

    if (encontrado == 0) {
        printf("El patron no fue encontrado.\n");
    } else {
        printf("Cantidad de apariciones: %d\n", encontrado);
    }
}

int main() {

    char texto[500];
    char patron[100];

    printf("Ingrese el texto: ");
    fgets(texto, sizeof(texto), stdin);

    printf("Ingrese el patron: ");
    fgets(patron, sizeof(patron), stdin);

    texto[strcspn(texto, "\n")] = '\0';
    patron[strcspn(patron, "\n")] = '\0';

    rabinKarp(texto, patron);

    return 0;
}
