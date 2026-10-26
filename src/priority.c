#include <stdio.h>

int main() {
    int n, i, j, aux;
    int processo[100];
    int tempo[100];
    int prioridade[100];
    int espera[100];
    int retorno[100];

    printf("Quantidade de processos: ");
    scanf("%d", &n);

    if (n < 1 || n > 100) {
        printf("Quantidade invalida! Digite de 1 a 100.\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        processo[i] = i + 1;

        printf("\nTempo de execucao do P%d: ", processo[i]);
        scanf("%d", &tempo[i]);

        printf("Prioridade do P%d (menor numero = maior prioridade): ",
               processo[i]);
        scanf("%d", &prioridade[i]);

        if (tempo[i] <= 0 || prioridade[i] < 1) {
            printf("Tempo ou prioridade invalida!\n");
            return 1;
        }
    }

    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (prioridade[i] > prioridade[j]) {
                aux = prioridade[i];
                prioridade[i] = prioridade[j];
                prioridade[j] = aux;

                aux = tempo[i];
                tempo[i] = tempo[j];
                tempo[j] = aux;

                aux = processo[i];
                processo[i] = processo[j];
                processo[j] = aux;
            }
        }
    }

    espera[0] = 0;
    retorno[0] = tempo[0];

    for (i = 1; i < n; i++) {
        espera[i] = espera[i - 1] + tempo[i - 1];
        retorno[i] = espera[i] + tempo[i];
    }

    printf("\n--- Escalonamento por Prioridade ---\n");
    printf("Processo\tPrioridade\tTempo\tEspera\tRetorno\n");

    for (i = 0; i < n; i++) {
        printf("P%d\t\t%d\t\t%d\t%d\t%d\n",
               processo[i],
               prioridade[i],
               tempo[i],
               espera[i],
               retorno[i]);
    }

    return 0;
}


