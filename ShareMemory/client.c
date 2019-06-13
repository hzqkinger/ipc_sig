#include"comm.h"
int main()
{
	int shm_id;
	if((shm_id = shmget(1234,0,0)) < 0){ //1.获取shm_id
		printf("shmget error\n");exit(1);
	}
	void *shm_addr = shmat(shm_id,NULL,0);  //2.映射共享地址
	if(shm_addr == (void*)-1){
		printf("shmat error\n");exit(2);
	}

	int sem_id;
	if((sem_id = semget(123,1,0)) == -1){  //一.获取信号量
		printf("semget error\n");exit(-1);
	}
//	union semun sem_union;
//	sem_union.val = 0;
//	if(semctl(sem_id,0,SETVAL,sem_union) == -1){   //二.初始化信号量(注意,根本不需要在客户端设置信号量)
//		printf("semctl error\n");exit(-2);
//	}

	int i = 0;
	while(i++ < 50){
			sem_down(sem_id);         //三.P操作
//		char str[24];
//		memcpy(str,shm_addr,strlen(str)+1);
//		printf("[%d]",i);printf(str);
		printf(shm_addr);printf("\n");
		//sleep(1);
			sem_up(sem_id);       //四.V操作
	}
	
	shmdt(shm_addr);
	return 0;
}

