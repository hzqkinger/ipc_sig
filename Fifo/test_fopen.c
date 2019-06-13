#include<stdio.h>

int main()
{
		FILE *fp0 = fopen("file","r");    //以只读的方式代开一个文件
		FILE *fp1 = fopen("file","a");    //以尾加的方式打开一个文件
		
		const char *s = "this is test";
		fputs(s,fp1);

		char Read[10] = { 0 };
		int i = 0;
		while(i < 9){
			Read[i] = fgetc(fp0);
			++i;
		}
		Read[9] = '\0';
		printf(Read);

		printf("\n");
		rewind(fp0);
		char ch;
		while(!feof(fp0)){
				ch = fgetc(fp0);
				putchar(ch);
		}


//		rewind(fp0);
//		putchar(fgetc(fp0));  //为什么不能输出到标准输出上
		return 0;
}
