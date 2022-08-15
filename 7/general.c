// 程序名：myshell
// 作者：吴杰枫 3190100833

// 此文件用于存储通用函数

#include "global.h"
#include "general.h"




// 该函数用于显示命令提示符
// 提示符格式为 [root@localhost ~]$
//　root：当前登录用户名
//  localhost：当前主机名
//  ~：当前所在路径
//　$:表示普通用户
void function_showPrompt(){
    // 定义三个变量预备
    char root[MAX_NAME_NUMBER];
    char localhost[MAX_NAME_NUMBER];
    char path[MAX_NAME_NUMBER];
    // 用getcwd得到当前所在路径
    getcwd( path, MAX_NAME_NUMBER );
    // 用gethostname函数得到当前主机名
    gethostname( localhost, MAX_NAME_NUMBER );
    // 声明一个passwd结构变量来获得含有用户名的passwd结构
    struct passwd *temp;
    // 用getuid得到用户的uid，再利用用户的uid得到用户信息
    temp = getpwuid( getuid());
    // 把temp中的用户名信息部分存到root中
    strcpy( root, temp->pw_name );
    // 输出提示符
    printf(NORMAL "[%s@%s %s]$", root, localhost, path );
}

// commandHandle函数
// 读入一个包含命令的字符串，命令之间以“ ”为分割，返回一个命令链表
// 需要string.h库和commandList结构
// 测试出来的注意事项，输入的需要是字符数组，不是指针，不然就会报错
commandList *commandHandle( char *commandString){
    // 判断是否为缺省命令
    if ( commandString[0] == '\0' ) return NULL;
    // 先创建起始变量，Head是返回的头指针，List是当前操作指针
    commandList *Head;
    commandList *List;
    // 给Head分配地址
    Head = (commandList*)malloc(sizeof(commandList));
    // 调用strtok函数，第一次在循环外，后面就可以用NULL了
    Head->containString = strtok( commandString, " ");
    // 判断一下是否未录入有效命令，若是，直接返回NULL
    if( Head->containString == NULL )   return NULL;
    // 为下一个分配地址
    List = (commandList*)malloc(sizeof(commandList));
    // List从Head->next开始
    Head->next = List;
    // 循环为死循环，内部条件开循环
    while( 1 ){
        // 读取下一个命令
        char *temp=NULL;
        List->containString = strtok( NULL, " \n\t");
        // 测试点可以结束了
        // printf("%s\n",List->containString);
        // 判断是否还有命令，如果没有就退出循环
        if ( ! List->containString) break;
        // 如果有命令就建立链表
        commandList *NEXT;
        NEXT = (commandList*)malloc(sizeof(commandList));
        List->next = NEXT;
        List = List->next;
    }
    // 把最后的next定义为NULL
    commandList *temp;
    commandList *temp2;
    temp = Head;
    while( 1 ){
        temp2 = temp->next;
        if( temp2->containString == NULL){
            temp->next = NULL;
            free(temp2);
            break;
        }
        temp = temp->next;
    }
    // 最后返回Head头指针
    return Head;
}

// 判断是否读入脚本，若有，便返回，没有则返回空指针
FILE *judgeCommand( int argc, char ** argv){
    // 给的脚本文件过多报错,错误退出
    if ( argc > 2 ){
        fprintf(stderr, RED "[myshell] Error: Incorrect parameters! ");
        exit(1);
    }
    // 正常给出脚本文件，开始处理
    if ( argc == 2 ){
        FILE * temp;
        // 打开脚本文件
        temp = fopen( argv[1], "r" );
        // 不存在该文件，读取失败,错误退出
        if ( temp == NULL ){
            fprintf(stderr, RED "[myshell] Error: There is no file \"%s\"!\n", argv[1]);
            exit(1);
        }
        // 能正常打开就返回temp脚本文件符
        else    return temp;
    }
    // 脚本文件缺省，返回NULL空指针
    else 
        return NULL;
}

// 读取命令行操作，读取脚本文件中的一行命令并输出链表命令
// 若无脚本则读取输入字符串
// 并将命令行转化为commandList链表
commandList * readCommand( FILE * script ){
    // 如果文件为NULL，就读取用户输入
    if( script == NULL){
        // 首先跳出提示符
        function_showPrompt();
        // 读取输入
        gets( string );
        // 如果读取失败，就报错提示，继续下一波循环
        if( *string == '\0' ){
            return NULL;
        }
    }
    else{
        // 用fget的返回值判断是否成功读取到内容，若未读取到则到达末尾结束进程
        if ( fgets( string, MAX_COMMAND_NUMBER, script ) == NULL ){
            printf("The script has all been handled!\n");
            function_exit();
        }
        // 清除最后的换行符
        // 先计数
        int count = strlen( string );
        // 再判断,如果是换行符,就换成终止符
        if( string[count-1] == '\n' )
            string[count-1] = '\0';
    }

    // 若成功读取到内容，则将读取到的命令行文本转化为命令链表返回
    return commandHandle( string );
}

// setGlobalVarious函数，在处理完命令后，对命令进行初步处理，调整好全局变量
void setGlobalVarious(){
    // 先把commandHead提取出来
    commandList *temp;
    temp = commandHead;
    // 留一个temp的上级链表便于控制
    commandList *uptemp;
    // 搞一个指针用来释放空间
    commandList *freeTemp;
    // 遍历处理
    // 处理的内容主要有两个，一个是处理管道，重定向等
    // 第二个是处理命令的参数数量
    // 作为计数指针
    int *count;
    count = &temp->commandCount;
    // 初始化命令数值
    // 测试的时候发现第一次命令一直没用,最后用gdb跟踪发现commandCount根本没有动过,发现问题所在
    *count = 0;
    while( temp != NULL ){

        // 如果碰上管道，就改变管道变量
        if( strcmp( temp->containString, "|") == 0 ){
            ifPipeExist = 1;
            // 把命令分为两段，前一段从｜段开始分开，因此uptemp的next就指向NULL
            uptemp->next = NULL;
            freeTemp = temp;
            temp = temp->next;
            // 可以把含有｜的这个节点释放了
            // 不用裁,最后初始化可以解决,裁了反而出bug
            // free( freeTemp );
            // 同时计数停止，把指针指到下一个链表上
            count = &temp->commandCount;
            // 初始化count
            *count = 0;
            // 同时记下管道右边命令的指针
            commandHead_pipe = temp;
            continue;
        }

        // 如果碰上重定向，则把重定向符号后面的参数录入对应的全局变量
        // 重定向输入
        if( strcmp( temp->containString, "<") == 0 ){
            ifInputExist = 1;
            // temp再找下一节录入路径
            freeTemp = temp;
            temp = temp->next;
            strcpy( inputFilePath, temp->containString );
            // 中间就可以直接裁掉了
            // 不用裁,最后初始化可以解决,裁了反而出bug
            // free( freeTemp );
            freeTemp = temp;
            temp = temp->next;
            // free( freeTemp );
            continue;
        }

        // 重定向输出（覆盖
        if( strcmp( temp->containString, ">") == 0 ){
            ifOutputExist = 1;
            // temp再找下一节录入路径
            freeTemp = temp;
            temp = temp->next;
            strcpy( outputFilePath, temp->containString );
            // 中间就可以直接裁掉了
            // 不用裁,最后初始化可以解决,裁了反而出bug
            // free( freeTemp );
            freeTemp = temp;
            temp = temp->next;
            // free( freeTemp );
            continue;
        }

        // 重定向输出（追加
        if( strcmp( temp->containString, ">>") == 0 ){
            ifOutputExist_plus = 1;
            // temp再找下一节录入路径
            freeTemp = temp;
            temp = temp->next;
            strcpy( outputFilePath_Plus, temp->containString );
            // 中间就可以直接裁掉了
            // 不用裁,最后初始化可以解决,裁了反而出bug
            // free( freeTemp );
            freeTemp = temp;
            temp = temp->next;
            // free( freeTemp );
            continue;
        }

        // 后台
        if( strcmp( temp->containString, "&") == 0 ){
            // 管他呢，默认命令行结束了
            // 给后台开关开一下
            ifBg = 1;
            // 收尾撤退
            uptemp->next = NULL;
            break;
        }

        *count = *count + 1;
        uptemp = temp;
        temp = temp->next;
    }
    return;
}

// initJoblist函数用于初始化后台进程表
// 后台进程表用共享内存以加速各进程间的联络交流
void initJoblist(){
    // 首先用int整数型tempJob,tempJobnumber保存shmget函数的返回值，用于判断是否成功申请共享内存
    int tempJob;
    // 第一个参数取值为IPC_PRIVATE，则函数shmget()将创建一块新的共享内存
    // 第二个参数是最大的任务数乘任务结构的大小，和一个int存储任务数量避免溢出
    // 第三个参数看例子照着学的，创建新的内存
    tempJob = shmget( IPC_PRIVATE , sizeof(job)*(MAX_JOBS_NUMBER) + sizeof(int) , 0666 | IPC_CREAT );
    // 测试是否正确返回内存
    if( tempJob == -1 ){
        // 返回失败，报错
        fprintf( stderr, RED "[myshell] Error: Create shared memory failed!");
        exit(1);
    }

    // 把共享内存挂接到当前进程中,并用void*型指针tempJobP接受结果
    void *tempJobP;
    // 用shmat函数挂接
    // 第一个参数为共享存储短的标识符，即我们在上面得到的返回值
    // 第二个参数推荐为0，可以链接到由内核选择的第一个可以地址上
    // 第三个直接写0，以读写形式
    tempJobP = shmat( tempJob, 0, 0);

    // 测试是否正确返回
    if( tempJobP == (void*)-1 ){
        fprintf( stderr, RED "[myshell] Error: Connnect shared memory failed!");
        exit(1);
    }

    // 将最后得到的tempJobP和tempJobnumberP指针赋给全局变量jobList，jobNumber
    jobList = (job*)tempJobP;
    jobNumber = (int*)((char*)tempJobP + sizeof(job)*MAX_JOBS_NUMBER );

    // 将赋给的两个全局变量进行初始化
    //  首先将jobList中的第一个任务定义为myshell，其他的pid改为-1即无任务
    jobList[0].pid = getpid();
    strcpy( jobList[0].jobname, "myshell");
    jobList[0].type = FG;
    jobList[0].status = RUN;
    for(int i=1; i<MAX_JOBS_NUMBER; i++){
        jobList[i].pid = -1;
    }

    // 将jobNumber改为1
    *jobNumber = 1;
}

// plusJob函数用于添加新的任务到最近的一个数组格子中
// 返回值是新增加的任务的结构指针
job* plusJob( pid_t npid, char *njobname, int ntype, int nstatus){
    // 首先看一眼任务有没有达到上限，是就直接报错
    // 首先把全局变量jobNumber转给本地变量便于使用
    int sumNumber = *jobNumber;
    // 进行判断
    if( sumNumber == MAX_JOBS_NUMBER ){
        printf( stderr, RED "[myshell] Error: Job has reached the max number!");
        exit(1);
    }
    
    // 进行任务加入
    jobList[sumNumber].pid = npid;
    jobList[sumNumber].type = ntype;
    jobList[sumNumber].status = nstatus;
    strcpy( jobList[sumNumber].jobname, njobname);
    *jobNumber = sumNumber + 1;
    return( &jobList[sumNumber] );
}

// deleteJob函数用于删除指定pid的任务
void deleteJob( pid_t pid){
    // 把全局变量jobNumber存下便于使用
    int sumNumber = *jobNumber;
    // 遍历寻找
    int location;
    for( location = 0; location<sumNumber; location++){
        // 如果找到了就直接退出
        if( jobList[location].pid == pid )  break;
    }
    // 此时通过判断location的值可以判断是否找到
    if( location == sumNumber ){
        printf("pid:%d can't find in jobList!",pid);
        return;
    }
    // 存在该任务，删除后台的任务
    // 循环左移覆盖
    while( location<sumNumber-1 ){
        jobList[location] = jobList[location+1];
        location++;
    }
    // 更新任务数量
    *jobNumber = sumNumber - 1;
}
