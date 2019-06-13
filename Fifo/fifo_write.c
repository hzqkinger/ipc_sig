#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>

#define _PATH "/home/huangzhiqiang/day22/fifo_file2"
#define _SIZE 100
int main()
{
		if(mkfifo(_PATH,0666 | S_IFIFO) == -1){
			printf("mkfifo error\n");
			return 1;
		}
		
		int fd = open(_PATH,O_WRONLY);
		if(fd < 0){
			printf("open error\n");
			return 2;
		}
		char buf[_SIZE] = { 0 };
		while(1){
			scanf("%s",buf);
			if(write(fd,buf,strlen(buf)+1) < 0){
				printf("write error\n");
				break;
			}
		}
		close(fd);

		return 0;
}
