// funcionando perfeitamente

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINHA 8000

// Função para verificar se a linha é vazia (ou só tem espaço/tabs)
int linha_vazia(const char *linha) {
    for (int i = 0; linha[i] != '\0'; i++) {
        if (!isspace((unsigned char)linha[i])) {
            return 0; // encontrou algo que não é espaço, então não é vazia
        }
    }
    return 1; // é vazia ou só tem espaços/tabs/enter
}

int verificar_usuario(FILE *arq2, const char *linha) {
    char linha_arq2[MAX_LINHA];
    rewind(arq2);

    while (fgets(linha_arq2, sizeof(linha_arq2), arq2)) {
        linha_arq2[strcspn(linha_arq2, "\n")] = '\0';

        if (linha_vazia(linha_arq2)) continue; // IGNORA LINHA VAZIA NO arquivo 2

        if (strcmp(linha_arq2, linha) == 0) {
            return 1;
        }
    }
    return 0;
}

int main() {
    FILE *arq1, *arq2;
    char linha[MAX_LINHA];

    arq1 = fopen("VerSeTem.txt", "r");
    arq2 = fopen("total.txt", "r");

    if (arq1 == NULL || arq2 == NULL) {
        perror("Erro ao abrir os arquivos");
        return 1;
    }

    printf("Comparando os arquivos...\n");

    while (fgets(linha, sizeof(linha), arq1)) {
        linha[strcspn(linha, "\n")] = '\0';

        if (linha_vazia(linha)) continue; // IGNORA LINHA VAZIA NO arquivo 1

        if (verificar_usuario(arq2, linha)) {
            printf("%s\n", linha);
        } else {
            printf("-------------Não encontrado: %s\n", linha);
        }
    }

    fclose(arq1);
    fclose(arq2);

    return 0;
}