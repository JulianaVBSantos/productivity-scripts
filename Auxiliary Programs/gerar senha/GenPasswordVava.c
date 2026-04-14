// Gerar senhas com no mín 12 caracteres e no máx 16

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define NUM_PREFIXOS 46
#define NUM_SUFIXOS 16
#define TAMANHO_MAX_SENHA 16
#define TAMANHO_MIN_SENHA 12

const char *prefixos[NUM_PREFIXOS] = {
    "Phoenix", "Iso", "Harbor", "Chamber", "Vyse",
    "Deadlock", "Fade", "KAY/O", "Clove", "Gekko",
    "Neon", "Astra", "Yoru", "Reyna", "Skye", "Brimstone",
    "Cypher", "Killjoy", "Viper", "Sova", "Jett", "Breach",
    "Sage", "Raze", "Omen", "Tejo", "Waylay", "Ahri", "Garen", 
    "Lux", "Zed", "Jinx", "Leona",
    "Sona", "Riven", "Ekko", "Vayne", "Leesin",
    "Yasuo", "Thresh", "Ashe", "Miss",
    "Teemo", "Aatrox", "Zilean", "Samira"};

const char *sufixos[NUM_SUFIXOS] = {
    "Fire", "Wind", "Dark", "Ace", "Star", "Gold",
    "Time", "Void", "Storm", "Smoke", "Spike", "Ultimatum",
    "Flash", "Defuse", "Lockdown", "Doom"};

void gerar_senha()
{
    const char *prefixo = prefixos[rand() % NUM_PREFIXOS];
    const char *sufixo = sufixos[rand() % NUM_SUFIXOS];
    int numero = (rand() % 9) + 1;
    const int tamanho_num = 3;

    int tam_disp = TAMANHO_MAX_SENHA - tamanho_num;

    int max_prefix_len = strlen(prefixo);
    int max_sufix_len = strlen(sufixo);

    while (max_prefix_len + max_sufix_len > tam_disp)
    {
        if (max_prefix_len > max_sufix_len)
            max_prefix_len--;
        else
            max_sufix_len--;
    }

    char prefixo_cortado[20], sufixo_cortado[20];
    strncpy(prefixo_cortado, prefixo, max_prefix_len);
    prefixo_cortado[max_prefix_len] = '\0';
    strncpy(sufixo_cortado, sufixo, max_sufix_len);
    sufixo_cortado[max_sufix_len] = '\0';

    char senha[50];
    snprintf(senha, sizeof(senha), "%s%s@%d", prefixo_cortado, sufixo_cortado, numero);

    int tam_atual = strlen(senha);
    while (tam_atual < TAMANHO_MIN_SENHA)
    {
        char extra = 'A' + (rand() % 26);
        senha[tam_atual++] = extra;
        senha[tam_atual] = '\0';
    }

    printf("%s\n", senha);
}

int main()
{
    int quantidade;

    srand(time(NULL));

    printf("Quantas senhas a senhora quer gerar? ");
    scanf("%d", &quantidade);

    for (int i = 0; i < quantidade; i++)
    {
        gerar_senha();
    }

    return 0;
}
