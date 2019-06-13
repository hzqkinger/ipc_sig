#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/wait.h>

//1.写端关闭读端开放：如果此时仍有进程从管道的读端读数据，那么管道中剩余的数据都被读取后，再次read会返回0，就像读到文件末尾一样。
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
				char _mesg[20];
				sprintf(_mesg,"i am child %d",i);
				write(_pipe[1],_mesg,strlen(_mesg)+1);
				sleep(1);
				++i;
			}
			close(_pipe[1]);
		}else{
			close(_pipe[1]);
			char _mesg[100] = { 0 };
			int j = 0;
			while(j < 100){
				int ret = read(_pipe[0],_mesg,sizeof(_mesg));
				printf("%s  code is:%d\n",_mesg,ret);
				++j;
			}
		if(waitpid(pid,NULL,0) < 0){
			return 3;
		}


		}
		return 0;
}

