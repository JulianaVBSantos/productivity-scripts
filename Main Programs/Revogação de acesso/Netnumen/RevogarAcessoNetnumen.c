#include <windows.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    int x, y;
    DWORD tempoEspera;
    int botao; // 0 para esquerdo, 1 para direito
} Passo;

Passo passos[10]; // Aumentando para 10 cliques
int totalPassos = 0;
int primeiroCliqueRegistrado = 0;

void clickMouse(int x, int y, int botao);
void typeText(const char* text);
void pressEnter();
void clearField();

int main() {
    printf("Automação iniciada. Clique no campo de inserção do nome e pressione ESC ao finalizar a primeira entrada.\n");

    DWORD ultimoTempo = GetTickCount();
    int recording = 1;

    FILE* arquivo = fopen("usuarios.txt", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    char nomeUsuario[256];
    if (!fgets(nomeUsuario, sizeof(nomeUsuario), arquivo)) {
        printf("Erro ao ler o primeiro usuário.\n");
        fclose(arquivo);
        return 1;
    }
    nomeUsuario[strcspn(nomeUsuario, "\n")] = 0;

    printf("Primeiro usuário: %s\n", nomeUsuario);

    while (recording) {
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000 || GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
            POINT pt;
            if (GetCursorPos(&pt)) {
                DWORD agora = GetTickCount();
                DWORD tempoDecorrido = agora - ultimoTempo;
                int botao = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) ? 0 : 1;

                if (!primeiroCliqueRegistrado) {
                    passos[0].x = pt.x;
                    passos[0].y = pt.y;
                    passos[0].botao = 0; // Primeiro clique sempre esquerdo
                    primeiroCliqueRegistrado = 1;
                    printf("Primeiro clique registrado: X=%d, Y=%d\n", pt.x, pt.y);

                    // Executa a inserção do nome no primeiro clique
                    clickMouse(passos[0].x, passos[0].y, 0);
                    Sleep(200);
                    clearField();
                    typeText(nomeUsuario);
                    pressEnter();
                    Sleep(500);
                } else if (totalPassos < 9) { // Agora captura até 9 cliques
                    totalPassos++;
                    passos[totalPassos].x = pt.x;
                    passos[totalPassos].y = pt.y;
                    passos[totalPassos].tempoEspera = tempoDecorrido;
                    passos[totalPassos].botao = botao;
                    printf("Clique registrado: X=%d, Y=%d, Tempo: %lu ms, Botão: %s\n", pt.x, pt.y, tempoDecorrido, botao == 0 ? "Esquerdo" : "Direito");
                }
                ultimoTempo = agora;
            }
            Sleep(300);
        }

        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            printf("Gravação finalizada. Aguardando 5 segundos para iniciar automação...\n");
            recording = 0;
            Sleep(5000);
        }
    }
    
    fclose(arquivo);

    arquivo = fopen("usuarios.txt", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fgets(nomeUsuario, sizeof(nomeUsuario), arquivo); // Ignora a primeira linha

    while (fgets(nomeUsuario, sizeof(nomeUsuario), arquivo)) {
        nomeUsuario[strcspn(nomeUsuario, "\n")] = 0;
        printf("Processando usuário: %s\n", nomeUsuario);

        clickMouse(passos[0].x, passos[0].y, 0); // Clica no campo de inserção
        Sleep(200);
        clearField(); // Limpa o campo
        typeText(nomeUsuario); // Digita o nome
        pressEnter(); // Confirma
        Sleep(500);

        for (int i = 1; i <= totalPassos; i++) { // Executa os cliques gravados
            Sleep(passos[i].tempoEspera);
            clickMouse(passos[i].x, passos[i].y, passos[i].botao);
        }
    }

    fclose(arquivo);
    printf("Automação concluída.\n");
    return 0;
}

void clickMouse(int x, int y, int botao) {
    SetCursorPos(x, y);
    if (botao == 0) {
        mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
    } else {
        mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, 0, 0);
        mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, 0, 0);
    }
}

void typeText(const char* text) {
    for (int i = 0; text[i] != '\0'; i++) {
        SHORT key = VkKeyScan(text[i]);
        keybd_event((BYTE)key, 0, 0, 0);
        keybd_event((BYTE)key, 0, KEYEVENTF_KEYUP, 0);
        Sleep(50);
    }
}

void pressEnter() {
    keybd_event(VK_RETURN, 0, 0, 0);
    keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
}

void clearField() {
    keybd_event(VK_CONTROL, 0, 0, 0);
    keybd_event('A', 0, 0, 0);
    keybd_event('A', 0, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
    Sleep(100);
    keybd_event(VK_BACK, 0, 0, 0);
    keybd_event(VK_BACK, 0, KEYEVENTF_KEYUP, 0);
}