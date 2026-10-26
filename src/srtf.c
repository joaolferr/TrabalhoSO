
#include <stdio.h>
#include <stdlib.h>

#define MAX_P 100
#define MAX_SEG 10000

/* ---------- Processo (PCB simplificado) ---------- */
typedef struct {
    int pid;        
    int chegada;     
    int burst;       
    int restante;    
    int inicio;      
    int termino;     
} Processo;

/* ---------- Fila de prontos (lista encadeada) ---------- */
typedef struct No {
    Processo *proc;
    struct No *prox;
} No;

typedef struct {
    No *inicio;
    No *fim;
} Fila;

void fila_inicializar(Fila *f) {
    f->inicio = NULL;
    f->fim = NULL;
}

int fila_vazia(Fila *f) {
    return f->inicio == NULL;
}

/* Insere no fim da fila */
void inserir(Fila *f, Processo *p) {
    No *novo = (No *)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro de memoria.\n");
        exit(1);
    }
    novo->proc = p;
    novo->prox = NULL;
    if (fila_vazia(f))
        f->inicio = novo;
    else
        f->fim->prox = novo;
    f->fim = novo;
}

/* Retorna 1 se 'a' deve ser escolhido antes de 'b'.
 * Critério: menor restante; empate -> chegou antes; empate -> menor pid. */
int melhor(Processo *a, Processo *b) {
    if (a->restante != b->restante) return a->restante < b->restante;
    if (a->chegada != b->chegada)   return a->chegada < b->chegada;
    return a->pid < b->pid;
}

/* Remove da fila e devolve o processo de MENOR tempo restante */
Processo *remover_menor_restante(Fila *f) {
    if (fila_vazia(f)) return NULL;

    No *menor = f->inicio, *menor_ant = NULL;
    No *atual = f->inicio, *ant = NULL;

    while (atual != NULL) {
        if (melhor(atual->proc, menor->proc)) {
            menor = atual;
            menor_ant = ant;
        }
        ant = atual;
        atual = atual->prox;
    }

    /* desliga o nó 'menor' da lista */
    if (menor_ant == NULL) f->inicio = menor->prox;
    else                   menor_ant->prox = menor->prox;
    if (menor == f->fim)   f->fim = menor_ant;

    Processo *p = menor->proc;
    free(menor);
    return p;
}

/* Mostra o conteúdo da fila: [P2(3) P4(5)] = pid(restante) */
void imprimir_fila(Fila *f) {
    printf("[");
    for (No *n = f->inicio; n != NULL; n = n->prox)
        printf("%sP%d(%d)", n == f->inicio ? "" : " ", n->proc->pid, n->proc->restante);
    printf("]");
}

/* ---------- Linha do tempo (para o Gantt) ---------- */
typedef struct {
    int pid;   /* 0 = CPU ociosa */
    int ini;
    int fim;
} Segmento;

Segmento seg[MAX_SEG];
int n_seg = 0;

void registrar(int pid, int t) {   /* registra 1 unidade [t, t+1) */
    if (n_seg > 0 && seg[n_seg - 1].pid == pid && seg[n_seg - 1].fim == t) {
        seg[n_seg - 1].fim = t + 1;            /* continua o mesmo segmento */
    } else if (n_seg < MAX_SEG) {
        seg[n_seg].pid = pid;
        seg[n_seg].ini = t;
        seg[n_seg].fim = t + 1;
        n_seg++;
    }
}

/* ---------- Programa principal ---------- */
int main(void) {
    Processo p[MAX_P];
    int n;

    printf("=== SRTF - Shortest Remaining Time First ===\n");
    printf("Quantidade de processos: ");
    if (scanf("%d", &n) != 1 || n <= 0 || n > MAX_P) {
        printf("Quantidade invalida.\n");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("P%d - tempo de chegada e burst: ", p[i].pid);
        if (scanf("%d %d", &p[i].chegada, &p[i].burst) != 2 ||
            p[i].chegada < 0 || p[i].burst <= 0) {
            printf("Entrada invalida.\n");
            return 1;
        }
        p[i].restante = p[i].burst;
        p[i].inicio = -1;
        p[i].termino = 0;
    }

    Fila prontos;
    fila_inicializar(&prontos);

    int tempo = 0, terminados = 0;
    int ultimo = -1;   /* pid que usou a CPU na unidade anterior (-1 = nenhum) */

    printf("\n--- Simulacao (eventos) ---\n");

    while (terminados < n) {
        /* 1) enfileira os processos que chegam neste instante */
        for (int i = 0; i < n; i++) {
            if (p[i].chegada == tempo) {
                inserir(&prontos, &p[i]);
                printf("t=%-3d chegada de P%d (burst %d)\n", tempo, p[i].pid, p[i].burst);
            }
        }

        /* 2) fila vazia -> CPU ociosa */
        if (fila_vazia(&prontos)) {
            if (ultimo != 0)
                printf("t=%-3d CPU ociosa\n", tempo);
            registrar(0, tempo);
            ultimo = 0;
            tempo++;
            continue;
        }

        /* 3) escolhe o menor tempo restante */
        Processo *exec = remover_menor_restante(&prontos);

        if (exec->inicio == -1)
            exec->inicio = tempo;

        if (exec->pid != ultimo) {
            printf("t=%-3d executa P%d (restante %d)  fila: ", tempo, exec->pid, exec->restante);
            imprimir_fila(&prontos);
            printf("\n");
        }

        /* executa 1 unidade de tempo */
        exec->restante--;
        registrar(exec->pid, tempo);
        tempo++;
        ultimo = exec->pid;

        /* 4) terminou ou volta para a fila */
        if (exec->restante == 0) {
            exec->termino = tempo;
            terminados++;
            printf("t=%-3d P%d terminou\n", tempo, exec->pid);
        } else {
            inserir(&prontos, exec);
        }
    }

    /* ---------- Ordem de execução (Gantt) ---------- */
    printf("\n--- Ordem de execucao (Gantt) ---\n|");
    for (int i = 0; i < n_seg; i++) {
        if (seg[i].pid == 0) printf(" ocioso |");
        else                 printf(" P%d |", seg[i].pid);
    }
    printf("\n%d", seg[0].ini);
    for (int i = 0; i < n_seg; i++)
        printf("%*d", seg[i].pid == 0 ? 9 : 5, seg[i].fim);
    printf("\n");

    /* ---------- Tabela de resultados ---------- */
    double soma_tat = 0, soma_esp = 0, soma_resp = 0;
    printf("\n--- Resultados ---\n");
    printf("%-4s %-8s %-6s %-8s %-11s %-8s %-8s\n",
           "PID", "Chegada", "Burst", "Termino", "Turnaround", "Espera", "Resposta");
    for (int i = 0; i < n; i++) {
        int tat  = p[i].termino - p[i].chegada;   /* turnaround */
        int esp  = tat - p[i].burst;              /* tempo de espera */
        int resp = p[i].inicio - p[i].chegada;    /* tempo de resposta */
        printf("P%-3d %-8d %-6d %-8d %-11d %-8d %-8d\n",
               p[i].pid, p[i].chegada, p[i].burst, p[i].termino, tat, esp, resp);
        soma_tat += tat;
        soma_esp += esp;
        soma_resp += resp;
    }
    printf("\nTurnaround medio: %.2f\n", soma_tat / n);
    printf("Espera media:     %.2f\n", soma_esp / n);
    printf("Resposta media:   %.2f\n", soma_resp / n);

    return 0;
}