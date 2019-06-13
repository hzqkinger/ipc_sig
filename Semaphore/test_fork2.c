#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
//#include<sys/types.h>
//#include<sys/stat.h>
#include<fcntl.h>

int main()
{
	FILE *fp;                           //注意,父子进程之间共享文件描述符，不会共享文件指针
	if((fp = fopen("file","r"))< 0){    // ps:文件指针对应的属性是父子私有,然而它们访问的文件是共享的，这就很好的解释了在in child一直打印问号的现象
		printf("fopen error\n");
		exit(1);
	}

	pid_t pid;
	if((pid = fork()) < 0){
		printf("fork error\n");
		exit(1);
	}else if(pid == 0){   //父子进程交替运行，假设它们共享文件指针的话，那么肯定会交替输出file文件中的字符
		while(1){
			sleep(1);
			char ch = fgetc(fp);
			printf("in child:%c\n",ch);
		}
	}else{
		while(1){
			sleep(1);
			char ch = fgetc(fp);
			printf("in parent:%c\n",ch);
		}
	}
	return 0;
}

