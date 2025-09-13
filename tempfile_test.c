/*
 * mkstemp() - generates temporary file
 */

#include <stdio.h>
#include "utils.h"
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define DELAY 3

void loud_sleep(void);
void check_file(char *filename);

int main(int argc, char** argv)
{
    char tmpname[] = "/tmp/fileXXXXXX";
    int fd = mkstemp(tmpname);
    if (fd == -1)
        perror_quit("mkstemp()");
    printf("Created temporary file with name %s\n", tmpname);
    check_file(tmpname);
    loud_sleep();
    
/*
 * unlink()  deletes a name from the filesystem.  If that name was the last
 * link to a file and no processes have the file open, the file is  deleted
 * and the space it was using is made available for reuse.
 * 
 * If the name was the last link to a file but any processes still have the
 * file  open,  the  file  will remain in existence until the last file de‐
 * scriptor referring to it is closed.
 * 
 * If the name referred to a symbolic link, the link is removed.
 * 
 * If the name referred to a socket, FIFO, or device, the name  for  it  is
 * removed but processes which have the object open may continue to use it.
 */
    if (unlink(tmpname) == -1)
        perror_quit("unlink()");
    printf("Unlinked %s\n", tmpname);
    check_file(tmpname);
    loud_sleep();

    if (close(fd) == -1)
        perror_quit("close()");
    printf("Closed %s\n", tmpname);
    check_file(tmpname);
    return 0;
}

void loud_sleep(void)
{
    printf("Sleeping %d seconds\n", DELAY);
    sleep(DELAY);
}

void check_file(char *filename)
{
    pid_t pid = fork();
    if (pid == 0) {
        const char *cmd = "/bin/find";
        char *args[] = {(char*)cmd, filename, NULL};
        execv(cmd, args);
        perror_quit("execv");
    } else if (pid == -1) {
        perror_quit("fork");
    } else {
        if (waitpid(pid, NULL, 0) == -1)
            perror_quit("wait()");
    }
}