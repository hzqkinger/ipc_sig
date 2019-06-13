#include"comm.h"
int main()
{
	int shm_id;
	if((shm_id = shmget(1234,1024,IPC_CREAT | 0666)) < 0){  //获取shm_id
		printf("shmget error\n");exit(1);
	}
	char *shm_addr = (char*)shmat(shm_id,NULL,0); //2.映射共享地址
	if(shm_addr == (char*)-1){
		printf("shmat error\n");exit(2);
	}

	int sem_id;
	if((sem_id = semget(123,1,IPC_CREAT | 0666)) == -1){  //一.获取信号量
		printf("semget error\n");exit(-1);
	}
	union semun sem_union;
	sem_union.val = 1;
	if(semctl(sem_id,0,SETVAL,sem_union) == -1){   //二.初始化信号量（注意,只需在服务器端初始化信号量即可）
		printf("semctl error\n");exit(-2);
	}

	int i = 0;
	while(i < 50){
			sem_down(sem_id);         //三.P操作
//		char str[24];
//		sprintf(str,"i am server:%d\n",i);
//		memcpy(shm_addr,str,strlen(str)+1);
		shm_addr[i++] = 'A';shm_addr[i] = '\0';
		sleep(2);
			sem_up(sem_id);       //四.V操作
	}
	
	shmdt(shm_addr);
	return 0;
}

