// automação funcionando perfeitamente
// PRECISA CRIAR UMA TELA WINDOW FLOAT PRA ELE PEGAR O USUÁRIO BEM EM CIMA
// v. imagem do teams (https://teams.microsoft.com/l/message/48:notes/1755610548625?context=%7B%22contextType%22%3A%22chat%22%2C%22oid%22%3A%228%3Aorgid%3Aac97065c-cf9b-4c13-8d55-e37db7179810%22%7D)
// implementar quantidade de usuários restantes
// p o unm spc, precisa clicar na setinha verde pra cima ao lado da lupa

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <ctype.h>

#define DELAY 200
#define MAX_LINHA 256
#define MAX_USUARIOS 300
#define MAX_CLICKS 9

volatile int parar = 0;

typedef struct {
    char login[50];
} Usuario;

typedef struct {
    int x, y;
    int tempo;
    int botao; // 0 = esquerdo, 1 = direito
} Clique;

Clique cliques[MAX_CLICKS];
int totalCliques = 0;

// Thread para escutar F12 e parar automação
// ainda não testado
DWORD WINAPI escutarTecla(LPVOID lpParam) {
    while (1) {
        if (GetAsyncKeyState(VK_F12) & 0x8000) {
            parar = 1;
            printf("\nAutomacao interrompida pelo usuario!\n");
            return 0;
        }
        Sleep(100);
    }
}

// Registro de cliques
void registrarClique(int x, int y, int tempo, int botao) {
    if (totalCliques < MAX_CLICKS) {
        cliques[totalCliques].x = x;
        cliques[totalCliques].y = y;
        cliques[totalCliques].tempo = tempo;
        cliques[totalCliques].botao = botao;
        totalCliques++;
        printf("Clique %s registrado: X=%d, Y=%d, Tempo=%d ms\n",
               botao == 0 ? "ESQUERDO" : "DIREITO", x, y, tempo);
    }
}

// Captura cliques do usuário
void capturarCliques() {
    printf("Realize os cliques necessários (esquerdo ou direito). Pressione ESC ao terminar.\n");
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
            break;
        }
        Sleep(50);
    }
}

// Executa clique do mouse conforme tipo
void clickMouse(int x, int y, int botao) {
    if (parar) return;
    SetCursorPos(x, y);

    if (botao == 0) { // esquerdo
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    } else { // direito
        mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
    }

    Sleep(DELAY);
}

// Digita texto usando o método original que funciona
void digitarTexto(const char *texto) {
    while (*texto) {
        if (parar) return;

        SHORT vk = VkKeyScan(*texto);
        if (isupper(*texto)) keybd_event(VK_SHIFT, 0, 0, 0);

        keybd_event((BYTE)vk, 0, 0, 0);
        keybd_event((BYTE)vk, 0, KEYEVENTF_KEYUP, 0);

        if (isupper(*texto)) keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);

        Sleep(10);
        texto++;
    }
}

// se pá nem precisa
void pressTab() {
    keybd_event(VK_TAB, 0, 0, 0);
    keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
    Sleep(DELAY);
}

// Lê logins de um arquivo TXT, 1 por linha
int lerUsuarios(Usuario usuarios[], const char *arquivo) {
    FILE *file = fopen(arquivo, "r");
    if (!file) {
        printf("Erro ao abrir o arquivo!\n");
        return 0;
    }

    char linha[MAX_LINHA];
    int i = 0;
    while (fgets(linha, MAX_LINHA, file) && i < MAX_USUARIOS) {
        size_t len = strlen(linha);
        if (len > 0 && linha[len - 1] == '\n') {
            linha[len - 1] = '\0'; 
        }
        strncpy(usuarios[i].login, linha, sizeof(usuarios[i].login)-1);
        usuarios[i].login[sizeof(usuarios[i].login)-1] = '\0';
        i++;
    }
    fclose(file);
    return i;
}

// Aguarda 5 segundos antes de iniciar automação
void iniciarAutomacao() {
    printf("Iniciando automação em 5 segundos...\n");
    Sleep(5000);
    printf("Iniciando...\n");
}

int main() {
    Usuario usuarios[MAX_USUARIOS];
    int totalUsuarios = lerUsuarios(usuarios, "usuarios.txt");
    if (totalUsuarios == 0) return 1;

    CreateThread(NULL, 0, escutarTecla, NULL, 0, NULL);

    printf("Realize o primeiro preenchimento manualmente e pressione ESC ao terminar.\n");
    capturarCliques();

    iniciarAutomacao();

    printf("Iniciando automação para os logins.\n");

    for (int i = 0; i < totalUsuarios && !parar; i++) {
        printf("Processando login: %s\n", usuarios[i].login);

        // Primeiro clique + digitar login
        clickMouse(cliques[0].x, cliques[0].y, cliques[0].botao);
        Sleep(300); // garante foco
        digitarTexto(usuarios[i].login);
        pressTab();

        // Executa demais cliques se houver
        for (int j = 1; j < totalCliques; j++) {
            clickMouse(cliques[j].x, cliques[j].y, cliques[j].botao);
        }

        printf("Login %s processado.\n", usuarios[i].login);
        Sleep(2000);
    }

    printf("Último login processado. Finalizando automação.\n");
    return 0;
}