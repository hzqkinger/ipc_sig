#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

int main()
{
//	mkfifo("fi",S_IFIFO | 0666);
//	mknod("fi",S_IFIFO | 0666,0);
	if(open("fi",O_RDWR) < 0){
		perror("open");
	}
	printf("hh\n");
	return 0;
}
