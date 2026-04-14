// em fase de testes
// baseado no doc do unm

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <ctype.h>

#define DELAY 500
#define MAX_LINHA 256
#define MAX_USUARIOS 500
#define MAX_CLICKS 30

volatile int parar = 0;

typedef struct {
    char nome[100];
    char email[100];
    char login[50];
    char senha[50];
    char descricao[100];
} Usuario;

typedef struct {
    int x, y;
    int tempo;
    int botao; // 0 = esquerdo, 1 = direito
} Clique;

Clique cliques[MAX_CLICKS];
int totalCliques = 0;

DWORD WINAPI escutarTecla(LPVOID lpParam) {
    while (1) {
        if (GetAsyncKeyState(VK_F12) & 0x8000) {
            parar = 1;
            printf("\n>>> Automacao interrompida pelo usuario (F12)!\n");
            return 0;
        }
        Sleep(100);
    }
}

void registrarClique(int x, int y, int tempo, int botao) {
    if (totalCliques < MAX_CLICKS) {
        cliques[totalCliques].x = x;
        cliques[totalCliques].y = y;
        cliques[totalCliques].tempo = tempo;
        cliques[totalCliques].botao = botao;
        totalCliques++;
        printf("Clique registrado: X=%d, Y=%d, Tempo=%d ms, Botao=%s\n",
               x, y, tempo, botao == 0 ? "Esquerdo" : "Direito");
    }
}

void capturarCliques() {
    printf("=== Gravacao de cliques ===\n");
    printf("Realize os cliques na ORDEM dos campos:\n");
    printf("1) Login\n2) Nome\n3) Senha\n4) Confirmacao Senha\n5) Descricao\n6) Email\n");
    printf("Pressione ESC para encerrar a gravacao.\n");

    totalCliques = 0;
    POINT p;
    DWORD tempoInicial = GetTickCount();

    while (totalCliques < MAX_CLICKS) {
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            GetCursorPos(&p);
            registrarClique(p.x, p.y, GetTickCount() - tempoInicial, 0);
            tempoInicial = GetTickCount();
            Sleep(300);
        }
        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
            GetCursorPos(&p);
            registrarClique(p.x, p.y, GetTickCount() - tempoInicial, 1);
            tempoInicial = GetTickCount();
            Sleep(300);
        }
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            printf("Gravacao encerrada pelo usuario (ESC).\n");
            break;
        }
        Sleep(50);
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
    while (*texto) {
        if (parar) return;

        char ch = *texto;

        switch (ch) {
            case '@':
                keybd_event(VK_SHIFT, 0, 0, 0);
                keybd_event(0x32, 0, 0, 0); // "2"
                keybd_event(0x32, 0, KEYEVENTF_KEYUP, 0);
                keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
                break;

            case '.':
                keybd_event(VK_OEM_PERIOD, 0, 0, 0);
                keybd_event(VK_OEM_PERIOD, 0, KEYEVENTF_KEYUP, 0);
                break;

            case '-':
                keybd_event(VK_OEM_MINUS, 0, 0, 0);
                keybd_event(VK_OEM_MINUS, 0, KEYEVENTF_KEYUP, 0);
                break;

            case '_':
                keybd_event(VK_SHIFT, 0, 0, 0);
                keybd_event(VK_OEM_MINUS, 0, 0, 0);
                keybd_event(VK_OEM_MINUS, 0, KEYEVENTF_KEYUP, 0);
                keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
                break;

            default: {
                SHORT vk = VkKeyScan(ch);
                if (vk == -1) {
                    printf("Caractere nao suportado: %c\n", ch);
                    texto++;
                    continue;
                }
                if ((vk >> 8) & 1) keybd_event(VK_SHIFT, 0, 0, 0);

                keybd_event((BYTE)vk, 0, 0, 0);
                keybd_event((BYTE)vk, 0, KEYEVENTF_KEYUP, 0);

                if ((vk >> 8) & 1) keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
                break;
            }
        }

        Sleep(20);
        texto++;
    }
}

void pressTab() {
    keybd_event(VK_TAB, 0, 0, 0);
    keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
    Sleep(DELAY);
}

void pressCtrlA() {
    keybd_event(VK_CONTROL, 0, 0, 0);
    keybd_event(0x41, 0, 0, 0); // tecla "A"
    keybd_event(0x41, 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
    Sleep(100);
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
        sscanf(linha, "%99[^,],%99[^,],%49[^,],%49[^,],%99[^,]", 
               usuarios[i].nome, usuarios[i].email, usuarios[i].login, usuarios[i].senha, usuarios[i].descricao);
        i++;
    }
    fclose(file);
    return i;
}

void preencherCampos(Usuario usuario) {

    // Login
    clickMouse(cliques[0].x, cliques[0].y);
    pressCtrlA();
    digitarTexto(usuario.login);
    pressTab();

    // Nome
    pressCtrlA();
    digitarTexto(usuario.nome);
    pressTab();

    // Senha
    pressCtrlA();
    digitarTexto(usuario.senha);
    pressTab();

    // Confirmacao Senha
    pressCtrlA();
    digitarTexto(usuario.senha);
    pressTab();

    // Descricao
    pressCtrlA();
    digitarTexto(usuario.descricao);
    pressTab();

    // Email
    pressCtrlA();
    digitarTexto(usuario.email);
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

    printf("Realize o primeiro preenchimento manualmente (cliques) e pressione ESC ao terminar a gravacao.\n");
    capturarCliques();

    iniciarAutomacao();

    printf("Processando usuarios do CSV...\n");

    for (int i = 0; i < totalUsuarios && !parar; i++) {
        printf(">>> Processando usuario: %s\n", usuarios[i].nome);

        preencherCampos(usuarios[i]);

        printf("Usuario %s processado.\n", usuarios[i].nome);
        Sleep(2000);
    }

    printf("Todos os usuarios foram processados. Finalizando automacao.\n");
    return 0;
}
