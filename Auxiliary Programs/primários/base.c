// codigo base para executar ações com o mouse e teclado

#include <windows.h>
#include <stdio.h>

// Função para mover o mouse para uma posição e clicar
void clickMouse(int x, int y) {
    SetCursorPos(x, y); // Move o mouse para as coordenadas (x, y)
    mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0); // Clique esquerdo pressionado
    mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);   // Clique esquerdo solto
}

// Função para digitar texto
void typeText(const char* text) {
    for (int i = 0; text[i] != '\0'; i++) {
        SHORT key = VkKeyScan(text[i]); // Mapeia o caractere para o código da tecla
        keybd_event((BYTE)key, 0, 0, 0); // Pressiona a tecla
        keybd_event((BYTE)key, 0, KEYEVENTF_KEYUP, 0); // Solta a tecla
        Sleep(50); // Atraso para simular digitação
    }
}

int main() {
    Sleep(2000); // Espera 2 segundos antes de começar (para dar tempo de trocar de janela)

    clickMouse(500, 500); // Move e clica no ponto (500, 500)
    Sleep(500); // Pausa entre ações
    typeText("Hello, World!"); // Digita "Hello, World!"

    return 0;
}