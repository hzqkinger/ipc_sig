#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>

//2.写端开放读端开放，但写端不再写数据进管道：如果此时仍有进程从管道的读端读数据，那么管道中剩余的数据都被读取后，再次read会阻塞，
//   直到管道中有数据可读了才读数据并返回
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
			while( i < 10){                 //1.子进程刚开始正常网管道中写数据	
				char _mesg[20];
				sprintf(_mesg,"i am child %d",i);
				write(_pipe[1],_mesg,strlen(_mesg)+1);
				sleep(1);
				++i;
			}
			sleep(10);                     //2.子进程睡10秒，当父进程读完管道中的数据后就会阻塞10秒
			while( i < 20){	               //3.子进程又开始正常往管道中写数据
				char _mesg[20];
				sprintf(_mesg,"i am child %d",i);
				write(_pipe[1],_mesg,strlen(_mesg)+1);
				sleep(1);
				++i;
			}
			//4.因为管道随进程，进程在管道在，进程消失管道对应的端口也关闭。所以子进程退出后管道变成写端关闭读段开放的情况
		}else{
			close(_pipe[1]);
			char _mesg[100] = { 0 };
			int j = 0;
			while(j < 50){
				int ret = read(_pipe[0],_mesg,sizeof(_mesg));
				printf("the %dth times  %s  code is:%d\n",j,_mesg,ret);
				++j;
			}
		if(waitpid(pid,NULL,0) < 0){    //这一步的作用是什么，防止子进程变僵尸吗？
			return 3;
		}
		}
		return 0;
}


