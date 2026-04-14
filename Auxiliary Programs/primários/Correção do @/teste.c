#include <stdio.h>
#include <string.h>

int main () {
    char a = '@';
    printf("var a: %c\n", a);

    scanf(&a);
    printf("var a att: %c\n", a);

    printf("@@@\n");

    return 0;
}