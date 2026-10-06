#include <stdio.h>
#include <string.h>

void fuerzaBruta(char texto[], char patron[]) {
    int n = strlen(texto);
    int m = strlen(patron);
    int encontrado = 0;

    printf("\nPosiciones donde aparece el patron:\n");
    
	int i,j;
	
    for ( i = 0; i <= n - m; i++) {
        int j;

        for (j = 0; j < m; j++) {
            if (texto[i + j] != patron[j]) {
                break;
            }
        }

        if (j == m) {
            printf("Posicion: %d\n", i);
            encontrado++;
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

    fuerzaBruta(texto, patron);

    return 0;
}
