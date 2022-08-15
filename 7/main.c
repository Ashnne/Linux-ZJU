// 程序名：myshell
// 作者：吴杰枫 3190100833
// 主循环函数

#include "global.h"
#include "main.h"

int main(int argc, char** argv ){

    // 首先判断是否有脚本，并设置int型变量ifScript
    int ifScript = 0;
    // 用于存储脚本文件（如果有
    FILE *script;
    // 用judgeCommand函数判断脚本是否存在
    script = judgeCommand( argc, argv);
    // 初始化信号，后台进程
    initJoblist();
    initSignal();
    // 设置 shell环境变量
    // 获得当前文件夹
    char shell[MAX_PATH_NUMBER];
    getcwd(shell,80);
    // 加上/myshell后缀
    strcat( shell, "/myshell");
    // 调用setenv设置shell环境变量
    setenv( "shell", shell, 1);

    // 开始进入处理命令
    while ( TRUE ){
        // 构建命令行OI交互
        // 初始化命令指针
        initCommandList();
        // 初始化全局变量
        initGlobalVarious();
        // 处理命令行
        // 直接调用readCommand
        commandHead = readCommand( script );
        // 如果没有任何输入,就直接下一个
        if( commandHead == NULL )   continue;
        // 预处理一下命令，把全局变量设置好
        setGlobalVarious();
        // 运行命令
        commandExecute();

    }
}

// 处理命令的全局变量的初始化
void initGlobalVarious(){
    ifPipeExist = 0;
    ifOutputExist = 0;
    ifOutputExist_plus = 0;
    ifInputExist = 0;
    ifBg = 0;
    strcpy( string, "\0");
    strcpy( inputFilePath, "\0");
    strcpy( outputFilePath, "\0");
    strcpy( outputFilePath_Plus, "\0");
}

// 初始化命令链表
void initCommandList(){
    // 只要commandHead未初始化结束,就一直进行下去
    while( commandHead != NULL ){
        // 定义temp用于保存下一个链表
        commandList *temp;
        temp = commandHead->next;
        // 把commandead的内存释放
        free( commandHead );
        // 再用commandHead继承temp里的指针
        commandHead = temp;
    }
    // 同上
    while( commandHead_pipe != NULL ){
        commandList *temp;
        temp = commandHead_pipe->next;
        free( commandHead_pipe );
        commandHead_pipe = temp;
    }
    
}
