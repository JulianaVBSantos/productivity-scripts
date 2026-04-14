// programa para reconhecer quando o botão direito ou esquerdo do mouse for pressionado

#include <windows.h>
#include <stdio.h>

int main() {
    printf("Monitorando cliques do mouse... Pressione CTRL + C para sair.\n");

    while (1) {
        if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
            printf("Botão ESQUERDO clicado!\n");
            while (GetAsyncKeyState(VK_LBUTTON) & 0x8000); // Espera o botão ser solto
        }

        if (GetAsyncKeyState(VK_RBUTTON) & 0x8000) {
            printf("Botão DIREITO clicado!\n");
            while (GetAsyncKeyState(VK_RBUTTON) & 0x8000); // Espera o botão ser solto
        }

        Sleep(50); // Pequeno delay para evitar consumo excessivo de CPU
    }

    return 0;
}
