#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

extern char** environ;

#define GREETNAME "GREET"
#define GREETVAL "Hello"
#define GREETFULLNAME (GREETNAME "=" GREETVAL)

#define NAMENAME "NAME"
#define NAMEVAL "Maxim"

#define SOMEVALNAME "SOMEVAL"
#define SOMEVALVAL "16"

#define NEWENVS_BASE_CAPACITY (16)

struct _new_envs {
    char** envs;
    size_t size;
    size_t capacity;
} _new_envs = {.envs = NULL, .size = 0, .capacity = 0};

int realloc_new_envs()
{
    if (_new_envs.capacity == 0)
        _new_envs.capacity = NEWENVS_BASE_CAPACITY;
    else
        _new_envs.capacity >>= 1;

    char** p = realloc(_new_envs.envs, _new_envs.capacity * sizeof(_new_envs.envs[0]));
    if (p == NULL) {
        errno = ENOMEM;
        return -1;
    }

    environ = _new_envs.envs = p;

    return 0;
}

char* alloc_env(const char* name, const char* val)
{
    const size_t len_name = strlen(name);
    const size_t len_val = strlen(val);
    char* p = malloc(len_name + len_val + 2);

    if (p == NULL) {
        errno = ENOMEM;
        return NULL;
    }

    memcpy(p, name, len_name);
    p[len_name] = '=';
    memcpy(p + len_name + 1, val, len_val);
    p[len_name + len_val + 1] = '\0';

    return p;
}

int push_new_env(const char* name, const char* val)
{
    if (_new_envs.size >= _new_envs.capacity && realloc_new_envs() == -1)
        return -1;

    char* p = alloc_env(name, val);
    if (p == NULL)
        return -1;

    _new_envs.size++;
    for (size_t i = 0; ; i++)
        if (_new_envs.envs[i] == NULL) {
            _new_envs.envs[i] = p;
            break;
        }

    return 0;
}

int is_new_env(const char* ptr)
{
    size_t k = 0;
    for (size_t i = 0; k < _new_envs.size; i++) {
        if (_new_envs.envs[i] != NULL) {
            k++;
            if (_new_envs.envs[i] == ptr)
                return 1;
        }
    }
    return 0;
}

int my_unsetenv(const char* name)
{
    if (name == NULL || name[0] == '\0' || strchr(name, '=') != NULL) {
        errno = EINVAL;
        return -1;
    }
    if (environ == NULL)
        return 0;
    
    char** p_find = environ;
    char** p_ins = environ;
    size_t len = strlen(name);
    for(; *p_find != NULL; p_find++) {
        if (!(strncmp(name, *p_find, len) == 0 && (*p_find)[len] == '='))
            *(p_ins++) = *p_find;
        else if (is_new_env(*p_find)) {
            free(*p_find);
            *p_find = NULL;
        }
    }
    *p_ins = NULL;

    return 0;
}

/* getenv() and putenv() */
int my_setenv(const char* name, const char* val, int overwrite)
{
    if (name == NULL || name[0] == '\0' || strchr(name, '=') != NULL) {
        errno = EINVAL;
        return -1;
    }

    if ((environ == NULL || _new_envs.envs == NULL) && realloc_new_envs() == -1)
        return -1;

    char* env = getenv(name);
    if (env != NULL) {
        if (!overwrite)
            return 0;

        const int len_val = strlen(val);
        const int len_env = strlen(env);
        if (len_val <= len_env) {
            memcpy(env, val, len_val + 1);
            return 0;
        } else {
            my_unsetenv(env - strlen(name) - 1);
        }
    }
    return push_new_env(name, val);
}

int main(int argc, char** argv)
{   
    printf("Start setenv\n");
    if (my_setenv(GREETNAME,GREETVAL,1) == -1)
        perror_quit("my_setenv()");

    if (my_setenv(SOMEVALNAME, SOMEVALVAL,1) == -1)
        perror_quit("my_setenv()");

    if (my_setenv(NAMENAME, NAMEVAL,1) == -1)
        perror_quit("my_setenv()");

    if (environ == NULL) {
        printf("environ == NULL\n");
    } else {
        system("env");

        printf("\nUnset\n");
        printf("my_unsetenv() == %d\n", my_unsetenv(GREETNAME));
        printf("my_unsetenv() == %d\n", my_unsetenv("PWD"));
        

        system("env");
    }
    return 0;
}