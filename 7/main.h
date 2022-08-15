// 程序名：myshell
// 作者：吴杰枫 3190100833

// 一些常用到的全局变量

// 需要用到的外部变量的声明
extern commandList *commandHead;
extern commandList *commandHead_pipe;
extern int ifPipeExist;
extern int ifBg;
extern int ifOutputExist;
extern int ifOutputExist_plus;
extern int ifInputExist;
extern char string[MAX_COMMAND_NUMBER];
extern char inputFilePath[MAX_PATH_NUMBER];  
extern char outputFilePath[MAX_PATH_NUMBER];
extern char outputFilePath_Plus[MAX_PATH_NUMBER];

// 需要用到的外部函数的声明
extern FILE *judgeCommand( int argc, char ** argv);
extern void initJoblist();
extern void initSignal();
extern commandList *readCommand( FILE *script );
extern void setGlobalVarious();
extern void commandExecute();

// 内部函数的声明
// 初始化命令链表
void initCommandList();

// 处理命令的全局变量的初始化
void initGlobalVarious();
