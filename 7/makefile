# 程序名：myshell
# 作者：吴杰枫 3190100833
# makefile文件，包括clean命令

myshell: main.o general.o global.o mysignal.o program.o simple.o
	gcc -g -o myshell main.o general.o global.o mysignal.o program.o simple.o

general.o : general.h global.h
	gcc -g -c general.c
global.o : global.h
	gcc -g -c global.c
main.o : main.h global.h
	gcc -g -c main.c
mysignal.o : mysignal.h global.h
	gcc -g -c mysignal.c
program.o : program.h global.h
	gcc -g -c program.c
simple.o : simple.h global.h
	gcc -g -c simple.c
clean : 
	rm myshell main.o general.o global.o mysignal.o program.o simple.o
