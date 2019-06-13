#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>

//4.写端开放读端开放，但是读端不读数据：这时有进程向管道中写数据，那么管道被写满时，write会被阻塞，直到管道中有空位了才能再次写入数据。
int main()
{
		int _pipe[2];
		if(pipe(_pipe) == -1){
			printf("pipe error\n");
			exit(1);
		}

		pid_t pid;
		if((pid = fork()) < 0){
			printf("fork error\n");
			return 2;
		}else if(pid == 0){
			close(_pipe[0]);
			int i = 0;
			while( i < 10000){
				char _mesg[20] = "12345678";
				//sprintf(_mesg,"i am child %d",i);
				write(_pipe[1],_mesg,strlen(_mesg));
				printf("I'm child,i'm writing data to pipe %d\n",i);
				++i;
			}
		}else{
				close(_pipe[1]);
				if(waitpid(pid,NULL,0) < 0){    //这一步的作用是什么，防止子进程变僵尸吗？（是的，假如没有waitpid，父进程没有接收子进程的退出状态）
					return 3;
				}
		}
		return 0;
}



