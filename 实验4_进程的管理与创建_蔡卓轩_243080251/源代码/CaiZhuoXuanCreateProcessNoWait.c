/* ============================================================
 * CaiZhuoXuanCreateProcessNoWait.c
 * 实验4（5.5）：去掉父进程的 wait()，观察孤儿进程现象
 * 学号：243080251   姓名：蔡卓轩
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

    childpid = fork();
    if (childpid < 0) {
        perror("fork error!");
        exit(1);
    }

    if (childpid == 0) {
        /* ---------- 子进程 ---------- */
        printf("CHILD: I am the child process!\n");
        printf("CHILD: Here's my PID: %d\n", getpid());
        printf("CHILD: Sleep for 3 seconds...\n");
        sleep(3);   /* 父进程在这 3 秒内已经退出 */

        /* 此时父进程已结束，子进程被 init/systemd 收养，PPID 变为 1 */
        printf("CHILD: I wake up, my parent's PID is: %d\n", getppid());
        printf("CHILD: Enter an exit value(0~255): ");
        fflush(stdout);
        int retval = 0;
        scanf("%d", &retval);
        printf("CHILD: Goodbye!\n");
        exit(retval);
    } else {
        /* ---------- 父进程（没有 wait） ---------- */
        printf("PARENT: I am the parent process!\n");
        printf("PARENT: Here's my PID: %d\n", getpid());
        printf("PARENT: The value of my child's PID is: %d\n", childpid);
        printf("PARENT: I will NOT wait for my child, I exit now!\n");
        printf("PARENT: Goodbye!\n");
        exit(0);    /* 父进程直接退出，不再 wait 子进程 */
    }
}
