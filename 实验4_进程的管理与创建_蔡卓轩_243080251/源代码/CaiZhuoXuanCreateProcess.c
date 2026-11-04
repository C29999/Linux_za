/* ============================================================
 * CaiZhuoXuanCreateProcess.c
 * 实验4：进程的管理与创建 —— 使用 fork() 创建子进程（父进程 wait 回收）
 * 学号：243080251   姓名：蔡卓轩
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    pid_t childpid;     /* 子进程 PID */
    int   retval;       /* 用户输入的子进程返回值 */
    int   status;       /* 子进程退出状态 */

    childpid = fork();  /* 一次调用，两次返回 */
    if (childpid < 0) {
        perror("fork error!");
        exit(1);
    }

    if (childpid == 0) {
        /* ---------- 子进程 ---------- */
        printf("CHILD: I am the child process!\n");
        printf("CHILD: Here's my PID: %d\n", getpid());
        printf("CHILD: My parent's PID is: %d\n", getppid());
        printf("CHILD: Sleep for 1 second...\n");
        sleep(1);
        printf("CHILD: Enter an exit value(0~255): ");
        fflush(stdout);
        if (scanf("%d", &retval) != 1) {
            retval = 0;
        }
        printf("CHILD: Goodbye!\n");
        exit(retval);   /* 退出码会传送给父进程 */
    } else {
        /* ---------- 父进程 ---------- */
        printf("PARENT: I am the parent process!\n");
        printf("PARENT: Here's my PID: %d\n", getpid());
        printf("PARENT: The value of my child's PID is: %d\n", childpid);
        printf("PARENT: My parent's PID is: %d\n", getppid());
        printf("PARENT: I will now wait for my child to exit.\n");
        wait(&status);  /* 阻塞等待子进程结束 */
        printf("PARENT: Child's exit code is: %d\n", WEXITSTATUS(status));
        printf("PARENT: Goodbye!\n");
        exit(0);
    }
}
