// 程序名：myshell
// 作者：吴杰枫 3190100833

// 需要用到的全局变量的声明
extern int *jobNumber;
extern job *jobList;
extern struct sigaction sigchldAction;
extern struct sigaction sigtstpAction;

// 需要用到的外部函数的声明
extern void deleteJob( pid_t pid );

// 内部函数的声明

// 信号系统初始化函数
void initSignal();

// function_sigchld函数，因为调用SA_SIGINFO标志，因此以void handler( int signo， siginfo_t *info, void *context );的形式接受信号
// 该信号表示一个进程终止或停止，如果是后台进程需要更新jobList，否则就可以直接删除该记录
void function_sigchld( int signo, siginfo_t *info, void *context );

// function_sigtstp函数,接到信号后，将最近一个前台的进程挂到后台去
// 因为未调用SA_SIGINFO标志，因此以void handler( int signo );的形式接受信号
void function_sigtstp( int signo );