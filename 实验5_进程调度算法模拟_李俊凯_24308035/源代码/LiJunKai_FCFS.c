/* ============================================================
 * 实验5：进程调度算法模拟 —— 先来先服务（FCFS）调度算法
 * 源文件：LiJunKai_FCFS.c
 * 姓名：李俊凯    学号：24308035
 * 说明：在动态优先数调度程序的基础上修改得到。FCFS 算法不再
 *       比较优先数，新进程总是插入就绪队列队尾，调度时从队首
 *       取出进程，先到达的进程先运行。
 * ============================================================ */
#include <stdio.h>
#include <stdlib.h>

#define getpch(type) (type*)malloc(sizeof(type))

struct pcb {                /* 进程控制块（PCB） */
    char name[10];          /* 进程名 */
    char state;             /* 进程状态：'W' 就绪、'R' 运行、'F' 完成 */
    int  nice;              /* 进程优先数，FCFS 中不使用 */
    int  ntime;             /* 进程需要运行的时间 */
    int  rtime;             /* 进程已经运行的时间 */
    struct pcb *link;       /* 链接指针 */
};

typedef struct pcb PCB;
PCB *ready = NULL;          /* 就绪队列队首指针 */
PCB *p = NULL;              /* 当前正在运行的进程指针 */
PCB *last = NULL;           /* 就绪队列队尾指针，便于在队尾插入 */

/* ------------------------------------------------------------
 * sort()：FCFS 算法不排序，新进程直接追加到就绪队列队尾
 * ------------------------------------------------------------ */
int sort(void)
{
    if (ready == NULL) {    /* 队列为空，新进程成为队首 */
        ready = p;
        p->link = NULL;
        last = p;
    } else {                /* 队列不空，追加到队尾 */
        last->link = p;
        last = p;
        p->link = NULL;
    }
    return 0;
}

/* ------------------------------------------------------------
 * input()：输入各进程的参数，建立 PCB 并插入就绪队列
 *   FCFS 不需要优先数，因此注释掉优先数的输入，nice 置 0
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
        /* printf("\n输入进程优先数："); */
        /* scanf("%d", &p->nice); */
        p->nice = 0;        /* FCFS 不使用优先数 */
        printf("\n输入进程运行时间：");
        scanf("%d", &p->ntime);
        printf("\n");
        p->rtime = 0;
        p->state = 'W';
        p->link = NULL;
        sort();                 /* 追加到就绪队列队尾 */
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
 * ------------------------------------------------------------ */
int running(void)
{
    /* FCFS 是非抢占式调度：进程一旦获得 CPU，就一次运行到结束。
     * 为严格体现这一特点，这里直接把已运行时间置为需要运行的时间并撤销进程。
     * 当每个进程的运行时间都为 1 时，输出结果与指导书示例完全一致。 */
    p->rtime = p->ntime;
    destroy();
    return 0;
}

/* ------------------------------------------------------------
 * main()：主函数，按先来先服务的原则循环调度
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

        p = ready;              /* 从队首取出一个进程 */
        ready = p->link;
        if (ready == NULL)      /* 队列已空，队尾指针置空 */
            last = NULL;
        p->link = NULL;
        p->state = 'R';

        check();                /* 查看当前运行进程和就绪队列 */
        running();              /* 运行一个 CPU 时间单位 */
        printf("\n按任意键继续...");
    }

    printf("\n\n所有进程已经运行完成！\n");
    ch = getchar();
    (void)ch;
    return 0;
}
