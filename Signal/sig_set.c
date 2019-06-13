#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include<string.h>
#include <stdlib.h>

//typedef void (*sighandler_t)(int);
void sig_h(int sig_no)/*{{{*/
{
	if(sig_no == SIGINT)
			printf("get (ctrl+c) SIGINT\n");
	else if(sig_no == SIGQUIT)//signal收到该信号，貌似不会进sig_h函数，而是执行默认动作
			printf("get (ctrl+\\) SIGINT\n");
	else if(sig_no == SIGSTOP)
			printf("get (ctrl+z) SIGINT\n");
	else
			printf("i have no idea what signal is\n");
}/*}}}*/

void test1()/*{{{*/
{
	// sighandler_t signal(int signum,sighandler_t handler);
	// signum 需要重新定义处理方式的信号
	// handler 信号处理方式：
	// 			1，SIG_IGN  忽略处理
	// 			2，SIG_DFL  默认处理
	// 			3，typedef void(*handler)(int)  自定义处理函数
	
	while(1){
		printf("wait for signal...\n");
//		//int sig_no;//下面三行代码无效
//		//scanf("%d",&sig_no);
//		//signal(sig_no,sig_h);
		signal(SIGINT,sig_h);              //利用signal函数注册信号
		printf("%d\n",SIGINT);
		signal(SIGQUIT,sig_h);
		signal(SIGSTOP,sig_h);
		//signal(2,sig_h);
		pause();
	}
}/*}}}*/

void test2() //该函数是从终端获取一个信号/*{{{*/
{
	// int sigaction(int signum,const struct sigaction *act,struct sigaction *oldact)
	// signum 信号编号
	// act    如果非NULL，则信号signum以及该信号处理方法被安装到act中
	// oldact 如果非NULL，则旧的信号以及该信号处理方法被保存到oldact中
	// 返回值 成功0，失败-1并设置errno
//  struct sigaction
//  {
//  	void (*sa_handler)(int);//信号处理函数
//  	void (*sa_sigaction)(int,siginfo_t*,void*);//另一种替代的信号处理函数
//  	sigset_t sa_mask;//在处理信号过程中如果不希望受到其他信号的影响，就可以
//  	  把其他信号添加到这个集合中，暂时阻塞
//  	int sa_flags;//该标志决定调用哪个信号信号处理函数，如果被指定为SA_SIGINFO
//  	  那么调用sa_sigaction；否则调用sa_handler
//  	void (*sa_restorer)(void);//应用程序通常不使用这个成员
//  }

	printf("wait for signal...\n");
	struct sigaction act;           //struct sigaction是内核中写好的结构体
	memset(&act,0,sizeof(act));
	act.sa_handler = sig_h;

	if(sigaction(SIGINT,&act,NULL) < 0){        //利用sigaction函数注册信号
		printf("sigaction error\n");
		exit(1);
	}
	if(sigaction(SIGQUIT,&act,NULL) < 0){
		printf("sigaction error\n");
		exit(1);
	}
	if(sigaction(SIGPIPE,&act,NULL) < 0){
		printf("sigaction error\n");
		exit(1);
	}
	pause();//pause函数使调用进程挂起直到有信号递达
}/*}}}*/

void sig_alrm(int signo){}/*{{{*/
unsigned int mysleep(unsigned int nsec)
{
	struct sigaction new,old;
	unsigned int unslept = 0;

	new.sa_handler = sig_alrm;
//	new.sa_handler = SIG_DFL; 
	sigemptyset(&new.sa_mask);
	new.sa_flags = 0;

	sigaction(SIGALRM,&new,&old); //注册信号处理函数
	alarm(nsec);//设置闹铃
	pause();

	//unslept = alarm(0);//清除闹铃
	//sigaction(SIGALRM,&old,NULL);//恢复默认信号处理动作

	return unslept;
}/*}}}*/

int main()
{
//    test1();
//	test2();

//	while(1){
//		int left = mysleep(5);
//		printf("left:%d  5s passed\n",left);
//	}
	return 0;
}


