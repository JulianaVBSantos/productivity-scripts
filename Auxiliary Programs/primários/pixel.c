#include <windows.h>
#include <stdio.h>

int main() {
    printf("Clique com o mouse para exibir as coordenadas. Pressione ESC para sair.\n");
    
    POINT pt;
    int mouseState = 0;

    while (1) {
        // Verifica se a tecla ESC foi pressionada para sair do loop
        if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
            printf("Tecla ESC pressionada. Finalizando...\n");
            break;
        }

        // Verifica o estado do botão esquerdo do mouse
        if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000) && mouseState == 0) {
            mouseState = 1;  // Marca que o botão foi pressionado
            
            // Obtém as coordenadas do mouse
            if (GetCursorPos(&pt)) {
                printf("Coordenadas do mouse: X=%d, Y=%d\n", pt.x, pt.y);
            }
        } else if (!(GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
            mouseState = 0;  // Reseta o estado quando o botão é solto
        }

        Sleep(50);  // Pequena pausa para evitar sobrecarga do processador
    }

    return 0;
}