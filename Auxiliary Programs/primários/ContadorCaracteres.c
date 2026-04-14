// para determinar a quantidade de caracteres
// campo NOME do I-Manager só suporta 40 caracteres
// funcionando perfeitamente
// aprimorar: em vez de mostrar o numero da linha, mostrar o login do usuario

#include <stdio.h>
#include <string.h>

#define MAX_LINHA 1024

int main() {
    FILE *arquivo;
    char linha[MAX_LINHA];
    int numeroLinha = 1;

    arquivo = fopen("entrada.txt", "r");
    if (arquivo == NULL) {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    while (fgets(linha, MAX_LINHA, arquivo) != NULL) {
        size_t comprimento = strlen(linha);
        if (linha[comprimento - 1] == '\n') {
            linha[--comprimento] = '\0';
        }

        printf("Linha %d: %zu caracteres\n", numeroLinha++, comprimento);
    }

    fclose(arquivo);
    return 0;
}
