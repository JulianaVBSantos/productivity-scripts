// Verrsão 1 - SEM CRIAR A SENHA
// NÃO ESTA FUNCIONANDO

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <ctype.h>

#define DELAY 500
#define MAX_LINHA 256
#define MAX_USUARIOS 300
#define MAX_CLICKS 23

volatile int parar = 0;

typedef struct {
    char nome[100];
    char email[100];
} Usuario;

typedef struct {
    int x, y;
    int tempo;
} Clique;

Clique cliques[MAX_CLICKS];
int totalCliques = 0;

DWORD WINAPI escutarTecla(LPVOID lpParam) {
    while (1) {
        if (GetAsyncKeyState(VK_F12) & 0x8000) {
            parar = 1;
            printf("\nAutomação interrompida pelo usuário!\n");
            return 0;
        }
        Sleep(100);
    }
}

void registrarClique(int x, int y, int tempo) {
    if (totalCliques < MAX_CLICKS) {
        cliques[totalCliques].x = x;
        cliques[totalCliques].y = y;
        cliques[totalCliques].tempo = tempo;
        totalCliques++;
        printf("Clique %d registrado: X=%d, Y=%d, Tempo=%d ms\n", totalCliques, x, y, tempo);
    }
}

void capturarCliques() {
    printf("Realize os 23 cliques nos campos necessários. Pressione ESC ao terminar.\n");
    totalCliques = 0;

    POINT p;
    DWORD tempoInicial = GetTickCount();
    while (totalCliques < MAX_CLICKS) {
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            GetCursorPos(&p);
            registrarClique(p.x, p.y, GetTickCount() - tempoInicial);
            tempoInicial = GetTickCount();
            Sleep(300);
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            break;
        }
        Sleep(50);
    }

    if (totalCliques < MAX_CLICKS) {
        printf("Aviso: foram capturados apenas %d cliques. São necessários 23.\n", totalCliques);
    }
}

void clickMouse(int x, int y) {
    if (parar) return;
    SetCursorPos(x, y);
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    Sleep(DELAY);
}

void digitarTexto(const char *texto) {
    while (*texto && !parar) {
        char c = *texto;

        switch (c) {
            case '/':
                keybd_event(0xC1, MapVirtualKey(0xC1, 0), 0, 0);
                keybd_event(0xC1, MapVirtualKey(0xC1, 0), KEYEVENTF_KEYUP, 0);
                break;
            case '@':
                keybd_event(VK_SHIFT, 0, 0, 0);
                keybd_event(0x32, 0, 0, 0);
                keybd_event(0x32, 0, KEYEVENTF_KEYUP, 0);
                keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
                break;
            default: {
                SHORT vk = VkKeyScan(c);
                BYTE vkCode = LOBYTE(vk);
                BYTE shiftState = HIBYTE(vk);

                if (vkCode == 0xFF) break;

                if (shiftState & 1) keybd_event(VK_SHIFT, 0, 0, 0);

                keybd_event(vkCode, MapVirtualKey(vkCode, 0), 0, 0);
                keybd_event(vkCode, MapVirtualKey(vkCode, 0), KEYEVENTF_KEYUP, 0);

                if (shiftState & 1) keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
            }
        }

        Sleep(50);
        texto++;
    }
}

int lerUsuarios(Usuario usuarios[], const char *arquivo) {
    FILE *file = fopen(arquivo, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo CSV!\n");
        return 0;
    }

    char linha[MAX_LINHA];
    int i = 0;
    while (fgets(linha, MAX_LINHA, file) && i < MAX_USUARIOS) {
        size_t len = strlen(linha);
        if (len > 0 && linha[len - 1] == '\n') {
            linha[len - 1] = '\0';
        }
        sscanf(linha, "%99[^,],%99[^,]", usuarios[i].nome, usuarios[i].email);
        i++;
    }
    fclose(file);
    return i;
}

void preencherCampos(Usuario usuario) {
    if (totalCliques < MAX_CLICKS) {
        printf("Erro: são necessários exatamente 23 cliques registrados.\n");
        return;
    }

    // Clique 1 - Email
    clickMouse(cliques[0].x, cliques[0].y);
    digitarTexto(usuario.email);

    // Clique 2 - Nome
    clickMouse(cliques[1].x, cliques[1].y);
    digitarTexto(usuario.nome);

    // Cliques 3-5 - "callcenter"
    for (int i = 2; i <= 4; i++) clickMouse(cliques[i].x, cliques[i].y);
    digitarTexto("callcenter");

    // Cliques 6-11 - "OSMOBILE"
    for (int i = 5; i <= 10; i++) clickMouse(cliques[i].x, cliques[i].y);
    digitarTexto("OSMOBILE");

    // Cliques 12-17 - "AUX"
    for (int i = 11; i <= 16; i++) clickMouse(cliques[i].x, cliques[i].y);
    digitarTexto("AUX");

    // Cliques 18-23 - Finalizar
    for (int i = 17; i <= 22; i++) clickMouse(cliques[i].x, cliques[i].y);
}

void iniciarAutomacao() {
    printf("Iniciando automação em 5 segundos...\n");
    Sleep(5000);
    printf("Iniciando...\n");
}

int main() {
    Usuario usuarios[MAX_USUARIOS];
    int totalUsuarios = lerUsuarios(usuarios, "usuarios.csv");
    if (totalUsuarios == 0) return 1;

    CreateThread(NULL, 0, escutarTecla, NULL, 0, NULL);
    printf("Realize o primeiro preenchimento manualmente e pressione ESC ao terminar.\n");
    capturarCliques();

    iniciarAutomacao();

    for (int i = 0; i < totalUsuarios && !parar; i++) {
        printf("Processando usuário: %s\n", usuarios[i].nome);
        preencherCampos(usuarios[i]);
        printf("Usuário %s processado.\n", usuarios[i].nome);
        Sleep(2000);
    }

    printf("Automação finalizada.\n");
    return 0;
}
