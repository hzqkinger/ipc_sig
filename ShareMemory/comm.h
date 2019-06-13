#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>

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

