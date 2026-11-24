/* ============================================================
 * 实验5：进程调度算法模拟 —— 动态优先数调度算法
 * 源文件：LiJunKai_schedule.c
 * 姓名：李俊凯    学号：24308035
 * 说明：优先数大者优先占用 CPU；每个进程一次只能运行 1 个
 *        CPU 时间单位，运行后优先数减 1，重新插入就绪队列。
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>

#define getpch(type) (type*)malloc(sizeof(type))

struct pcb {                /* 进程控制块（PCB） */
    char name[10];          /* 进程名 */
    char state;             /* 进程状态：'W' 就绪、'R' 运行、'F' 完成 */
    int  nice;              /* 进程优先数，数值越大优先级越高 */
    int  ntime;             /* 进程需要运行的时间 */
    int  rtime;             /* 进程已经运行的时间 */
    struct pcb *link;       /* 指向就绪队列中下一个进程的指针 */
};

typedef struct pcb PCB;
PCB *ready = NULL;          /* 就绪队列队首指针 */
PCB *p = NULL;              /* 当前正在运行的进程指针 */

/* ------------------------------------------------------------
 * sort()：按优先数从大到小把进程 p 插入就绪队列
 * ------------------------------------------------------------ */
int sort(void)
{
    PCB *first, *second;
    int insert = 0;

    if ((ready == NULL) || (p->nice > ready->nice)) {
        /* 队列为空，或新进程优先数最大，插入队首 */
        p->link = ready;
        ready = p;
    } else {
        /* 在队列中查找插入位置：插到第一个优先数比它小的进程之前 */
        first = ready;
        second = first->link;
        while (second != NULL) {
            if (p->nice > second->nice) {
                p->link = second;
                first->link = p;
                second = NULL;
                insert = 1;
            } else {
                first = first->link;
                second = second->link;
            }
        }
        if (insert == 0)
            first->link = p;   /* 优先数最小，插入队尾 */
    }
    return 0;
}

/* ------------------------------------------------------------
 * input()：输入各进程的参数，建立 PCB 并插入就绪队列
 * ------------------------------------------------------------ */
int input(void)
{
    int i, num;

    printf("\n请输入被调度的进程数目：");
    scanf("%d", &num);

    for (i = 0; i < num; i++) {
        printf("\n---进程号No.%d---", i);
        p = getpch(PCB);
        printf("\n输入进程名：");
        scanf("%s", p->name);
        printf("\n输入进程优先数：");
        scanf("%d", &p->nice);
        printf("\n输入进程运行时间：");
        scanf("%d", &p->ntime);
        printf("\n");
        p->rtime = 0;
        p->state = 'W';
        p->link = NULL;
        sort();                 /* 按优先数插入就绪队列 */
    }
    return 0;
}

/* ------------------------------------------------------------
 * space()：统计就绪队列中的进程个数
 * ------------------------------------------------------------ */
int space(void)
{
    int l = 0;
    PCB *pr = ready;

    while (pr != NULL) {
        l++;
        pr = pr->link;
    }
    return l;
}

/* ------------------------------------------------------------
 * disp()：显示一个进程的信息
 * ------------------------------------------------------------ */
int disp(PCB *pr)
{
    printf("\n qname\tstate\tnice\tndtime\truntime\n");
    printf("  %s\t", pr->name);
    printf(" %c\t", pr->state);
    printf(" %d\t", pr->nice);
    printf(" %d\t", pr->ntime);
    printf(" %d\t", pr->rtime);
    return 0;
}

/* ------------------------------------------------------------
 * check()：查看当前正在运行的进程和当前就绪队列的状态
 * ------------------------------------------------------------ */
int check(void)
{
    PCB *pr;

    printf("\n*********当前正在运行的进程是：%s********", p->name);
    disp(p);

    pr = ready;
    if (pr != NULL)
        printf("\n*************当前就绪队列状态*************");
    else
        printf("\n***********当前就绪队列状态为：空*********\n");

    while (pr != NULL) {
        disp(pr);
        pr = pr->link;
    }
    return 0;
}

/* ------------------------------------------------------------
 * destroy()：进程运行结束，撤销进程
 * ------------------------------------------------------------ */
int destroy(void)
{
    printf("\n进程[%s]已完成。\n", p->name);
    free(p);
    return 0;
}

/* ------------------------------------------------------------
 * running()：进程运行一个 CPU 时间单位
 *   运行后已运行时间加 1；若已达到需要运行的时间则撤销进程，
 *   否则优先数减 1，重新插入就绪队列。
 * ------------------------------------------------------------ */
int running(void)
{
    (p->rtime)++;
    if (p->rtime == p->ntime) {
        destroy();
    } else {
        (p->nice)--;
        p->state = 'W';
        sort();
    }
    return 0;
}

/* ------------------------------------------------------------
 * main()：主函数，循环进行进程调度
 * ------------------------------------------------------------ */
int main(void)
{
    int len, h = 0;
    char ch;

    input();
    len = space();

    while ((len != 0) && (ready != NULL)) {
        ch = getchar();         /* 按任意键继续 */
        (void)ch;
        h++;
        printf("\n The execute number is %d\n", h);

        p = ready;              /* 从就绪队列队首取出一个进程 */
        ready = p->link;
        p->link = NULL;
        p->state = 'R';         /* 置为运行状态 */

        check();                /* 查看当前运行进程和就绪队列 */
        running();              /* 运行一个 CPU 时间单位 */
        printf("\n按任意键继续...");
    }

    printf("\n\n所有进程已经运行完成！\n");
    ch = getchar();
    (void)ch;
    return 0;
}
