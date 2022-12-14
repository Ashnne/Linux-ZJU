**myshell设计文档**

一、基本思路：
    在开始设计myshell前，我再次回顾了Linux课堂上所讲的，一些shell的基础操作和基础框架。同时我参考了老师建议的推荐UNIX环境高级编程的相关部分，因此我此次制作myshell的语言就选择UNIX环境的示例语言C语言，加上我只学过C语言相关的知识，相对来说用C语言来作为此次大作业的语言会相对比较方便。
    同时，在确定编程语言后，考虑到myshell的交互，显示等，加上前一题用shell做vim的思路有些大同小异，因此决定用一个恒对循环来做输入，并在最前面加上提示来实现myshell的初始界面，一方面不断的循环接受命令等，另一方面在循环开始时使用printf来实现命令提示符。在接受用户命令方面，首先将用户命令以字符串的形式保存下来，再将命令字符串进行分析分解，分别读取处理，再通过分析不同的命令调用不同的函数进行处理，处理结束后开始下一个循环。
    其次是脚本文件的输入，因为在输入时，除了直接输入命令集合字符串外，若输入未知命令时则自动认为输入的为脚本路径，并尝试打开脚本文件，如果成功打开则逐行读取脚本文件中的命令行，并分析分解命令，执行对应命令，并在读取到脚本文件最后的时候结束进程。
    主循环的框架思路已经搭建好了，下面需要设计的就是命令对于的调用函数模块，在这些模块中其实也是仅仅实现一小部分，因为命令行字符串可能包含很多段小命令，如用exec或者｜管道等，因此可以通过将命令行字符串分段成一个个小部分，分别调用不同的函数来进行，同时不同的函数也可以通过调用系统库内的内建函数命令来进行，如调用envirn来输出环境变量，通过替换标准输出输入来实现重定向等，还有用getuid（）等内置函数来得到uid。
    其次是一些需要另外调用进程的较大的命令。这里借鉴了一个学长的程序，首先用fork（）函数创建一个子进程，在子进程中利用execvp（）函数查找执行相应的外部命令，在主进程中在使用waitpid（）来阻塞主进程，等待子进程退出，就实现调用新的子进程运行较大的命令的功能。
    其次再考虑一些其他的功能。
    & : 后台运行功能的实现。检测到 & 字符后，打开后台运行的全局变量后台运行的开关，这样在主进程中仍可以使用waitpid（）函数，但是选择非阻塞选项，这样就不会阻塞主进程的进行，同时实现命令在后台执行了。
    > < >> : 重定向功能的实现。这里我主要查阅了UNIX环境高级编程的第五章，标准O/I库，重定向只需要将标准输入、输出、错误流改变为不同打开的文件即可，通过在读取到'>' '<' '>>'的时候，将后面紧跟着的存入到inputFile和outputFile变量中，并打开对应的replaceInput，replaceOutput，replaceOutputPlus全局变量开关，再用不同方法的open（）函数打开得到文件描述符后，再将相应的文件描述符用dup2（）函数替换标准输入输出，达到重定向的目的。
    ｜ ：管道操作和重定向操作类似，在检测到 ‘｜’ 的标志后，打开全局变量开关ifPipe。在检测到ifPipe的情况下，首先用pipe（）函数创建管道并将管道两端赋给pipeFile[2]字符数组，再用fork（）函数创建子进程，通过fork（）的返回值得到父进程的pid，并将父进程的标准输入替换为管道出口，用waitpid（）函数阻塞父进程直到子进程，再通过在子进程中再次利用fork（）函数调用读取到当前子进程的pid，在主进程将子进程的标准输入替换为管道出口。(在参考书中，有一个用管道来进行的分页的例子很详细的介绍了每步的程序)

二、模块划分
    在参考了bash和学长的例子后，结合我自己的感觉，我把整个系统具体分为几个大的功能模块，主体循环模块，命令读取模块，命令处理（分割）模块，命令执行模块，信号处理模块。
    主体循环模块：主要用于在打开程序后最基本的循环的建立，包括每次循环时最基本的提示符的显示。是一个简单的主体框架功能模块。
    命令读取模块：主要分为两块，一块是一般命令行文本的读取，相对较为简单，直接读取录入命令行文本即可；另一种为脚本文件读取，若打开myshell时有且仅有一个参数，或者使用命令bash，则认为为脚本路径。试图打开该路径脚本，若打开失败则报错，若打开成功则逐行读取脚本内容并作为命令行字符运行，脚本文件读取只结尾时结束myshell进程。
    命令处理（分割）模块：主要对读取的命令行文本进行处理。主要可以分为命令初处理（分割），命令预处理（判定特殊情况如管道、重定向、后台运行等）和命令执行几个部分。首先对读取到的命令行进行分割，将命令行文本转化为链表便于后期处理。再对链表中的命令字符串逐个读取识别，并在读取到“｜”“&”“>“等特殊命令字符时进行相应的全局变量开关设置或者全局变量参数调整等。
    命令执行模块：通过逐个读取经处理过的命令链表中的命令与参数，并结合预处理阶段调整的全局变量开关等，调用自建库或系统库中的函数执行相应的命令。
    信号处理模块：信号方面我了解不多，但是书中提到一些信号的释放与信号的收集，再加上上网查到的资料，决定增加这个模块处理SIGCHLD信号和SIGTSTP信号用于更新后台进程表和挂起当前进程。

三、数据结构与算法
    在myshell的设计过程中，为了便于保存并逐个处理命令，决定将命令行文本转化为命令行链表的形式，同时设计对应的将命令行文本转化为链表形式的函数，以及在后续其他函数的设计过程中，将命令行作为链表为前提进行设计。
    另外，还有job结构和old_action和new_action结构变量为在看完过往其他学长的大作业后，发现确实好用，直接抄的。前者用于存储任务信息，后者用于处理信号。
    typedef struct job
    {
        pid_t pid;
        char jobname[20];
        int type;
        int status;
    }
    struct sigaction old_action;
    struct sigaction new_action;

myshell用户手册

一、基础命令的介绍：
    1.cd
    参数形式： cd <direction>
    命令作用： 把当前默认目录改变为<directory>。如果没有<directory>参数，则显示当前目录。如该目录不存在，会出现合适的错误信息。这个命令也可以改变PWD 环境变量。
    样例：cd /home/dannis

    2.pwd
    参数形式：无
    命令作用：显示当前目录
    样例：pwd

    3.time
    参数形式：无
    命令作用：显示当前时间
    样例：time

    4.clr
    参数形式：无
    命令作用：清屏
    样例：clr或clear

    5.dir
    参数形式：dir <direction>
    命令作用：列出目录<directory>的内容
    样例：dir /home/dannis

    6.set
    参数形式：无
    命令作用：列出所有的环境变量
    样例：set

    7.echo
    参数形式：echo <comment>
    命令作用：在屏幕上显示<comment>并换行（多个空格和制表符可能被缩减为一个空格）
    样例：echo Hello world

    8.help
    参数形式：无
    命令作用：显示用户手册，并且使用more命令过滤
    样例：help

    9.exit
    参数形式：无
    命令作用：退出shell
    样例：exit

    10.bg
    参数形式：无
    命令作用：将最近一个挂起的进程转化为后台运行
    样例：bg

    11.exec
    参数形式：exec <command>
    命令作用：用参数所代表的命令替换当前进程
    样例：exec pwd

    12.fg
    参数形式：无
    命令作用：将最近的一个后台进程转化到前台运行
    样例：fg

    13.jobs
    参数形式：无
    命令作用：显示所有后台进程
    样例：jobs

    14.test
    参数形式：test <compare_1> <command> <compare_2>
    命令作用：缩减版)通过command命令和compare_1,compare_2的组成不同，进行字符串或者数字的比较,command共8种（字符串相等=、字符串不等！=、数值相等-eq、数值不等-ne、数值大于-gt、数值大于等于-ge、数值小于-lt、数值小于等于-le）
    样例：test abc = abc
         test 12 -eq 13

    15.umask
    参数形式：umask <new_mask>
    命令作用：无参数时，显示当前掩码；有一个参数时，将当前的掩码改为参数值
    样例：umask 0122

    







整体组成
1.主文件

main函数（主界面函数）
思路：将用户的命令直接以字符串的形式读入，再通过command函数处理成以空格为分段的字符数组再进行处理

command函数（处理命令函数）
char **command( char *string)

定义：

结构commandList
typedef struct command{
    char *commandString;
    struct command *next;
}commandList;