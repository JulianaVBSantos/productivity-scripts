#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_PREFIXOS 19
#define NUM_SUFIXOS 10

const char *prefixos[NUM_PREFIXOS] = {
    "Ahri", "Garen", "Lux", "Zed", "Jinx", "Leona", 
    "Sona", "Riven", "Ekko", "Vayne", "Leesin", 
    "Yasuo", "Thresh", "Ashe", "Miss",
    "Teemo", "Aatrox", "Zilean", "Samira"
};

const char *sufixos[NUM_SUFIXOS] = {
    "fire", "wind", "dark", "ice", "star", "gold", 
    "time", "void", "storm", "doom"
};

void gerar_senha() {
    const char *prefixo = prefixos[rand() % NUM_PREFIXOS];
    const char *sufixo = sufixos[rand() % NUM_SUFIXOS];

    int numero = (rand() % 9) + 1;

    printf("%s%s@%d\n", prefixo, sufixo, numero);
}

int main() {
    int quantidade;
    
    srand(time(NULL));
    
    printf("Quantas senhas você quer gerar? ");
    scanf("%d", &quantidade);
    
    for (int i = 0; i < quantidade; i++) {
        gerar_senha();
    }
    
    return 0;
}
