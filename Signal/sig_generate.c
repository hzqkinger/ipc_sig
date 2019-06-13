#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include<string.h>
#include <stdlib.h>

void test1()/*{{{*/
{
	//调用raise来向当前进程或线程发送一个信号（1.向自身发送信号）
	//int raise(int sig);  sig是信号编号；return成功返回0，失败返回非0

	//raise(SIGKILL);
	raise(SIGSTOP);
	printf("process run ok\n");
}/*}}}*/

void test2()/*{{{*/
{
	//调用kill来向一个指定进程发送指定信号（2.向别的进程发送信号）
	//int kill(pid_t pid,int sig);  pid是进程编号；sig是信号编号; return同上.

	kill(3570,SIGKILL);
	//注意:1.这里杀死的是目标文件loop;2.先要使用ps命令查loop的pid，然后把值填进来即可.
	printf("has killed\n");
}/*}}}*/

void test3()/*{{{*/
{
	//调用alarm来定时seconds发送一个SIGALRM信号,该信号的默认动作是终止进程.
	//unsigned int alarm(unisgned int seconds);
	//seconds是定时时间,如果为0则取消所有并定的定时器；return 饭户闹钟的剩余时间
	
	alarm(3);
	int i = 0;
	while(1){
		usleep(500000);
		printf("I am running happily:%d\n",i);
		++i;
	}
	printf("test3 exit\n");//根本不会运行这句代码
}/*}}}*/

void test4()/*{{{*/
{
	//void abort();
	//直接给当前线程或进程发送一个SIGABRT信号
	printf("before\n");
	abort();
	printf("after\n");
}/*}}}*/

void test5()/*{{{*/
{
	// int sigqueue(pid_t pid,int signo,const union sigval val);
	// sigqueue()比kill()传递了更多的附加信息，但sigqueue()只能向一个进程发送信号，
	//  而不能发送信号给一个进程组。如果signo=0，将会执行错误检查，但实际上不发送
	//  任何信号，0值信号可用于检查pid的有效性以及当前进程是否有权限向目标进程发送信号
	//
	// 在调用sigqueue时，sigval_t指定的信息会拷贝到对应sig 注册的3参数信号处理函数的
	//  siginfo_t结构中，这样信号处理函数就可以处理这些信息了。

	printf("before\n");
	union sigval val;
	val.sival_int = 100;
	sigqueue(getpid(),SIGINT,val);
	printf("after\n");
}/*}}}*/

int main()
{
	//test1();//raise
	//test2();//kill
	//test3();//alarm
	//test4();//abort
	test5();//sigqueue
	return 0;
}
