#include<stdio.h>
#include<signal.h>
#include<unistd.h>

void printsigset(sigset_t *set)
{
	int i = 0;
	for(;i<32;i++){
		if(sigismember(set,i))//判断指定信号是否在目标集合中
				putchar('1');
		else
				putchar('0');
	}
	printf("\n");
}

int main()
{
	// int sigpromask(int how,const sigset_t *set,sigset_t *oldset);
	// how 如何处理set信号集中的信号
	// 	   1，SIG_BLOCK    阻塞set集合中的信号
	// 	   2，SIG_UNBLOCK  解除阻塞set集合中的信号
	// 	   3，SIG_SETMASK  。。
	// set 自己设置的一个位图，告诉操作系统我们需要处理哪些信号
	// oldset 将原本阻塞信号位图中输出来

	sigset_t s,p;       //1.定义信号集对象
//	sigemptyset(&s); //2.清空初始化
//	sigaddset(&s,SIGINT);//3.添加ctrl+c
	sigfillset(&s);//2.full初始化
	sigdelset(&s,SIGINT);//3.删除ctrl+c
	sigprocmask(SIG_BLOCK,&s,NULL);//4.设置阻塞信号集，阻塞SIGINT信号
	while(1){
		sigpending(&p);//5.读取当前进程的未决信号集,通过参数传出
		printsigset(&p);
		sleep(1);
	}

	return 0;
}
