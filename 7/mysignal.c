// 程序名：myshell
// 作者：吴杰枫 3190100833
// 信号系统

#include "global.h"
#include "mysignal.h"




// 信号系统初始化函数
void initSignal(){
    // 首先将sigchldAction变量初始化一下
    memset( &sigchldAction, 0, sizeof(sigchldAction));
    // 设置sigchldAction的sa_flags
    // 设置SA_RESTART使该信号中断的系统调用可以自动重启动
    // 设置SA_SIGINFO增加附加信息，使其可以sa_sigactio处理信号
    sigchldAction.sa_flags = SA_SIGINFO | SA_RESTART;
    // 设置sigchidAction的信号处理函数为写好的function_sigchid
    sigchldAction.sa_sigaction = function_sigchld;
    // 重置sigchidAction的sa_mask
    sigemptyset( &sigchldAction.sa_mask );
    // sigchidAction变量初始化完毕，将其作为SIGCHLD信号的处理方式
    // 调用sigaction函数将SIGCHLD信号的处理方式设置为sigchldAction
    // 最后一个参数设空，不想知道该信号的上一个动作
    sigaction( SIGCHLD, &sigchldAction, NULL);

    // 把function_sigtstp函数赋给SIGTSTP和SIGSTOP信号作为处理函数
    // 这里大部分都是照着UNIX参考书来写的
    // 首先将sigtstpAction变量初始化一下
    memset( &sigtstpAction, 0, sizeof(sigtstpAction));
    // 将函数赋给变量sigtstpAction
    sigtstpAction.sa_handler = function_sigtstp;
    // 重置sa_mask
    sigemptyset( &sigtstpAction.sa_mask );
    // 设置sigtstpAction的sa_flags
    // 仅仅希望因为这个信号中断后可以自动重启动就行
    sigtstpAction.sa_flags |= SA_RESTART;
    // 用sigaction函数将function_sigtstp函数赋给SIGTSTP和SIGSTOP信号作为处理函数
    sigaction( SIGTSTP, &sigtstpAction, NULL);
    sigaction( SIGSTOP, &sigtstpAction, NULL);
}

// function_sigchld函数，因为调用SA_SIGINFO标志，因此以void handler( int signo， siginfo_t *info, void *context );的形式接受信号
// 该信号表示一个进程终止或停止，如果是后台进程需要更新jobList，否则就可以直接删除该记录
void function_sigchld( int signo, siginfo_t *info, void *context ){
    // 首先保存好返回信号进程的pid用于查找，全局变量jobNumber便于使用
    pid_t pid = info->si_pid;
    int sumNumber = *jobNumber;
    // 查找在job List中的进程
    int location;
    for( location=0; location<sumNumber; location++){
        if( pid == jobList[location].pid ) break;
    }

    // 判断是否找到相应pid的任务
    if( location == sumNumber ){
        // 未找到,那就算了
        return;
    }
    // 找到了
    else{
        // 根据找到的进程类型进行不同的处理
        // 如果是正在运行的后台程序，把状态调整为DONE，先不删除
        if( jobList[location].type == BG && jobList[location].status == RUN )
            jobList[location].status = DONE;
        // 如果是暂停的后台进程，就不做任何处理
        else if( jobList[location].type == BG && jobList[location].status == SUSPEND )
            ;
        // 如果不是以上情况，说明是前台进程结束，直接删除就好
        else
            deleteJob( pid );
    }
}


// function_sigtstp函数,接到信号后，将最近一个前台的进程挂到后台去
// 因为未调用SA_SIGINFO标志，因此以void handler( int signo );的形式接受信号
void function_sigtstp( int signo ){
    // 学长的code里说接收到这个信号会输出^z，因此先输出个换行符
    printf("\n");
    // 测试点,挂起失灵了
    // printf("get the sign!");
    // 常规保存jobNumber和查找进程(最近一个前台进程，不包括主进程)
    int sumNumber = *jobNumber;
    int location;
    for( location=sumNumber-1; location>0; location--){
        // 测试点
        // printf("location:%d type:%d",location, jobList[location].type);
        if( jobList[location].type == FG )  break;
    }

    // 判断是否找到,找到了就挂起，没找到就算逑，结束函数
    if( location>0 ){
        // 在jobList全局变量中改变
        jobList[location].status = SUSPEND;
        jobList[location].type = BG;
        // 用kill函数向该进程发送信号
        kill( jobList[location].pid, SIGSTOP );
    }
}
