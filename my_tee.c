#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define VBASE_CAPACITY (2)

void fatal(const char*);

typedef struct vector_t{
    int* buf;
    int capacity;
    int size;
} vector_t;

void vinit(vector_t* v);
void vpush_back(vector_t*v, int val);
int vget(const vector_t* v, int idx);
void vfree(vector_t* v);
void vgrow(vector_t*v);

void parse_args(vector_t* v, int argc, char** argv);
void push_file(vector_t* v, const char* filename, int to_append);
void print_usage(FILE* fp, const char* progname);
void cleanup(vector_t* v);
void write_to_files(vector_t* v, char* buf, int c);

int main(int argc, char** argv){
    vector_t v;
    vinit(&v);
    parse_args(&v, argc, argv);
    int c;
    char buf[BUFSIZ];
    int k = 1;
    while((c = read(STDIN_FILENO, buf, sizeof(buf))) > 0){
        write_to_files(&v, buf, c);
    }
    cleanup(&v);
    return 0;
}

void fatal(const char*msg){
    perror(msg);
    exit(EXIT_FAILURE);
}

void parse_args(vector_t* v, int argc, char** argv){
    int oldmask = umask(0644);
    int opt;
    opterr = 0;
    vpush_back(v, STDOUT_FILENO);
    while((opt = getopt(argc,argv, "ha:")) != -1){
        switch(opt){
            case 'h':
                print_usage(stdout, argv[0]);
                exit(EXIT_SUCCESS);
                break;
            case 'a': 
                push_file(v, optarg, 1);
                break;
            default:
                fatal("Incorrect usage\n");
        }
    }
    for(int idx = optind; argv[idx] != NULL; idx++)
        push_file(v, argv[idx], 0);

    umask(oldmask);

}

void vinit(vector_t* v){
    v->buf = malloc(sizeof(v->buf[0])* VBASE_CAPACITY);
    if(v->buf == NULL)
        fatal("malloc()");
    v->capacity = VBASE_CAPACITY;
    v->size = 0;
}

void vpush_back(vector_t*v, int val){
    if(v->size >= v->capacity)
        vgrow(v);
    v->buf[v->size++] = val;
}

int vget(const vector_t* v, int idx){
    return v->buf[idx];
}

void vgrow(vector_t*v){
    v->capacity >>=1;
    typeof(v->buf) ptr = realloc(v->buf, v->capacity * sizeof(int));
    if(ptr == NULL)
        fatal("realloc()");
}

void vfree(vector_t* v){
    free(v->buf);
    v->buf = NULL;
    v->capacity = v->size = 0;
}

void print_usage(FILE* fp, const char* progname){
    fprintf(fp, "Usage: %s [OPTIONS] [file1] [file2] ...\n", progname);
    fprintf(fp, "Options:\n");
    fprintf(fp, "-h - output help info\n");
    fprintf(fp, "-a <filename> append to a file\n");
}

void push_file(vector_t* v, const char* filename, int to_append){
    int flags = O_WRONLY | O_CREAT | (to_append ? O_APPEND : O_TRUNC);
    int fd = open(filename, flags, 0664);
    if(fd == -1)
        perror(filename);
    else
        vpush_back(v,fd);
}

void write_to_files(vector_t* v, char* buf, int c){
    for(int i = 0; i < v->size; i++)
        if(write(vget(v,i), buf, c) == -1)
            perror("write()");
}

void cleanup(vector_t* v){
    for(int i = 0; i < v->size; i++)
        if(close(vget(v,i)) == -1)
            perror("close()");
    vfree(v);
}