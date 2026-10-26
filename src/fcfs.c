#include <stdio.h>

#define MAX 20

// Estrutura para armazenar os dados de cada processo
typedef struct {
    int id;
    int tempo_chegada;
    int burst_cpu;
    int tempo_espera;
    int turnaround;
} Processo;

// Implementacao de uma fila estatica para o FIFO
typedef struct {
    Processo dados[MAX];
    int frente;
    int tras;
} FilaFIFO;

// Funcao para inserir um novo processo no fim da fila
void enfileirar(FilaFIFO* f, Processo p) {
    if (f->tras < MAX) {
        f->dados[f->tras] = p;
        f->tras++;
    }
}

int main() {
    int n, relogio = 0;
    float soma_espera = 0, soma_total = 0;

    FilaFIFO fila;
    fila.frente = 0;
    fila.tras = 0;

    printf("============================================================\n");
    printf("        SIMULADOR DE ESCALONAMENTO DE PROCESSOS (FIFO)      \n");
    printf("============================================================\n\n");
    
    printf("Digite a quantidade de processos (Max 20): ");
    // Valida o limite do vetor para evitar estouro de memoria
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX) {
        printf("\n[ERRO]: Quantidade inválida de processos! Digite um valor entre 1 e %d.\n", MAX);
        return 1;
    }
    
    // Leitura dos dados de entrada
    for (int i = 0; i < n; i++) {
        Processo p;
        p.id = i + 1;
        printf("\n--- DADOS DO PROCESSO P%d ---\n", p.id);
        
        printf("  Tempo de Chegada (minuto): ");
        scanf("%d", &p.tempo_chegada);
        
        printf("  Burst de CPU / Tempo de Execucao (minutos): ");
        scanf("%d", &p.burst_cpu);
        
        enfileirar(&fila, p);
    }

    printf("\n======================= REPORT FINAL =======================\n");
    printf("%-8s %-10s %-15s %-10s %-10s\n", "PROCESSO", "CHEGADA", "BURST DE CPU", "ESPERA", "TURNAROUND");
    printf("------------------------------------------------------------\n");

    // Loop de execucao do escalonador FIFO
    while (fila.frente < fila.tras) {
        Processo p = fila.dados[fila.frente];

        // Avanca o relogio caso a CPU esteja ociosa aguardando o processo chegar
        if (relogio < p.tempo_chegada) {
            printf("[CPU Ociosa aguardando ate o minuto %d]\n", p.tempo_chegada);
            relogio = p.tempo_chegada;
        }

        // Calculo das metricas do processo atual
        p.tempo_espera = relogio - p.tempo_chegada;
        p.turnaround = p.tempo_espera + p.burst_cpu;
        
        // Atualiza o relogio global com o tempo de execucao do processo
        relogio += p.burst_cpu;

        // Acumula os valores para o calculo das medias finais
        soma_espera += p.tempo_espera;
        soma_total += p.turnaround;

        printf("P%-6d %-3d min   %-3d min         %-3d min   %-3d min\n", 
            p.id, p.tempo_chegada, p.burst_cpu, p.tempo_espera, p.turnaround);
            
        // Atualiza o ponteiro da frente (simula o dequeue)
        fila.frente++;
    }

    printf("============================================================\n");
    printf("METRICAS DE DESEMPENHO:\n");
    printf("  > Tempo Medio de Espera na Fila: %.2f minutos\n", soma_espera / n);
    printf("  > Tempo Medio de Turnaround (Total): %.2f minutos\n", soma_total / n);
    printf("============================================================\n");

    return 0;
}
