// 程序名：myshell
// 作者：吴杰枫 3190100833
// 全局变量都在这里定义，需要再调用

#include "global.h"



// 处理命令的全局变量，用于辅助处理
// 用于判断是否存在管道
int ifPipeExist;
// 用于判断是否为后台命令
int ifBg;
// 用于判断是否输出重定向(覆盖
int ifOutputExist;
// 用于判断是否输出重定向(追加
int ifOutputExist_plus;
// 用于判读是否输入重定向
int ifInputExist;

// 用于存储重定向的输入路径
char inputFilePath[MAX_PATH_NUMBER];    
// 用于存储重定向的输出路径（覆盖
char outputFilePath[MAX_PATH_NUMBER];
// 用于存储重定向的输出路径（追加
char outputFilePath_Plus[MAX_PATH_NUMBER];

// 用于存储任务的job结构类型数组
job *jobList;
// 用于存储当前任务数目
int *jobNumber;

// 创建用于读取文件命令行的字符数组,上限用宏定义，数值为128
char string[MAX_COMMAND_NUMBER];
// 用于存储用户的命令的链表头
commandList *commandHead;
// 用于存储管道后半段命令的链表头
commandList *commandHead_pipe;

// 进程停止或者终止时信号的处理结构
struct sigaction sigchldAction;
// 挂起信号的处理结构
struct sigaction sigtstpAction;