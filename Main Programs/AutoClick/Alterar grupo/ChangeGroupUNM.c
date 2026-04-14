// Programa feito para alterar o grupo de usuários do UNM
// funcionando perfeitamente

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
void pressEnter();

int main() {
    printf("Automacao iniciada.\n");
    printf("Realize o procedimento manualmente. Pressione ESC ao terminar.\n");

    DWORD ultimoTempo = GetTickCount();
    int recording = 1;
    int firstClick = 0;

    FILE* arquivo = fopen("usuarios.txt", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    char nomeUsuario[256];
    if (!fgets(nomeUsuario, sizeof(nomeUsuario), arquivo)) {
        printf("Erro ao ler o primeiro ser humano.\n");
        fclose(arquivo);
        return 1;
    }
    nomeUsuario[strcspn(nomeUsuario, "\n")] = 0;

    printf("Primeiro usuário: %s\n", nomeUsuario);

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

                if (!firstClick) {
                    firstClick = 1;
                    printf("Primeiro clique registrado. Iniciando digitação do nome do usuário.\n");
                    typeText(nomeUsuario); 
                    pressEnter(); 
                    Sleep(500); 
                    printf("Nome digitado: %s\n", nomeUsuario);
                }

                if (totalPassos >= 7) { // 1 para digitação, 6 para cliques sub
                    printf("7 cliques registrados. Pressione ESC para finalizar a gravação.\n");
                    break;
                }
            }
            Sleep(300);
        }

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            printf("Gravação finalizada para o primeiro usuário.\n");
            recording = 0;
            Sleep(1000);
        }
    }

    fclose(arquivo); 

    printf("Aguardando 5 segundos antes de processar os próximos usuários...\n");
    Sleep(5000);

    arquivo = fopen("usuarios.txt", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fgets(nomeUsuario, sizeof(nomeUsuario), arquivo);

    while (fgets(nomeUsuario, sizeof(nomeUsuario), arquivo)) {
        nomeUsuario[strcspn(nomeUsuario, "\n")] = 0;

        printf("Processando o usuário: %s\n", nomeUsuario);

        for (int i = 0; i < totalPassos; i++) {
            Sleep(passos[i].tempoEspera);
            clickMouse(passos[i].x, passos[i].y);
        }

        typeText(nomeUsuario);
        pressEnter();  
        Sleep(500);
        printf("Cadastro do usuário %s concluído.\n", nomeUsuario);
    }

    fclose(arquivo);
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
    keybd_event(VK_RETURN, 0, 0, 0);  // P/ Pressionar ENTER
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);  // P/ Liberar ENTER
}
