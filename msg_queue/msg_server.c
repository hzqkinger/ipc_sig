#include<stdio.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

#define _MSGSIZE 1024
#define _PATH "/home/huangzhiqiang/day22/file2"
#define ID 0

typedef struct _msginfo{
		long mtype;
		char mtext[_MSGSIZE];
}msginfo;

void print_log(char *msg)
{
		printf("%s[%d]:%s\n",__FUNCTION__,__LINE__,msg);
}


int _msg_id = -1;

int msg_server_start()
{
//		key_t _key = ftok(_PATH,ID);
//		if(_key < 0){
//			print_log("get key id error");
//			return 1;
//		}
//		printf("_key:%d\n",_key);

		_msg_id = msgget(1234,IPC_CREAT | 0666);
		if(_msg_id < 0){
				print_log("get _msg_id error" );
				return 2;
		}
		printf("_msg_id:%d\n",_msg_id);

		msginfo _ser_info;
		int i = 0;
		while(i++ < 100){
				if(msgrcv(_msg_id,&_ser_info,sizeof(_ser_info.mtext),2,0) == -1){//阻塞式收消息
						return 3;
				}
				printf("[%d]from client:>%s",i,_ser_info.mtext);

				_ser_info.mtype = 1;
				sprintf(_ser_info.mtext,"now i am server process%d",i);//把从客户端接收的数据改掉之后在发到队列中
				if(msgsnd(_msg_id,&_ser_info,_MSGSIZE,0) == -1){
					print_log("server send message error\n");
				}
		}
		return 0;
}

static void delete_msg()
{
		printf("_msg_id:%d\n",_msg_id);
		if(_msg_id == -1){
				if(msgctl(_msg_id,IPC_RMID,NULL) == -1){
						printf("delete msg queue error");
				}
		}
		printf("delete msg queue end\n");
}

int main()
{
		//atexit(delete_msg);
		msg_server_start();
		
//		int msg_id = msgget(1234,IPC_CREAT | 0666);
//		if(msg_id == -1){
//				printf("msgget error\n");
//				return 1;
//		}
//		printf("%d\n",msg_id);
//		msginfo mybuf;
//		mybuf.mtype = 1;
//		strcpy(mybuf.mtext,"i am send process.\n");
//
//		if(msgsnd(msg_id,&mybuf,sizeof(mybuf.mtext),0) == 0){
//				printf("success\n");
//		}else{
//				printf("failed\n");
//		}
		return 0;
}
