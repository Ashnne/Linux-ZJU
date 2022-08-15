// 程序名：myshell
// 作者：吴杰枫 3190100833

// 以下为需要用到的外部变量的声明
extern commandList *commandHead;
extern commandList *commandHead_pipe;
extern job *jobList;
extern int *jobNumber;
extern int ifPipeExist;
extern int ifInputExist;
extern int ifOutputExist;
extern int ifOutputExist_plus;
extern int ifBg;
extern char string[MAX_COMMAND_NUMBER];
extern char inputFilePath[MAX_PATH_NUMBER];
extern char outputFilePath[MAX_PATH_NUMBER];
extern char outputFilePath_Plus[MAX_PATH_NUMBER];

// 以下为需要用到的函数的声明
extern int function_exit();

// 以下为文件内函数的定义

// 该函数用于显示命令提示符
// 提示符格式为 [root@localhost ~]$
//　root：当前登录用户名
//  localhost：当前主机名
//  ~：当前所在路径
//　$:表示普通用户
void function_showPrompt();

// commandHandle函数
// 读入一个包含命令的字符串，命令之间以“ ”为分割，返回一个命令链表
// 需要string.h库和commandList结构
// 测试出来的注意事项，输入的需要是字符数组，不是指针，不然就会报错
commandList *commandHandle( char *commandString );

// 判断是否读入脚本，若有，便返回，没有则返回空指针
FILE *judgeCommand( int argc, char ** argv);

// 读取命令行操作，读取脚本文件中的一行命令并输出链表命令
// 若无脚本则读取输入字符串
// 并将命令行转化为commandList链表
commandList * readCommand( FILE * script );

// setGlobalVarious函数，在处理完命令后，对命令进行初步处理，调整好全局变量
void setGlobalVarious();

// initJoblist函数用于初始化后台进程表
// 后台进程表用共享内存以加速各进程间的联络交流
void initJoblist();

// plusJob函数用于添加新的任务到最近的一个数组格子中
// 返回值是新增加的任务的结构指针
job* plusJob( pid_t npid, char *njobname, int ntype, int nstatus);

// deleteJob函数用于删除指定pid的任务
void deleteJob( pid_t pid);


commandList * getCommand();

// command函数
// 读入一个包含命令的字符串，命令之间以“ ”为分割，返回一个命令链表
// 需要string.h库和commandList结构
commandList *commandHandle( char *commandString);

// 判断是否读入脚本，若有，便返回
FILE *judgeCommand( int argc, char ** argv);



// initJoblist函数用于初始化后台进程表
// 后台进程表用共享内存以加速各进程间的联络交流
void initJoblist();


// deleteJob函数用于删除指定pid的任务
void deleteJob( pid_t pid);

