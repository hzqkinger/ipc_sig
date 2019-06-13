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

int msg_client_start()
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

		msginfo _cli_info;
		int i = 0;
		while(i++ < 100){
				//sprintf(_cli_info.mtext,"i am client process%d",i);
				fgets(_cli_info.mtext,_MSGSIZE,stdin); //这个fgets函数使该进程阻塞在stdin流
				_cli_info.mtype = 2;
				if(msgsnd(_msg_id,&_cli_info,sizeof(_cli_info.mtext),0) == -1){  //阻塞式时：如果没有消息接收者，那么一直会阻塞在这
						print_log("client send message error\n");           //非阻塞式时：如果没有消息接收者，只向队列里发了8条消息，然后立即退出
						return 4;
				}
				sleep(1);
			if(msgrcv(_msg_id,&_cli_info,sizeof(_cli_info.mtext),1,IPC_NOWAIT) != -1){
					printf("from server:>%s\n",_cli_info.mtext);
			}
		}
		return 0;
}

//static void delete_msg()
//{
//		if(_msg_id == -1){
//				if(msgctl(_msg_id,IPC_RMID,NULL) == -1){
//						printf("delete msg queue error");
//				}
//		}
//		printf("delete msg queue end\n");
//}

int main()
{
//		atexit(delete_msg);
		msg_client_start();
		printf("client over\n");
		return 0;
}
