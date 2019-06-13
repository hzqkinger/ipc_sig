#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
//#include<sys/types.h>
//#include<sys/stat.h>
#include<fcntl.h>

int main()
{
	int fd;                                    //1.父子进程会共享打开的文件
	if((fd = open("file",O_RDONLY))< 0){
		printf("fopen error\n");
		exit(1);
	}

	pid_t pid;
	if((pid = fork()) < 0){                  //2.fork之后,父子进程交替运行,交替从fd文件描述符指向的文件中读数据
		printf("fork error\n");
		exit(1);
	}else if(pid == 0){                   //3.执行时不管父子进程各自sleep多少秒, 我们希望父子进程交替者一个一个读取
		while(1){
			sleep(2);
			char ch;
			read(fd,&ch,1);
			printf("in child:%c\n",ch);
		}
	}else{
		while(1){
			sleep(1);
			char ch;
			read(fd,&ch,1);
			printf("in parent:%c\n",ch);
		}
	}
	return 0;
}
