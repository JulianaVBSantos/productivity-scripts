//revogar acesso no IAS

#include <windows.h>
#include <stdio.h>
#include <string.h>

void clickMouse(int x, int y);
void typeText(const char* text);
void clearText();

int main() {
    printf("Automação iniciada. Pressione ESC para sair.\n");

    FILE* arquivo = fopen("usuariospendentesosmobile.txt", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }
    printf("Arquivo localizado!\n");

    char nomeUsuario[256]; // Buffer para armazenar cada nome

    while (1) {
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            printf("Tecla ESC pressionada. Finalizando...\n");
            break;
        }

        int isLastUser = 0;
        while (fgets(nomeUsuario, sizeof(nomeUsuario), arquivo)) {
            // Verifica a tecla ESC dentro do loop
            if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
                printf("Tecla ESC pressionada. Finalizando...\n");
                fclose(arquivo);
                return 0;
            }

            nomeUsuario[strcspn(nomeUsuario, "\n")] = 0;

            if (feof(arquivo)) {
                isLastUser = 1;
            }

            printf("Processando usuario: %s\n", nomeUsuario);

            // 3 pontinhos
            clickMouse(868, 164);
            Sleep(500);

            // Filtro
            clickMouse(913, 302);
            Sleep(500);

            // busca
            clickMouse(1023, 435);
            Sleep(500);

            // limpa entrada
            clearText();
            Sleep(500);
            
            // digita user
            typeText(nomeUsuario);
            Sleep(1000);

            // pesquisa
            clickMouse(1023, 440);
            Sleep(800);

            // seleciona
            clickMouse(611, 183);
            Sleep(500);

            // edita
            clickMouse(1210, 137);
            Sleep(1800);
            
            // calendario 
            clickMouse(819, 318);
            Sleep(500);
            /*clickMouse(585, 536);
            Sleep(500);*/

            // seleciona dia 29
            clickMouse(726, 158);
            Sleep(500);

            // ok
            clickMouse(818, 377);
            Sleep(1000);

            clickMouse(624, 631);
            Sleep(1000);

            printf("Usuario %s revogado.\n", nomeUsuario);

            if (isLastUser) {
                printf("Ultimo usuario processado. Finalizando...\n");
                fclose(arquivo);
                return 1; // Finaliza o programa
            }
        }
        Sleep(1000);
    }

    fclose(arquivo); // Fecha o arquivo
    printf("Automacao finalizada.\n");
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