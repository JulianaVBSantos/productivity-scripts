#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

void corrigirArroba(char *string) {
    char *pos = strstr(string, "+AEA-"); // Encontra a primeira ocorrência de "+AEA-"
    while (pos != NULL) {
        strncpy(pos, "@", 1);  // Substitui "+AEA-" por "@"
        pos = strstr(pos + 1, "+AEA-"); 
    }
}

int main() {
    FILE *file = fopen("usuarios.csv", "r+");
    if (!file) {
        printf("Erro ao abrir o arquivo usuarios.csv\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    
    /*
    // Lê o cabeçalho (se houver)
    if (fgets(line, MAX_LINE_LENGTH, file)) {
        printf("Cabeçalho lido: %s", line);
    }*/

    long pos = ftell(file);

    while (fgets(line, MAX_LINE_LENGTH, file)) {
        // Corrige o símbolo @ nas linhas
        corrigirArroba(line);

        // Volta para a posição original
        fseek(file, pos, SEEK_SET);

        // Substitui a linha original com a linha corrigida
        fprintf(file, "%s", line);

        // Atualiza a posição para a próxima linha
        pos = ftell(file);
    }

    fclose(file);
    printf("Substituição concluída no arquivo usuarios.csv\n");
    return 0;
}