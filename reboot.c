#include <linux/reboot.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <unistd.h>

int main(void)
{
    syscall(SYS_reboot, LINUX_REBOOT_MAGIC1, LINUX_REBOOT_MAGIC2B, LINUX_REBOOT_CMD_RESTART, NULL);
    perror("Failed to reboot the system!");
    return 1;
}