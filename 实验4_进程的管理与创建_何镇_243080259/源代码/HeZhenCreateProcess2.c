/* ============================================================
 * HeZhenCreateProcess2.c
 * 实验4（5.6 选做）：子进程用 exec 族函数加载新的程序映像（ls）
 * 学号：243080259   姓名：何镇
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t childpid;
    int   status;

    childpid = fork();
    if (childpid < 0) {
        perror("fork error!");
        exit(1);
    }

    if (childpid == 0) {
        /* 子进程：用 execlp 把自身映像替换为 ls 命令 */
        execlp("ls", "ls", "-l", (char *)NULL);
        perror("execlp error!");   /* 只有失败才会执行到这里 */
        exit(1);
    } else {
        printf("PARENT: I am the parent process!\n");
        printf("PARENT: Here's my PID: %d\n", getpid());
        printf("PARENT: The value of my child's PID is: %d\n", childpid);
        printf("PARENT: I will now wait for my child to exit.\n");
        wait(&status);
        printf("PARENT: Child has exited, exit code = %d\n", WEXITSTATUS(status));
        printf("PARENT: Goodbye!\n");
        exit(0);
    }
}
