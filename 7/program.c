// 程序名：myshell
// 作者：吴杰枫 3190100833
// 命令处理程序

#include "global.h"
#include "program.h"

// 命令处理主函数commandExecute
void commandExecute(){
    // 先备份标准输入输出文件符，最后要改回来
    int stdinFd = dup(fileno(stdin));
    int stdoutFd = dup(fileno(stdout));

    // quit命令同就是exit函数，直接调用就好了
    if( strcmp( commandHead->containString, "quit" ) == 0 ){
        function_exit();
    }

    // 管道操作
    if( ifPipeExist == 1 ){
        // 测试点(通过)
        // printf("pipe begin\n");
        // 先定义管道两段的文件描述符
        int pipeFile[2];
        // 创建管道，并将管道两端的文件描述符保存到pipeFile中，失败则错误退出
        if( pipe( pipeFile ) == -1 ){
            fprintf(stderr, RED "[myshell] Error: Create pipe failed !\n");
            exit(1);
        }
        // 测试点(通过,成功创建)
        // printf( "pipeFile is %d %d \n", pipeFile[0], pipeFile[1]);

        // 调用fork函数创建子进程，并以pidTemp作为返回值来判断
        pid_t pidTemp;
        pidTemp = fork();

        // 这段管道重定向的基本上在UNIX的辅导书里可以抄例子
        // 在子进程中
        if( pidTemp == 0 ){
            // 测试点
            // fprintf(stderr,"管道左端开始创建!");
            // 先把标准输出重定向到管道输入，变成向管道输入
            dup2( pipeFile[1], STDOUT_FILENO );
            // 把管道输出关闭
            close( pipeFile[0]);
            // 调用simple_function函数来读取命令，看是否为内部命令
            // 返回值为1表示读取成功，否则需要继续处理
            if( simple_function( commandHead ) == 1 ){
                // 用listExecvp处理命令
                // 如果返回值不是0，说明运行失败，报错
                if( listExecvp( commandHead, commandHead->commandCount ) != 0)
                    fprintf(stderr, RED "[myshell] Error: Can not find the command named \"%s\"!\n", commandHead->containString );
            }
            // 测试点
            // char temp[100];
            // fgets(temp,100,pipeFile[1]);
            // puts(temp);
            // 退出子进程
            exit(0);
        }
        
        // 在主进程中
        else if( pidTemp > 0){
            // 调用waitpid函数等待管道左端输入完毕
            waitpid( pidTemp, NULL, 0);
            // 测试点
            // fprintf(stderr,"主进程回归");
            // 创造管道右端的进程
            pid_t pidIn;
            pidIn = fork();

            // 测试点(读入没问题)
            // char temp[100];
            // int n;
            // n = read( pipeFile[0], temp, 100);
            // 在右端的子进程中
            if( pidIn == 0 ){
                // 测试点
                // fprintf(stderr,"管道右端开始创建!");
                // 和上述操作差不多，把管道输出端链接到标准输入
                // 关闭管道的输入端
                close( pipeFile[1] );
                // 把输出端重定向
                dup2( pipeFile[0], STDIN_FILENO );
                // 调用命令和上述意义
                if(  simple_function( commandHead_pipe ) == 1 ){
                    // 用listExecvp函数处理命令
                    // 如果返回值不是0，说明运行失败，报错
                    // 测试点
                    fprintf(stderr,"开始用外部");
                    if( listExecvp( commandHead_pipe, commandHead_pipe->commandCount ) != 0)
                        fprintf(stderr, RED "[myshell] Error: Can not find the command named \"%s\"!\n", commandHead_pipe->containString ) ;
                }
                // 测试点
                fprintf(stderr,"运行结束");
                // 退出子进程
                exit(0);
            }

            // 在主进程中
            else{
                // 用waitpid函数等待子进程结束
                waitpid( pidIn, NULL, 0);
                // 关闭管道的两端
                close( pipeFile[0] );
                close( pipeFile[1] );
            }

        }

        // 结束
        return;
    }

    // 如果包含了输入重定向
    if( ifInputExist == 1 ){
        // 将对应的文件打开，标识符存到inputFile中
        // O_RDONLY代表只读取
        int inputFile = open( inputFilePath, O_RDONLY, 0666);
        // 将标准输入重定向，失败就报错
        // 用fileno函数把文件流指针转换成文件描述符
        if( dup2( inputFile, fileno( stdin ) ) == -1){
            fprintf(stderr,RED "[myshell] Error: Change stdin failed!\n");
        }
        // 关闭文件
        close( inputFile );
    }

    // 如果包含了输出重定向（覆盖
    if( ifOutputExist == 1 ){
        // 将对应的文件打开，标识符存到outputFile中
        // O_RDWR表示读写打开，O_TRUNC表示存在则刷新，O_CREAT表示不存在则创建
        int outputFile = open( outputFilePath, O_RDWR | O_CREAT | O_TRUNC, 0666);
        // 将标准输出重定向，失败就报错
        // 用fileno函数把文件流指针转换成文件描述符
        if( dup2( outputFile, fileno( stdout ) ) == -1){
            fprintf(stderr,RED "[myshell] Error: Change stdout failed!\n");
        }
        // 关闭文件
        close( outputFile );
    }

    // 如果包含了输出重定向（追加
    if( ifOutputExist_plus == 1 ){
        // 将对应的文件打开，标识符存到outputFile中
        // O_RDWR表示读写打开，O_APPEND表示追加，O_CREAT表示不存在则创建
        int outputFile = open( outputFilePath_Plus, O_RDWR | O_CREAT | O_APPEND, 0666);
        // 将标准输出重定向，失败就报错
        // 用fileno函数把文件流指针转换成文件描述符
        if( dup2( outputFile, fileno( stdout ) ) == -1){
            fprintf(stderr,RED "[myshell] Error: Change stdout failed!\n");
        }
        // 关闭文件
        close( outputFile );
    }

    // 如果不存在管道，则直接进行判断
    // 首先用simple_function确定是否为内部函数，如不是，则调用listExecvp
    if( simple_function( commandHead ) == 1 ){
        // 如果不是内部命令就用listExecvp调用新进程运行
        // 创建新进程
        pid_t pid = fork();

        // 在子进程中
        if( pid == 0 ){
            // 用listExecvp运行程序
            // 出错就报错
            // 添加parent环境变量
            char shell_[MAX_PATH_NUMBER];
            getcwd(shell_,80);
            // 加上/myshell后缀
            strcat( shell_, "/myshell");
            // 调用setenv设置parent环境变量
            setenv( "parent", shell_, 1);
            if(  listExecvp( commandHead ,commandHead->commandCount ) != 0){ 
                fprintf(stderr, RED "[myshell] Error: Not find the  command \"%s\"!\n", commandHead->containString );
            }
            // 退出进程
            exit(0);
        }

        // 在主进程中
        else if( pid > 0 ){ 
            // 如果是后台命令
            if( ifBg == 1 ){
                // 将其加到任务栏中
                plusJob( pid, commandHead->containString, BG, RUN );
                // 调用waitpid函数，但是用WNOHANG参数不阻塞主进程
                waitpid( pid, NULL, WNOHANG);
                // 把在后台的排序输出
                // 计数后台
                int bgCount = 0;
                // 把全局变量jobNumber拿来用
                int sum = *jobNumber;
                for( int i=0; i<sum; i++ ){
                    if( jobList[i].status == BG )
                        bgCount++;
                }
                printf("[%d] %d\n", bgCount+1, pid );
            }
            else{
                // 测试点,暂停信号失灵了,看看这边是否正确录入
                // printf("%s is plsing!", commandHead->containString );   
                // 将其加到任务栏中
                plusJob( pid, commandHead->containString, FG, RUN );
                // 等一等子进程,要用WUNTRACED选项,再进程暂停时返回
                waitpid( pid, NULL, WUNTRACED);
            }
        }
    }

    // 最后对重定向的标准输入输出进行复原
    // 输入重定向
    if( ifInputExist == 1){
        // 复原
        dup2( stdinFd, fileno(stdin));
        // 关闭文件
        close( stdinFd );
    }

    // 输出重定向
    if( ifOutputExist || ifOutputExist_plus ){
        // 复原
        dup2( stdoutFd, fileno(stdout) );
        // 关闭文件
        close( stdoutFd );
    }

    // 结束
    return;
}

// 用于commandList格式的exe函数
int listExecvp( commandList * muteList, int count ){
    // 首先定义用于保存命令行的字符指针数组，便于使用execvp函数
    char *commands[count+1];
    // 用count和muteList来提取命令行
    for( int i=0; i<count; i++ ){
        commands[i] = muteList->containString;
        muteList = muteList->next;
    }
    // 在结尾加上NULL方便execvp函数使用
    commands[count] = NULL;
    // 调用execvp函数并返回函数返回值
    return execvp( commands[0], commands );
}

//自定的函数的功能实现
// 处理成功就返回1，没有匹配的就返回0
int simple_function( commandList *muteList){
    // 把命令提取出来方便使用
    char commandTemp[MAX_COMMAND_NUMBER];
    strcpy( commandTemp, muteList->containString );


    if( strcmp( commandTemp, "cd") == 0 )
        function_cd( muteList, muteList->commandCount );
    else if( strcmp( commandTemp, "pwd") == 0 )
        function_pwd();
    else if( strcmp( commandTemp, "time") == 0 )
        function_time();
    else if( strcmp( commandTemp, "clr") == 0 || strcmp( commandTemp, "clean") == 0 )
        function_clear();
    else if( strcmp( commandTemp, "dir") == 0)
        function_dir( muteList, muteList->commandCount );
    else if( strcmp( commandTemp, "set") == 0 )
        function_set();
    else if( strcmp( commandTemp, "echo") == 0 )
        function_echo( muteList, muteList->commandCount );
    else if( strcmp( commandTemp, "exec") == 0)
        function_exec( muteList, muteList->commandCount );
    else if( strcmp( commandTemp, "help") == 0 )
        function_help();
    else if( strcmp( commandTemp, "exit") == 0 )
        function_exit();
    else if( strcmp( commandTemp, "bg") == 0 )
        function_bg();
    else if( strcmp( commandTemp, "fg") == 0 )
        function_fg();
    else if( strcmp( commandTemp, "jobs") == 0 )
        function_jobs();
    else if( strcmp( commandTemp, "test") == 0 )
        function_test( muteList, muteList->commandCount );
    else if( strcmp( commandTemp, "umask") == 0 )
        function_umask( muteList, muteList->commandCount );
    // 如果没有任何处理就返回1
    else
        return 1;
    // 如果处理成功就返回0
    return 0;
}