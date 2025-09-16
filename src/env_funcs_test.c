#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MYENV_NAME "NAME"
#define MYENV_VAL "bob"
#define MYENV_FULLNAME (MYENV_NAME "=" MYENV_VAL)

extern char** environ;

int main(int argc, char** argv)
{
    char* temp = NULL;
    setenv(MYENV_NAME, MYENV_VAL, 0);
    for (char** p = argv + 1; *p != NULL; p++) {
        temp = getenv(*p);
        printf("%s=%s\n", *p, temp == NULL ? "" : temp);
        if (temp != NULL && temp[0] != '\0') {
            temp[0] = '1';
        }
    }

    temp = getenv(MYENV_NAME);
    printf("%s=%s\n", MYENV_NAME, temp == NULL ? "" : temp);
    if (temp != NULL && temp[0] != '\0')
        temp[0] = '1';
    unsetenv(MYENV_NAME);

    printf("After change:\n");
    for (char** p = argv + 1; *p != NULL; p++) {
        temp = getenv(*p);
        printf("%s=%s\n", *p, temp == NULL ? "" : temp);
    }

    temp = getenv(MYENV_NAME);
    printf("%s=%s\n", MYENV_NAME, temp == NULL ? "" : temp);
    return 0;
}