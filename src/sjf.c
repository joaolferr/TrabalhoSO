#include <stdio.h>

int main() {
    int processo[5] = {1, 2, 3, 4, 5};
    int duracao[5];
    int espera[5];
    int retorno[5];
    int i, j, auxiliar;

    printf("=== ESCALONAMENTO SJF ===\n");

    for (i = 0; i < 5; i++) {
        printf("Digite a duracao do P%d: ", processo[i]);
        scanf("%d", &duracao[i]);

        if (duracao[i] <= 0) {
            printf("Duracao invalida!\n");
            return 1;
        }
    }

    for (i = 0; i < 5; i++) {
        for (j = i + 1; j < 5; j++) {
            if (duracao[j] < duracao[i]) {
                auxiliar = duracao[i];
                duracao[i] = duracao[j];
                duracao[j] = auxiliar;

                auxiliar = processo[i];
                processo[i] = processo[j];
                processo[j] = auxiliar;
            }
        }
    }

    espera[0] = 0;

    for (i = 1; i < 5; i++) {
        espera[i] = espera[i - 1] + duracao[i - 1];
    }

    for (i = 0; i < 5; i++) {
        retorno[i] = espera[i] + duracao[i];
    }

    printf("\nProcesso  Duracao  Espera  Retorno\n");

    for (i = 0; i < 5; i++) {
        printf("P%d        %d        %d       %d\n",
               processo[i], duracao[i], espera[i], retorno[i]);
    }

    printf("\nOrdem de execucao: ");

    for (i = 0; i < 5; i++) {
        printf("P%d ", processo[i]);
    }

    printf("\n");

    return 0;
}