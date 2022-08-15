// 程序名：myshell
// 作者：吴杰枫 3190100833

// 一些常用的库

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

// 一些常用的常量

#define MAX_PATH_NUMBER 1024
#define MAX_COMMAND_NUMBER 128
#define MAX_NAME_NUMBER 1024
#define MAX_JOBS_NUMBER 1024
#define TRUE 1
#define RED "\e[0;31m"
#define NORMAL "\e[1;37m"
// 用于增加job结构可读性的宏定义
#define FG 0
#define BG 1
#define RUN 0
#define SUSPEND 1
#define DONE 2

// 两个关键的结构

// commandList 链表用于存储命令行
// containString用于存储参数或者命令
// commandCount用于记录命令的参数个数（含命令本身
typedef struct command{
    char *containString;
    int commandCount;
    struct command *next;
}commandList;

// job 结构用于存储任务信息,pid编号，jobname任务名称，type任务类型，status任务状态
typedef struct job{
    pid_t pid;
    char jobname[MAX_NAME_NUMBER];
    int type;
    int status;
}job;


