#include "my_grep.h"
#include "filelist.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

enum{
    FIGNORE_REGISTER = 1,
    FSEEK_WORD = 2
};

static char* pattern = NULL;
static int flags = 0;

static void seek_pattern(FILE* fp);
//allocates new string according to flags, must call free()
static char* makestr_flag(const char* s);

static int find_substr(const char* str, const char* sub);
static int* create_offset_table(const char* s);
static void parse_flag(const char* flag);

void process_arg(const char *arg){
    //output help info
    if(strcmp(arg, "-h") == 0|| strcmp(arg, "--help") == 0 || strcmp(arg, "-help") == 0){
        printf("Usage: my_grep [OPTION]... PATTERNS [FILE]\n");
        exit(EXIT_SUCCESS);
    //parse flag
    }else if(arg[0] == '-'){
        parse_flag(arg);
    //add pattern
    }else if(pattern == NULL){
        pattern = strdup(arg);
    //add filename to the list
    }else{
        flist_add(arg);
    }
}

void grepping(){
    if(pattern == NULL)
        undefined_arg();

    FILE* fp = NULL;
    if(flist_is_empty()){
        fp = stdin;
        seek_pattern(fp);
    }else{
        for(char* fname = flist_get();fname != NULL; fname = flist_get()){
            if((fp = fopen(fname, "r")) == NULL){
                fprintf(stderr, "\"%s\" - failed to open: %s\n", fname, strerror(errno));
                continue;
            }
            seek_pattern(fp);
            fclose(fp);
        }
        flist_free();
    }
}

static void seek_pattern(FILE* fp){
    char buf[BUFSIZ];
    char* fl_pattern = makestr_flag(pattern);
    while(fgets(buf, sizeof(buf), fp)){
        int idx;
        char* s = makestr_flag(buf);
        if((idx = find_substr(s,fl_pattern)) != -1)
            printf("%s", buf);
    }
}

static int find_substr(const char* str, const char* sub){
    const int slen = strlen(str);
    const int plen = strlen(sub);
    
    if(plen == 0)
        return 1;

    int* offset = create_offset_table(sub);
    for(int t = 0; t + plen <= slen;){
        int p = plen-1;

        while(p >= 0 && str[t+p] == sub[p])
            p--;

        if(p == -1)
            return t;

        t += plen - offset[(unsigned char)str[t+p]];
    }
    return -1;
}

static int* create_offset_table(const char* s){
    static int buf[256];
    memset(buf,-1,sizeof(buf));
    for(int i = 0; s[i] != '\0'; i++)
        buf[(unsigned char)s[i]] = i + 1;
    return buf;
}

static char* makestr_flag(const char* s){
    char* res = strdup(s);
    if(flags & FIGNORE_REGISTER){
        for(char* p = res; *p != '\0'; p++)
            *p = tolower(*p);
    }
    return res;
}

static void parse_flag(const char* flag){
    if(strcmp(flag, "-i") == 0){
        flags |= FIGNORE_REGISTER;
    }else{
        eprintf("Undefined argument \"%s\"\n", flag);
    }
}