//先来两个认知:
//   一.父子进程共享打开的文件，在这暂且认为它们共享open的返回值
//   二.信号量在这使用来同步进程的(注意,无论进程间有没有血缘关系),为了防止出现因多个进程同时访问一个共享资源而引发的一系列问题.
//   (ps:注意拿该程序与当前目录下的test_fork.c文件比较一下)

#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
//#include<sys/types.h>
//#include<sys/stat.h>
#include<fcntl.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>

union semun{  //必须自己手动定义
	int val;
};

// 减 1，加锁，P 操作
void sem_down(int sem_id)
{
	struct sembuf sem_lock;
	memset(&sem_lock,'\0',sizeof(sem_lock));
	sem_lock.sem_num = 0;
	sem_lock.sem_op = -1;
	sem_lock.sem_flg = SEM_UNDO;//或者填0
	if (-1 == semop(sem_id, &sem_lock, 1)) 
		printf("semaphore lock failed.\n");
}
// 加 1，解锁，V 操作
void sem_up(int sem_id)
{
	struct sembuf sem_unlock;
	memset(&sem_unlock,'\0',sizeof(sem_unlock));
	sem_unlock.sem_num = 0;
	sem_unlock.sem_op = 1;
	sem_unlock.sem_flg = SEM_UNDO;//或者填0
	if (-1 == semop(sem_id, &sem_unlock, 1))
		printf("semaphore unlock failed.\n");
}

int main()
{
	int fd;                                    //1.父子进程会共享打开的文件
	if((fd = open("file",O_RDONLY))< 0){
		printf("fopen error\n");
		exit(1);
	}

	int sem_id;
	if((sem_id = semget(1234,1,IPC_CREAT | 0666)) == -1){  //一.获取信号量
		printf("semget error\n");
		exit(-1);
	}
	union semun sem_union;
	sem_union.val = 1;
	if(semctl(sem_id,0,SETVAL,sem_union) == -1){   //二.初始化信号量
		printf("semctl error\n");
		exit(-2);
	}

	pid_t pid;
	if((pid = fork()) < 0){                  //2.fork之后,父子进程交替运行,交替从fd文件描述符指向的文件中读数据
		printf("fork error\n");
		exit(1);
	}else if(pid == 0){                   //3.执行时不管父子进程各自sleep多少秒, 我们希望父子进程交替者一个一个读取
		while(1){
			sem_down(sem_id);         //三.P操作
			sleep(2);
			char ch;
			read(fd,&ch,1);
			printf("in child:%c\n",ch);
			sem_up(sem_id);       //四.V操作
		}
	}else{
		while(1){
			sem_down(sem_id);
			sleep(1);
			char ch;
			read(fd,&ch,1);
			printf("in parent:%c\n",ch);
			sem_up(sem_id);
		}
	}

	close(fd);
	return 0;
}

