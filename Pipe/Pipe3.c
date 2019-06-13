#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>

//3.写端开放读端关闭：这时有进程向管道的写端写数据，那么该进程会收到信号SIGPIPE，通常会导致进程异常终止。
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
			while( i < 10){
				char _mesg[20] = "i am child 5555";
				//sprintf(_mesg,"i am child %d",i);
				write(_pipe[1],_mesg,strlen(_mesg)+1);
				sleep(1);
				printf("I'm child,i'm writing data to pipe %d\n",i);
				++i;
			}
		}else{
				//sleep(4);
				close(_pipe[1]);
				int j = 0;
				while(j < 4){
					char _mesg[20] = { 0 };
					int ret = read(_pipe[0],_mesg,sizeof(_mesg));
					printf("in parent the %dth times  [%s]  code is:%d\n",j,_mesg,ret);
					++j;
					sleep(1);
				}
				close(_pipe[0]);//当关闭父进程的读写端之后，子进程再对管道进行写，子进程就会收到SIGPIPE信号，然后异常终止掉子进程

				j = 0;
				while(j < 10){  //证明一下只有子进程死了，而父进程还活着
					printf("i am parent and i am living:%d\n",j);
					++j;
					sleep(1);
				}

				if(waitpid(pid,NULL,0) < 0){    //这一步的作用是什么，防止子进程变僵尸吗？
					return 3;
				}
		}
		return 0;
}


