// 程序名：myshell
// 作者：吴杰枫 3190100833

// 需要用到的全局变量的声明
extern commandList *commandHead;
extern commandList *commandHead_pipe;
extern job *jobList;
extern int *jobNumber;
extern int ifPipeExist;
extern int ifBg;
extern int ifOutputExist;
extern int ifOutputExist_plus;
extern int ifInputExist;
extern char inputFilePath[MAX_PATH_NUMBER];  
extern char outputFilePath[MAX_PATH_NUMBER];
extern char outputFilePath_Plus[MAX_PATH_NUMBER];

// 需要用到的外部函数的声明
extern void function_cd( commandList * Head, int count );
extern void function_pwd();
extern void function_time();
extern void function_clear();
extern void function_dir( commandList * Head, int count );
extern void readDir( char *dirName );
extern void function_set();
extern void function_echo( commandList * Head, int count );
extern void function_exec( commandList * Head, int count );
extern void function_help();
extern void function_exit();
extern void function_bg();
extern void function_fg();
extern void function_jobs();
extern void function_test( commandList * Head, int count );
extern void function_umask( commandList * Head, int count );
extern job* plusJob( pid_t npid, char *njobname, int ntype, int nstatus);

// 内部函数的声明

// 命令处理主函数commandExecute
void commandExecute();

// 用于commandList格式的exe函数
int listExecvp( commandList * muteList, int count );

//自定的函数的功能实现
// 处理成功就返回1，没有匹配的就返回0
int simple_function( commandList *muteList);

