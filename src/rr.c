#include <stdio.h>

void trocar(int v[], int a, int b) {
    int temp = v[a];
    v[a] = v[b];
    v[b] = temp;
}

int main() {
    int id[50], chegada[50], burst[50], restante[50];
    int fila[50];
    int inicio = 0, fim = 0, quantidade = 0;
    int n, quantum, i, j, atual, exec;
    int tempo = 0, prox = 0, concluidos = 0;

    printf("Quantidade de processos: ");
    if (scanf("%d", &n) != 1 || n < 1 || n > 50) {
        printf("Quantidade invalida!\n");
        return 1;
    }

    printf("Quantum: ");
    if (scanf("%d", &quantum) != 1 || quantum < 1) {
        printf("Quantum invalido!\n");
        return 1;
    }

    for (i = 0; i < n; i++) {
        printf("Digite a chegada e o burst do P%d: ", i + 1);
        if (scanf("%d %d", &chegada[i], &burst[i]) != 2 ||
            chegada[i] < 0 || burst[i] < 1) {
            printf("Dados invalidos! Chegada >= 0 e burst >= 1.\n");
            return 1;
        }
        id[i] = i + 1;
        restante[i] = burst[i];
    }

    for (i = 0; i < n - 1; i++) {
        for (j = i + 1; j < n; j++) {
            if (chegada[i] > chegada[j] ||
                (chegada[i] == chegada[j] && id[i] > id[j])) {
                trocar(id, i, j);
                trocar(chegada, i, j);
                trocar(burst, i, j);
                trocar(restante, i, j);
            }
        }
    }

    printf("\nProcesso  Chegada  Burst\n");
    for (i = 0; i < n; i++) {
        printf("P%d        %d        %d\n", id[i], chegada[i], burst[i]);
    }

    printf("\nOrdem de execucao (quantum = %d):\n", quantum);

    while (concluidos < n) {

        while (prox < n && chegada[prox] <= tempo) {
            fila[fim] = prox;
            fim = (fim + 1) % 50;
            quantidade++;
            prox++;
        }

        if (quantidade == 0) {
            printf("CPU ociosa de %d a %d\n", tempo, chegada[prox]);
            tempo = chegada[prox];
            continue;
        }

        atual = fila[inicio];
        inicio = (inicio + 1) % 50;
        quantidade--;

        exec = quantum;
        if (restante[atual] < quantum) {
            exec = restante[atual];
        }

        printf("P%d executa de %d a %d\n", id[atual], tempo, tempo + exec);
        tempo += exec;
        restante[atual] -= exec;

        while (prox < n && chegada[prox] <= tempo) {
            fila[fim] = prox;
            fim = (fim + 1) % 50;
            quantidade++;
            prox++;
        }

        if (restante[atual] > 0) {
            fila[fim] = atual;
            fim = (fim + 1) % 50;
            quantidade++;
        } else {
            concluidos++;
            printf("P%d terminou no tempo %d\n", id[atual], tempo);
        }
    }

    return 0;
}