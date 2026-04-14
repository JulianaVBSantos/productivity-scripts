// pra detectar código da tecla pressionada

#include <windows.h>
#include <stdio.h>

int main() {
    printf("Pressione a tecla '/' (ou qualquer outra) para descobrir o código virtual.\n");
    printf("Pressione ESC para sair.\n\n");

    while (1) {
        for (int vk = 1; vk < 256; vk++) {

            if (GetAsyncKeyState(vk) & 0x8000) {
                if (vk == VK_ESCAPE) {
                    printf("Saindo...\n");
                    return 0;
                }

                printf("Tecla pressionada: VK_CODE = 0x%02X (%d)\n", vk, vk);

                while (GetAsyncKeyState(vk) & 0x8000) {
                    Sleep(50);
                }
            }
        }

        Sleep(50); // Evitar o uso excessivo da CPU
    }

    return 0;
}
