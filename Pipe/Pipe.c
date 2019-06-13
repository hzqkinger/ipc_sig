#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

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
			while( i < 100){	
				char _mesg[20];
				sprintf(_mesg,"i am child %d",i);
				write(_pipe[1],_mesg,strlen(_mesg)+1);
				sleep(1);
				++i;
			}
		}else{
			close(_pipe[1]);
			char _mesg[100] = { 0 };
			int j = 0;
			while(j < 100){
				read(_pipe[0],_mesg,sizeof(_mesg));
				printf("%s\n",_mesg);
				++j;
			}
		}
		return 0;
}
