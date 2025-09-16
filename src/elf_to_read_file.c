#include <stdio.h>
#include <stdlib.h>

char globBuf[65536];
int primes[] = { 2, 3, 5, 7 };

static int square(int x)
{
    int result;
    result = x * x;
    return result;
}

/* Сегмент неинициализированных данных */
/* Сегмент инициализированных данных */
/* Размещается в фрейме для square() */
/* Размещается в фрейме для square() */
/* Возвращаемое значение передается через регистр */

static void doCalc(int val)
/* Размещается в фрейме для doCalc() */
{
    printf("The square of %d is %d\n", val, square(val));
    if (val < 1000) {
        int t;
        /* Размещается в фрейме для doCalc() */
        t = val * val * val;
        printf("The cube of %d is %d\n", val, t);
    }
}
int main(int argc, char* argv[])
/* Размещается в фрейме для main() */
{
    static int key = 9973;
    /* Сегмент инициализированных данных */
    static char mbuf[10240000];
    /* Сегмент неинициализированных данных */
    char* p;
    /* Размещается в фрейме для main() */
    p = malloc(1024);
    /* Указывает на память в сегменте кучи */
    doCalc(key);
    exit(EXIT_SUCCESS);
}