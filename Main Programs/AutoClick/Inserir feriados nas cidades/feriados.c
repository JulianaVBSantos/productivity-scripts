// CODIGO PRECISA SER CORRIGIDO!!!

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct {
    int x, y;
    DWORD tempoEspera;
} Passo;

Passo passos[20];
int totalPassos = 0;

char primeiraLinhaColunaA[100]; // Para armazenar CIDADE
char primeiraLinhaColunaB[100]; // Para armazenar DATA
char primeiraLinhaColunaC[100]; // Para armazenar TIPO DE FERIADO

int primeiroCliqueX = -1;
int primeiroCliqueY = -1;
int segundoCliqueX = -1;
int segundoCliqueY = -1;
int terceiroCliqueX = -1;
int terceiroCliqueY = -1;

void clickMouse(int x, int y);
void typeText(const char* text);
void pressEnter();
void pressTab();
void processCSV();  // Função para processar o CSV

int main() {
    printf("Automacao iniciada.\n");
    printf("Realize o procedimento manualmente. Pressione ESC ao terminar.\n");

    DWORD ultimoTempo = GetTickCount();
    int recording = 1;

    // Processar o arquivo CSV para pegar a primeira linha da coluna A
    processCSV();

    while (recording) {
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            POINT pt;
            if (GetCursorPos(&pt)) {
                DWORD agora = GetTickCount();
                DWORD tempoDecorrido = agora - ultimoTempo;

                if (primeiroCliqueX == -1 && primeiroCliqueY == -1) {
                    // Primeiro clique manual (captura a posição do campo de digitação)
                    primeiroCliqueX = pt.x;
                    primeiroCliqueY = pt.y;
                    printf("Primeiro clique registrado: X=%d, Y=%d\n", pt.x, pt.y);
                    
                    // Simula a digitação do nome usando a primeira linha da coluna A (CIDADE)
                    clickMouse(primeiroCliqueX, primeiroCliqueY);
                    typeText(primeiraLinhaColunaA); 
                    pressEnter();
                    Sleep(500);
                    printf("Cidade digitada: %s\n", primeiraLinhaColunaA);
                    ultimoTempo = agora;
                } else if (segundoCliqueX == -1 && segundoCliqueY == -1) {
                    segundoCliqueX = pt.x;
                    segundoCliqueY = pt.y;
                    clickMouse(segundoCliqueX, segundoCliqueY);
                    clickMouse(segundoCliqueX, segundoCliqueY);
                    clickMouse(segundoCliqueX, segundoCliqueY);
                    typeText(primeiraLinhaColunaB);
                    printf("Data digitada: %s\n", primeiraLinhaColunaB);
                    pressTab(); // Pressiona TAB para ir para o próximo campo
                    Sleep(500);
                    ultimoTempo = agora;
                } else if (terceiroCliqueX == -1 && terceiroCliqueY == -1) {
                    terceiroCliqueX = pt.x;
                    terceiroCliqueY = pt.y;
                    typeText(primeiraLinhaColunaC);
                    printf("Tipo de feriado digitado: %s\n", primeiraLinhaColunaC);
                    Sleep(500);
                    ultimoTempo = agora;
                    clickMouse(terceiroCliqueX, terceiroCliqueY);
                    clickMouse(terceiroCliqueX, terceiroCliqueY);
                }

                if (primeiroCliqueX != -1 && segundoCliqueX != -1 && terceiroCliqueX != -1) {
                    printf("Clique manual concluído. Pressione ESC para iniciar a automação.\n");
                }
            }
            Sleep(300);
        }

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            printf("Gravação finalizada. Iniciando automação para o restante das linhas do CSV.\n");
            recording = 0;
            Sleep(1000);
        }
    }

    printf("Aguardando 5 segundos antes de processar as próximas linhas.\n");
    Sleep(5000);

    printf("Processando o arquivo CSV...\n");

    FILE* csvFile = fopen("feriados.csv", "r");
    if (!csvFile) {
        printf("Erro ao abrir o arquivo CSV.\n");
        return 1;
    }

    char linha[256];
    fgets(linha, sizeof(linha), csvFile); // Pular a primeira linha (já foi usada manualmente)

    while (fgets(linha, sizeof(linha), csvFile)) {
        char colunaA[100], colunaB[100], colunaC[100];
        sscanf(linha, "%[^,],%[^,],%s", colunaA, colunaB, colunaC);

        printf("Processando a cidade: %s\n", colunaA);

        clickMouse(primeiroCliqueX, primeiroCliqueY);
        typeText(primeiraLinhaColunaA); 
        pressEnter();
        Sleep(500);
        printf("Cidade digitada: %s\n", primeiraLinhaColunaA);

        clickMouse(segundoCliqueX, segundoCliqueY);
        clickMouse(segundoCliqueX, segundoCliqueY);
        clickMouse(segundoCliqueX, segundoCliqueY);
        typeText(primeiraLinhaColunaB);
        printf("Data digitada: %s\n", primeiraLinhaColunaB);
        pressTab();
        Sleep(500);

        typeText(primeiraLinhaColunaC);
        printf("Tipo de feriado digitado: %s\n", primeiraLinhaColunaC);
        Sleep(500);
        clickMouse(terceiroCliqueX, terceiroCliqueY);
        clickMouse(terceiroCliqueX, terceiroCliqueY);

        printf("Cadastro do usuário %s concluído.\n", colunaA);
    }

    fclose(csvFile);

    printf("Automação concluída.\n");
    return 0;
}

void clickMouse(int x, int y) {
    SetCursorPos(x, y);
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

void typeText(const char* text) {
    for (int i = 0; text[i] != '\0'; i++) {
        SHORT key = VkKeyScan(text[i]);

        if (key == -1) {
            keybd_event(VK_SHIFT, 0, 0, 0);
            keybd_event(VkKeyScan(text[i]), 0, 0, 0);
            keybd_event(VkKeyScan(text[i]), 0, KEYEVENTF_KEYUP, 0);
            keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
        } else {
            keybd_event((BYTE)key, 0, 0, 0);
            keybd_event((BYTE)key, 0, KEYEVENTF_KEYUP, 0);
        }
        Sleep(50);
    }
}

void pressEnter() {
    keybd_event(VK_RETURN, 0, 0, 0);  // Pressiona ENTER
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);  // Libera ENTER
}

void pressTab() {
    keybd_event(VK_TAB, 0, 0, 0);  // Pressiona TAB
    keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);  // Libera TAB
}

void processCSV() {
    // Aqui você pode implementar a leitura do arquivo CSV, mas como já tratamos a leitura no main,
    // não é necessário repetir a função. A leitura do CSV é feita dentro do laço principal
    // após o primeiro clique.

    // Leitura da primeira linha do arquivo CSV
    FILE* csvFile = fopen("feriados.csv", "r");
    if (!csvFile) {
        printf("Erro ao abrir o arquivo CSV.\n");
        return;
    }

    char linha[256];
    if (fgets(linha, sizeof(linha), csvFile)) {
        // Lê a primeira linha e pega as colunas A, B, e C
        sscanf(linha, "%[^,],%[^,],%s", primeiraLinhaColunaA, primeiraLinhaColunaB, primeiraLinhaColunaC);
        printf("Primeira linha lida - CIDADE: %s, DATA: %s, TIPO DE FERIADO: %s\n", primeiraLinhaColunaA, primeiraLinhaColunaB, primeiraLinhaColunaC);
    }

    fclose(csvFile);
}
