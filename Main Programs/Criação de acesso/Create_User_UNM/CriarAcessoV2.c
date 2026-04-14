//automação funcionando perfeitamente

#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <ctype.h>

#define DELAY 200
#define MAX_LINHA 256
#define MAX_USUARIOS 500
#define MAX_CLICKS 9

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
} Clique;

Clique cliques[MAX_CLICKS];
int totalCliques = 0;

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

void registrarClique(int x, int y, int tempo) {
    if (totalCliques < MAX_CLICKS) {
        cliques[totalCliques].x = x;
        cliques[totalCliques].y = y;
        cliques[totalCliques].tempo = tempo;
        totalCliques++;
        printf("Clique registrado: X=%d, Y=%d, Tempo=%d ms\n", x, y, tempo);
    }
}

void capturarCliques() {
    printf("Realize os 6 cliques necessários. Pressione ESC ao terminar.\n");
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

        if (*texto == '@') {
            // Tratamento específico para o caractere "@"
            keybd_event(VK_SHIFT, 0, 0, 0); // Shift pressionado
            keybd_event(0x32, 0, 0, 0);     // Digitar o "2" (0x32 é o código de tecla para "2")
            keybd_event(0x32, 0, KEYEVENTF_KEYUP, 0); // Liberar "2"
            keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0); // Liberar Shift
        } else {
            SHORT vk = VkKeyScan(*texto);
            if (isupper(*texto)) keybd_event(VK_SHIFT, 0, 0, 0);
            
            keybd_event((BYTE)vk, 0, 0, 0);
            keybd_event((BYTE)vk, 0, KEYEVENTF_KEYUP, 0);
            
            if (isupper(*texto)) keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
        }

        Sleep(10);
        texto++;
    }
}


void pressTab() {
    keybd_event(VK_TAB, 0, 0, 0);
    keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);
    Sleep(DELAY);
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
            linha[len - 1] = '\0'; // Substitui o '\n' por '\0'
        }
        sscanf(linha, "%99[^,],%99[^,],%49[^,],%49[^,],%99[^,]", 
               usuarios[i].nome, usuarios[i].email, usuarios[i].login, usuarios[i].senha, usuarios[i].descricao);
        i++;
    }
    fclose(file);
    return i;
}

void preencherCampos(Usuario usuario) {

    clickMouse(cliques[0].x, cliques[0].y);
    digitarTexto(usuario.login);
    pressTab();

    digitarTexto(usuario.nome);
    pressTab();

    digitarTexto("NULL");
    pressTab();

    digitarTexto(usuario.email);
    pressTab();

    digitarTexto(usuario.descricao);
    pressTab();
    pressTab();
    pressTab();

    digitarTexto(usuario.senha);
    pressTab();

    digitarTexto(usuario.senha);
    pressTab();
}

void iniciarAutomacao() {
    printf("Iniciando automação em 5 segundos...\n");
    Sleep(5000); // 5000 ms = 5 segundos
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

    printf("Iniciando automação para os usuários do CSV.\n");

    for (int i = 0; i < totalUsuarios && !parar; i++) {
        printf("Processando usuário: %s\n", usuarios[i].nome);

        preencherCampos(usuarios[i]);

        for (int j = 0; j < totalCliques; j++) {
            clickMouse(cliques[j].x, cliques[j].y);
        }

        printf("Usuário %s processado.\n", usuarios[i].nome);
        //Sleep(1500);
        Sleep(500); // Pausa de meio segundo entre os usuários
    }

    printf("Último usuário processado. Finalizando automação.\n");
    return 0;
}
