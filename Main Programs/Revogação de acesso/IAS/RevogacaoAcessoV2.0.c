//revogar acesso no IAS

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

void clickMouse(int x, int y);
void typeText(const char* text);
void clearText();

int main() {
    printf("Automacao iniciada.\n");
    printf("Realize o procedimento manualmente. Pressione ESC ao terminar.\n");

    DWORD ultimoTempo = GetTickCount(); 
    int recording = 1;

    while (recording) {
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            POINT pt;
            if (GetCursorPos(&pt)) {
                DWORD agora = GetTickCount();
                DWORD tempoDecorrido = agora - ultimoTempo;

                passos[totalPassos].x = pt.x;
                passos[totalPassos].y = pt.y;
                passos[totalPassos].tempoEspera = tempoDecorrido;
                totalPassos++;

                printf("Clique registrado: X=%d, Y=%d, Tempo desde o último clique: %lu ms\n", pt.x, pt.y, tempoDecorrido);

                ultimoTempo = agora;
                Sleep(300);
            }
        }

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            printf("Gravacao finalizada. Iniciando automacao.\n");
            recording = 0;
            Sleep(1000);
        }
    }

    FILE* arquivo = fopen("usuarios.txt", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
    printf("Arquivo localizado! Iniciando automacao.\n");

    char nomeUsuario[256];
    char proximoUsuario[256];
    int ultimoUsuario = 0;

    if (!fgets(nomeUsuario, sizeof(nomeUsuario), arquivo)) {
        printf("Arquivo vazio ou erro ao ler.\n");
        fclose(arquivo);
        return 1;
    }

    while (1) {
        nomeUsuario[strcspn(nomeUsuario, "\n")] = 0;

        if (fgets(proximoUsuario, sizeof(proximoUsuario), arquivo)) {
            proximoUsuario[strcspn(proximoUsuario, "\n")] = 0;
        } else {
            ultimoUsuario = 1;
        }

        printf("Processando usuario: %s\n", nomeUsuario);

        for (int i = 0; i < totalPassos; i++) {
            if (i == 3) {
                clearText(); 
                typeText(nomeUsuario);
                Sleep(500);
            }
            Sleep(passos[i].tempoEspera);
            clickMouse(passos[i].x, passos[i].y); 

        }

        printf("Usuario %s revogado.\n", nomeUsuario);

        if (ultimoUsuario) {
            printf("Ultimo usuario processado. Finalizando automacao.\n");
            break;
        }

        strcpy(nomeUsuario, proximoUsuario);
    }

    fclose(arquivo);
    printf("Automacao concluida.\n");
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
        keybd_event((BYTE)key, 0, 0, 0);
        keybd_event((BYTE)key, 0, KEYEVENTF_KEYUP, 0);
        Sleep(50);
    }
}

void clearText() {
    keybd_event(VK_CONTROL, 0, 0, 0);
    keybd_event(0x41, 0, 0, 0);      
    keybd_event(0x41, 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);

    Sleep(500);

    for (int i = 0; i < 10; i++) { 
        keybd_event(0x08, 0, 0, 0);  
        keybd_event(0x08, 0, KEYEVENTF_KEYUP, 0);
        Sleep(50);
    }
}