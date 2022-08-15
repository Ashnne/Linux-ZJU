// 程序名：myshell
// 作者：吴杰枫 3190100833
// 这个文件用于放置一些基础的简单的功能实现

#include "global.h"
#include "simple.h"



// function_cd函数，用于转换当前目录，对应cd功能
void function_cd( commandList * Head, int count ){
    // 先创建一个临时字符数组用于存储原路径
    char former_path[MAX_PATH_NUMBER];
    // 调用库中的getcwd函数将当前目录存储在former_path中
    getcwd( former_path, MAX_PATH_NUMBER);
    // 再利用调用chdir函数来实现目录切换
    // 分为参数的不同不同情况
    switch( count ){
        // 如果只有一个（即没有参数只有命令，输出当前目录即可
        case 1:
            printf("%s\n",former_path);
            break;
        // 如果只有两个（即正常的参数个数，尝试切换目录
        case 2:
            // 首先将链表切换为参数链表，即下一个链表
            Head = Head->next;
            // 返回值为0的时候，转化成功
            if( chdir( Head->containString ) == 0 ){
                // 用setenv函数改变环境变量,rewrite设置为1，改变现有的变量
                setenv( "PWD", Head->containString, 1 );
            }
            // 若返回值不为0，即为-1，制定失败，输出错误信息到标准错误输出,并标注为红色
            else
                fprintf( stderr, RED "[myshell] Error: Don't find the path named %s\n",Head->containString);
            // 结束
            break;
        default:
            // 其他情况即参数不对，报错
            printf(stderr, RED "[myshell] Error: incorrect parameters!\n");
    }
    // 结束，返回缺省
    return;
}

// funciton_pwd函数，显示当前目录，对应pwd功能
void function_pwd()
{
    // 创建临时字符数组
    char temp[80];
    // 通过getcwd获得当前目录
    getcwd(temp,80);
    // 将当前目录输出
    printf("%s\n",temp);
}

// function_time函数,输出当前时间,对应time函数
void function_time()
{
    // 主要用time.h中的函数进行
    //定义类型为time_t的变量nowTime 
    time_t nowTime;
    //定义指向tm结构的指针t
    struct tm *t;
    //定义Week字符指针数组，用来存放星期的信息
    char* Week[] = {"周日","周一","周二","周三","周四","周五","周六"};
    //time()函数返回从公元1979年1月1日的UTC时间从0时0分0秒起到现在经过的秒数
    time(&nowTime);
    //locattime()函数将参数所指的time_t结构中的信息转换成真实世界所使用的时间日期表示方法，然后将此结果由结构tm返回
    t = localtime(&nowTime);
    //输出当前时间，格式为：年/月/日 周X 时:分:秒
    printf("%d年%d月%d日 %s %02d:%02d:%02d\n",(1900+t->tm_year),(1+t->tm_mon),t->tm_mday,Week[t->tm_wday],t->tm_hour,t->tm_min,t->tm_sec);
}

// function_clear函数,清除现有的输出，对应clr或clear功能
void function_clear()
{
    // \e转义符为escape \e[{line};{column}H 可以将指针指向特定位置
    // \e[2J 可以清除整个屏幕的信息
    printf("\e[1;1H\e[2J");
}

// funciton_dir函数，列出参数目录的内容，对应dir功能
void function_dir( commandList * Head, int count ){
    // 如果没有任何参数，就输出本目录的内容
    if( count == 1){
        char locPath[MAX_COMMAND_NUMBER];
        getcwd( locPath, MAX_COMMAND_NUMBER );
        readDir( locPath );
        return;
    }
    // 如果是其他情况，就逐个输出参数目录文件
    for( int i=1; i<count; i++){
        // 第一个是命令符，不读取。剩下就是创建循环
        Head = Head->next;
        // 读取链表内的目录
        readDir( Head->containString );
    }
    // 结束函数
    return;
}

// readDir函数，function_dir的辅助函数，用于读取并输出单个目录中的内容，对应dir功能
void readDir( char *dirName ){
    // 这个函数其实在UNIX的辅导书中有可以抄的例子
    // 首先定义DIR和dirent类型指针dp、dirp
    DIR *dp;
    struct dirent *dirp;
    // 如果打开目录文件失败，则报错,并直接结束函数
    if( ( dp = opendir( dirName )) == NULL ){
        printf("can not open the dir %s correctly\n", dirName);
        return;
    }
    // 首先输出目录名
    printf("the contain of %s:\n",dirName);
    // 循环读取目录中的文件，直到结束为止
    while ( (dirp = readdir( dp )) != NULL){
        // 如果文件名不是.或者..，就输出
        if( strcmp( dirp->d_name,"." ) && strcmp( dirp->d_name, "..") ){
            printf("%s\n",dirp->d_name);
        }
    }
    // 结束函数，并输出一个空行隔离
    printf("\n");
    return;
}

// function_set函数，列出所有环境变量，对应set功能
void function_set(){
    // 因为外部变量environ中有所有的环境变量，直接声明输出就好了
    extern char ** environ;
    int i = 0;
    while( environ[i] ){
        printf("%s\n",environ[i]);
        i++;
    }
}

// function_echo函数，输出后续参数，对应echo功能
void function_echo( commandList * Head, int count ){
    // 直接把除了echo命令外的所有参数逐个输出即可
    for( int i=1; i<count; i++){
        // 第一次为跳过命令字符，后面为创建循环
        Head = Head->next;
        // 输出内容，并以空格区分
        printf("%s ",Head->containString);
    }
    // 最后输出换行符
    printf("\n");
}

// funciton_help函数，显示用户手册并用more命令过滤，对应help功能
void function_help(){
    FILE *readme;
    char *helpContain[MAX_INPUT];
    readme = fopen("readme.md","r");
    while( fgets(helpContain, MAX_INPUT, readme) ){
        // 清除最后的换行符
        // 先计数
        int count = strlen( helpContain );
        // 再判断,如果是换行符,就换成终止符
        if( helpContain[count-1] == '\n' )
            helpContain[count-1] = '\0';
        printf("%s",helpContain);
    }
    return;
}

// function_exit函数,直接退出进程,对应exit功能
void function_exit(){
    // 输出结束语
    printf("[myshell] Thanks for using myshell!\n");
    // 直接调用库函数里的exit()函数即可,正常退出
    exit(0);
}

// function_bg函数，将最近一个挂起的进程转化为后台运行。对应bg功能
void function_bg(){
    // 常规保存jobNumber并找最近一个挂起的后台任务
    int sumNumber = *jobNumber;
    int location;
    for( location=sumNumber-1; location>0; location--){
        if( jobList[location].status == SUSPEND && jobList[location].type == BG && jobList[location].pid > 0)   break;
    }

    // 判断是否找到
    // 未找到的话，就输出提示信息
    if( location == 0 ){
        printf("There is no suspend job in the backgroud!\n");
        return;
    }
    // 找到的话，就把该进程转为后台运行状态
    // 首先更新jobList
    jobList[location].status = RUN;
    // 用kill函数向该进程发送信号
    kill( jobList[location].pid, SIGCONT );
    // 最后输出一下提示符
    printf("[%d] %s &\n", location+1, jobList[location].jobname );
}

// function_exec函数，用参数所代表的命令替换当前进程，对应exec功能
void function_exec( commandList * Head, int count ){
    // 如果没有参数就直接结束
    if( count == 1 )    return;
    // 如果有参数就试图用execvp运行
    else{
        // 先把开头的exec去掉,数参数的时候也减一
        Head = Head->next;
        listExecvp( Head, count-1 );
    }
    
}


// function_fg函数，将最近的一个后台进程转化到前台运行，对应fg功能
void function_fg(){
    // 常规保存jobNumber并找最近一个后台任务
    int sumNumber = *jobNumber;
    int location;
    for( location=sumNumber-1; location>0; location--){
        if( jobList[location].type == BG && jobList[location].pid > 0)   break;
    }

    // 判断是否找到
    if( location == 0){
        // 未找到，打出提示符，结束函数
        printf("There is no job in backgroud!");
        return;
    }
    // 如果是挂起的后台程序，
    else if( jobList[location].status == SUSPEND ){
        // 输出准备转化的命令名
        printf("%s\n", jobList[location].jobname );
        // 在jobList中更新任务为运行中的前台任务
        jobList[location].type = FG;
        jobList[location].status = RUN;
        // 用kill函数向该进程发送信号,继续运行
        kill( jobList[location].pid, SIGCONT );
        // 阻塞主进程，等待该进程执行完毕
        // 第一个参数表示期待等待的子进程的pid
        // 第二个参数用于保存终止状态，不感兴趣，因此给空指针
        // 第三个参数用于进行进一步控制，不需要，因此为0即可
        waitpid( jobList[location].pid, NULL, 0);
    }
    // 如果是正在运行的后台进程
    else if( jobList[location].status == RUN){
        // 输出准备转化的命令名
        printf("%s\n", jobList[location].jobname );
        // 在jobList中更新任务为运行中的前台任务
        jobList[location].type = FG;
        // 阻塞主进程，等待该进程执行完毕
        // 第一个参数表示期待等待的子进程的pid
        // 第二个参数用于保存终止状态，不感兴趣，因此给空指针
        // 第三个参数用于进行进一步控制，不需要，因此为0即可
        waitpid( jobList[location].pid, NULL, 0);        
    }
}

// function_jobs函数，显示所有后台进程，对应jobs功能
void function_jobs(){
    // 首先获得全局变量jobNumber减少遍历时调取所用的时间
    int sumNumber = *jobNumber;
    // 用状态字符输出表示状态
    char *statusChar[] ={ "RUN", "SUSPEND", "DONE"};
    // 逐个遍历输出任务
    for( int i=0; i<sumNumber; i++){
        printf("[%d]  %-10s      %s\n",i,statusChar[jobList[i].status],jobList[i].jobname);
    }
    // 最后输出一个换行
    printf("\n");
    return;
}

// function_test函数，缩减版，进行字符串或者数字的比较，对应test功能
void function_test( commandList * Head, int count ){
    // 首先判断变量数量是否正确,错误则报错
    if( count != 4 ){
        fprintf(stderr, RED "[myshell] Error: Incorrect parameter number!\n");
        return;
    }
    // 变量数量正确的话，首先把三个变量依次取出方便使用
    Head = Head->next;
    char *compare_1 = Head->containString;
    Head = Head->next;
    char *cmd = Head->containString;
    Head = Head->next;
    char *compare_2 = Head->containString;

    // 下面进行字符串相关的判断
    // 如果是=，就进行字符串相等的判断
    if( strcmp( cmd, "=" ) == 0 ){
        // 用strcmp函数进行
        if( strcmp( compare_1, compare_2) ) printf("False!\n");
        else printf("True!\n");
        return;
    }
    // 如果是！=，就进行字符串不等的判断
    if( strcmp( cmd, "!=") == 0 ){
        // 用strcmp函数进行
        if( strcmp( compare_1, compare_2) ) printf("True!\n");
        else printf("False!\n");
        return;
    }

    // 下面进行数值相关的判断
    // 首先进行预先处理，加速函数判断速度
    // 两个int参数用于判断两个参数是否为数值
    int isNum_1,isNum_2;
    // 两个int参数用于接受函数返回值
    int Num_1,Num_2;
    // 用辅助函数IsNumber进行预先处理
    Num_1 = IsNumber( compare_1, &isNum_1 );
    Num_2 = IsNumber( compare_2, &isNum_2 );

    // 下面开始判断命令
    // 数值相等
    if( strcmp( cmd, "-eq" ) == 0){
        // 直接比对，同时判断两个参数是否为数值
        if( Num_1 == Num_2 && isNum_1 == 1 && isNum_2 == 1 )
            printf("True!\n");
        else
            printf("False!\n");
        return;
    }
    // 数值不等
    if( strcmp( cmd, "-ne" ) == 0){
        // 直接比对，同时判断两个参数是否为数值
        if( Num_1 != Num_2 && isNum_1 == 1 && isNum_2 == 1 )
            printf("True!\n");
        else
            printf("False!\n");
        return;
    }
    // 数值大于
    if( strcmp( cmd, "-gt" ) == 0){
        // 直接比对，同时判断两个参数是否为数值
        if( Num_1 > Num_2 && isNum_1 == 1 && isNum_2 == 1 )
            printf("True!\n");
        else
            printf("False!\n");
        return;
    }    
    // 数值大于等于
    if( strcmp( cmd, "-ge" ) == 0){
        // 直接比对，同时判断两个参数是否为数值
        if( Num_1 >= Num_2 && isNum_1 == 1 && isNum_2 == 1 )
            printf("True!\n");
        else
            printf("False!\n");
        return;
    }
    // 数值小于
    if( strcmp( cmd, "-lt" ) == 0){
        // 直接比对，同时判断两个参数是否为数值
        if( Num_1 < Num_2 && isNum_1 == 1 && isNum_2 == 1 )
            printf("True!\n");
        else
            printf("False!\n");
        return;
    }
    // 数值小于等于
    if( strcmp( cmd, "-gt" ) == 0){
        // 直接比对，同时判断两个参数是否为数值
        if( Num_1 <= Num_2 && isNum_1 == 1 && isNum_2 == 1 )
            printf("True!\n");
        else
            printf("False!\n");
        return;
    }
    
    // 最后真没东西了。说明命令有问题,报错
    fprintf(stderr, RED "[myshell] Error: Incorrect command!\n");
    return;
}

// 辅助函数IsNumber，用于判断字符串是否为一串数字
// 输入参数为一个判断int型指针和一个字符数组
// 若字符数组为一串数字，则判断指针内容改为1，并返回字符数组对应的数值
// 若字符数组不为一串数字，则判断指针内容改为0，并返回0
int IsNumber( char *test, int *isNum ){
    // judge变量用于判断正负
    int judge = 1;
    // count变量用于遍历字符数组
    int count = 1;
    // sum变量用于存储字符数组对应的数值
    int sum = 0;
    // 首先判断正负
    if( test[count] == '-'){
        judge = -1;
        count++;
    }

    // 判断字符数组是否全部都为数字
    // while进行遍历直至结束
    int maxNumber = count;
    while( test[maxNumber] != NULL){
        // 判断每一位都为数字，否则就改变is Num并结束函数
        // 若为数字则count自增下一个循环
        if( test[maxNumber] >='0' && test[maxNumber]<='9' ){
            maxNumber++;
            continue;
        }
        // 如果不是，则直接结束
        else{
            *isNum = 0;
            return 0;
        }
    }

    // 若判断为对应数值，则遍历计算出对应的数值
    while( test[count] != NULL){
        sum *= 10;
        sum += (test[count] - '0') ;
        count ++;
    }

    // 最后确定符号
    sum *= judge;

    // 输出结果
    *isNum = 1;
    return sum;
}

// function_umask函数，无参数时，显示当前掩码；有一个参数时，将当前的掩码改为参数值，对应umask功能
void function_umask( commandList * Head, int count ){
    // 如果没有参数，则显示当前的掩码
    if( count == 1 ){
        // 定义mask作为中转
        mode_t mask;
        // 用umask函数得到当前掩码
        mask = umask(0);
        // 重置掩码
        umask( mask );
        // 输出获得的掩码
        printf("%04d\n",mask);
    }

    // 如果参数大于一个，则参数错误，直接结束
    else if( count>2 ){
        fprintf(stderr, RED "[myshell] Error: umask only need 1 or 0 parameter!\n");
    }

    
    else{
        // 如果输入的参数不合法（长度大于4或任何一位大于7），则报错
        // 首先把链表转到参数上
        Head = Head->next;
        // 检验长度
        if( strlen( Head->containString )>4){
            // 报错，结束函数
            fprintf( stderr, RED "[myshell] Error: incorrect mask parameter!(too long)");
            return;
        }
        // 检验每一位
        // 首先用一个临时字符数组简化代码
        char *temp = Head->containString;
        // 逐个检验是否为8或9
        for(int i=0;i<4;i++){
            if( temp[i] == '8' || temp[i] == '9'){
                fprintf( stderr, RED "[myshell] Error: incorrect mask parameter!(bigger than 7)");
                return;
            }
        }

        // 此时掩码为正确的，可以调用umask函数直接改变掩码
        unsigned int new_mask = atoi(temp) % 1000;
        umask(new_mask);
    }
}










