// 程序名：myshell
// 作者：吴杰枫 3190100833

// 需要用到的外部变量的声明
extern commandList *commandHead;
extern job *jobList;
extern int *jobNumber;

// 需要用到的外部函数的声明

// 用于commandList格式的exe函数
int listExecvp( commandList * muteList, int count );

// 内部函数的声明

// function_cd函数，用于转换当前目录，对应cd功能
void function_cd( commandList * Head, int count );

// funciton_pwd函数，显示当前目录，对应pwd功能
void function_pwd();

// function_time函数,输出当前时间,对应time函数
void function_time();

// function_clear函数,清除现有的输出，对应clr或clear功能
void function_clear();

// funciton_dir函数，列出参数目录的内容，对应dir功能
void function_dir( commandList * Head, int count );

// readDir函数，function_dir的辅助函数，用于读取并输出单个目录中的内容，对应dir功能
void readDir( char *dirName );

// function_set函数，列出所有环境变量，对应set功能
void function_set();

// function_echo函数，输出后续参数，对应echo功能
void function_echo( commandList * Head, int count );

// function_exec函数，用参数所代表的命令替换当前进程，对应exec功能
void function_exec( commandList * Head, int count );

// funciton_help函数，显示用户手册并用more命令过滤，对应help功能
void function_help();

// function_exit函数,直接退出进程,对应exit功能
void function_exit();

// function_bg函数，将最近一个挂起的进程转化为后台运行。对应bg功能
void function_bg();

// function_fg函数，将最近的一个后台进程转化到前台运行，对应fg功能
void function_fg();

// function_jobs函数，显示所有后台进程，对应jobs功能
void function_jobs();

// function_test函数，缩减版，进行字符串或者数字的比较，对应test功能
void function_test( commandList * Head, int count );

// 辅助函数IsNumber，用于判断字符串是否为一串数字
// 输入参数为一个判断int型指针和一个字符数组
// 若字符数组为一串数字，则判断指针内容改为1，并返回字符数组对应的数值
// 若字符数组不为一串数字，则判断指针内容改为0，并返回0
int IsNumber( char *test, int *isNum );

// function_umask函数，无参数时，显示当前掩码；有一个参数时，将当前的掩码改为参数值，对应umask功能
void function_umask( commandList * Head, int count );