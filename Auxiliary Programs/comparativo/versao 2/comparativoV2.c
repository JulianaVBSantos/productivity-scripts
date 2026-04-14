// compara a coluna de uma planilha com uma lista arquivo.txt

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAX_LINHA 8000

// Verifica se a linha é vazia
int linha_vazia(const char *linha) {
    for (int i = 0; linha[i] != '\0'; i++) {
        if (!isspace((unsigned char)linha[i])) return 0;
    }
    return 1;
}

// Remove espaços do início e fim
void trim(char *str) {
    char *inicio = str;
    while (isspace((unsigned char)*inicio)) inicio++;

    if (inicio != str) memmove(str, inicio, strlen(inicio) + 1);

    char *fim = str + strlen(str) - 1;
    while (fim >= str && isspace((unsigned char)*fim)) *fim-- = '\0';
}

// Extrai um campo específico do CSV
void extrair_coluna_csv(const char *linha, int coluna_desejada, char *saida) {
    char copia[MAX_LINHA];
    strcpy(copia, linha);

    char *token = strtok(copia, ";");
    int coluna_atual = 0;

    while (token != NULL) {
        if (coluna_atual == coluna_desejada) {
            strcpy(saida, token);
            trim(saida);
            return;
        }
        token = strtok(NULL, ";");
        coluna_atual++;
    }

    saida[0] = '\0'; // caso a coluna não exista
}

int main() {
    FILE *arq_txt, *arq_csv, *arq_encontrados, *arq_naoencontrados;
    char linha_txt[MAX_LINHA];
    char linha_csv[MAX_LINHA];
    char campo_csv[MAX_LINHA];
    int coluna_desejada;

    printf("=== Comparar TXT com coluna de CSV e gerar 2 planilhas ===\n");
    printf("Digite o número da coluna do CSV (0 = primeira): ");
    scanf("%d", &coluna_desejada);
    getchar();

    arq_txt = fopen("usuarios.txt", "r");
    arq_csv = fopen("usuarios.csv", "r");
    arq_encontrados = fopen("encontrados.csv", "w");
    arq_naoencontrados = fopen("nao_encontrados.csv", "w");

    if (!arq_txt || !arq_csv || !arq_encontrados || !arq_naoencontrados) {
        perror("Erro ao abrir os arquivos");
        return 1;
    }

    printf("\nProcessando...\n");

    // Lê todos os valores do TXT e armazena na memória
    char **usuarios = NULL;
    int total_usuarios = 0;

    while (fgets(linha_txt, sizeof(linha_txt), arq_txt)) {
        trim(linha_txt);
        if (linha_vazia(linha_txt)) continue;

        usuarios = realloc(usuarios, (total_usuarios + 1) * sizeof(char *));
        usuarios[total_usuarios] = strdup(linha_txt);
        total_usuarios++;
    }

    // Percorre a planilha CSV
    while (fgets(linha_csv, sizeof(linha_csv), arq_csv)) {
        char original[MAX_LINHA];
        strcpy(original, linha_csv);
        linha_csv[strcspn(linha_csv, "\n")] = '\0';
        trim(linha_csv);
        if (linha_vazia(linha_csv)) continue;

        extrair_coluna_csv(linha_csv, coluna_desejada, campo_csv);
        trim(campo_csv);

        int encontrado = 0;
        for (int i = 0; i < total_usuarios; i++) {
            if (strcmp(campo_csv, usuarios[i]) == 0) {
                fprintf(arq_encontrados, "%s", original);
                if (original[strlen(original) - 1] != '\n') fprintf(arq_encontrados, "\n");
                encontrado = 1;
                break;
            }
        }

        if (!encontrado) {
            fprintf(arq_naoencontrados, "%s", original);
            if (original[strlen(original) - 1] != '\n') fprintf(arq_naoencontrados, "\n");
        }
    }

    printf("\nGerados com sucesso:\n");
    printf(" - encontrados.csv\n");
    printf(" - nao_encontrados.csv\n");

    // Libera memória
    for (int i = 0; i < total_usuarios; i++) free(usuarios[i]);
    free(usuarios);

    fclose(arq_txt);
    fclose(arq_csv);
    fclose(arq_encontrados);
    fclose(arq_naoencontrados);

    return 0;
}