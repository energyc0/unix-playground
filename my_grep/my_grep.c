#include <unistd.h>
#include "my_grep.h"

int main(int argc, char** argv){
    if(argc == 1){
        eprintf("Usage: my_grep [OPTION]... PATTERNS [FILE]\nTry my_grep --help for more information\n");
    }
    for(int i = 1; i < argc; i++){
        process_arg(argv[i]);
    }
    grepping();

    return 0;
}