#include <setjmp.h>
#include <stdio.h>

static jmp_buf env;

void f1(int val)
{
    printf("f1(%d)\n", val);
    longjmp(env, val);
}

void f2(int val)
{
    printf("f2(%d)\n", val);
    f1(val - 1);
}

void f3(int val)
{
    printf("f3(%d)\n", val);
    f2(val - 1);
}

void foo(int val)
{
    if (setjmp(env) == 0) {
        printf("foo(%d) setjmp() == 0\n", val);
    } else {
        printf("foo(%d) cookies!\n", val);
    }
}

int main(void)
{
    switch (setjmp(env)) {
    case 0:
        f3(3);
        break;
    case 1:
        f2(3);
        break;
    case 2:
        f1(3);
        break;
    case 3:
        break;
    default:
        fprintf(stderr, "Undefined value\n");
        return 1;
    }
    printf("done\n");

    foo(99);

    int someval = 11;
    printf("someval = %d\n", someval);
    printf("longjmp()\n");

    longjmp(env, 3);
    return 0;
}