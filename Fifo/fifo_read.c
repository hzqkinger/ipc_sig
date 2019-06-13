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
		int fd = open(_PATH,O_RDONLY);
		if(fd < 0){
			printf("open error\n");
			return 2;
		}
		char buf[_SIZE] = { 0 };
		while(1){
			if(read(fd,buf,sizeof(buf)) <= 0){
				printf("write error\n");
				break;
			}
			printf("%s\n",buf);
		}
		close(fd);

		return 0;
}

