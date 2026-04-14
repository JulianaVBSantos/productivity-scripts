#include <stdio.h>
#include <string.h>
#include <ctype.h>

// melhoria: a lista de logins não deve conter linhas vazias
// melhoria: a lista de logins não deve conter acentos

#define MAX_NOME 100
#define MAX_LOGIN 100

void gerarLogin(char nomeCompleto[], char login[]) {
    char *token;
    char separadores[] = " ";
    char *ignorados[] = {"de", "da", "das", "do", "dos"};
    int ignoradosCount = 5;

    login[0] = '\0'; 

    token = strtok(nomeCompleto, separadores);

    if (token != NULL) {
        strcat(login, token);
        token = strtok(NULL, separadores);
    }

    while (token != NULL) {
        int deveIgnorar = 0;
        for (int i = 0; i < ignoradosCount; i++) {
            if (strcasecmp(token, ignorados[i]) == 0) {
                deveIgnorar = 1;
                break;
            }
        }

        if (!deveIgnorar) {
            char letra = toupper(token[0]);
            strncat(login, &letra, 1);
        }

        token = strtok(NULL, separadores);
    }
}

int main() {
    char nomeCompleto[MAX_NOME];
    char login[MAX_LOGIN];
    FILE *arquivoEntrada, *arquivoSaida;

    arquivoEntrada = fopen("nomes.txt", "r");
    if (arquivoEntrada == NULL) {
        perror("Erro ao abrir o arquivo de entrada");
        return 1;
    }

    arquivoSaida = fopen("logins.txt", "w");
    if (arquivoSaida == NULL) {
        perror("Erro ao criar o arquivo de saída");
        fclose(arquivoEntrada);
        return 1;
    }

    while (fgets(nomeCompleto, MAX_NOME, arquivoEntrada) != NULL) {
        
        nomeCompleto[strcspn(nomeCompleto, "\n")] = '\0';

        gerarLogin(nomeCompleto, login);
        fprintf(arquivoSaida, "%s\n", login);
    }

    fclose(arquivoEntrada);
    fclose(arquivoSaida);

    printf("Logins gerados no arquivo 'logins.txt'.\n");

    return 0;
}