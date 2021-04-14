#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
//testing an idea to create as many child processes as I want without duplicates
//ignore this file

typedef struct {
 int * val;
}test_struct;

void child(){
 printf("child\n");
 exit(0);
}

int main(){
 pid_t pid;
 int * t = malloc(sizeof(int));
 *t = 1;
 test_struct * test = malloc(sizeof(test_struct));
 printf("hello");
 test->val = t;
 printf("%d\n",*(test->val));
 for(int i = 0;i < 5;i++){
	pid = fork();
	if (pid < 0){
	 printf("forking error");
	}
	if (pid == 0){
	 child();
	}
	else{
	 printf("parent\n");
	}
 }
 return 0;

}
