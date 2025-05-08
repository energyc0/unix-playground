#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>

#define eprintf(...) do{ \
    fprintf(stderr, __VA_ARGS__ "\n");\
    exit(EXIT_FAILURE); \
}while(0)

#define errnoprint(msg) do{ \
    perror(msg);\
    exit(EXIT_FAILURE);\
}while(0)

static void tracer_func(pid_t pid);
static void tracee_func(char** cmd);

int main(int argc, char** argv){
    if(argc != 2)
        eprintf("Command expected");

    pid_t pid = fork();
    if(pid == -1)
        errnoprint("fork()");
    else if(pid == 0)
        tracee_func(argv + 1);
    else
        tracer_func(pid);
    return 0;
}

void tracer_func(pid_t pid){
    printf("pid is %u\n", pid);

    if(waitpid(pid, 0, 0)  < 0 || ptrace(PTRACE_SETOPTIONS, pid, 0, PTRACE_O_EXITKILL) < 0)
        eprintf("something went wrong...");

    while (1) {
        struct user_regs_struct regs;
        int status;

        if(ptrace(PTRACE_SYSCALL, pid, NULL, NULL) < 0)
            errnoprint("ptrace()");
        if(waitpid(pid, &status, 0)  < 0)
            eprintf("waitpid()");

        if(ptrace(PTRACE_GETREGS, pid, NULL, &regs) < 0)
            errnoprint("ptrace()");
        printf("[ %ld ] (%ld, %ld, %ld, %ld, %ld, %ld)",
             (long)regs.orig_rax, (long)regs.rdi, (long)regs.rsi, (long)regs.rdx, (long)regs.r10, (long)regs.r9, (long)regs.r8);

        if(ptrace(PTRACE_SYSCALL, pid, NULL, NULL) < 0)
            errnoprint("ptrace()");
        if(waitpid(pid, &status, 0)  < 0)
            eprintf("waitpid()");
        if(WIFEXITED(status)){
            putchar('\n');
            break;
        }

        if(ptrace(PTRACE_GETREGS, pid, NULL, &regs) < 0)
            errnoprint("ptrace()");
        printf(" = %ld\n", (long)regs.rax);
    }
}
void tracee_func(char** cmd){
    if(ptrace(PTRACE_TRACEME)  < 0)
        errnoprint("ptrace()");
    execvp(cmd[0], cmd);
    errnoprint("execvp()");
}