#include <unistd.h>
#include <sys/types.h>
int main(){
	int pid=-1;
	printf("Hello %d\n",getpid());
	printf("Hello %d\n",getpid());

	pid=vfork();
	if(pid==0){
		exec("/bin/ls" );
		printf("Hello Child %d,%d\n",getpid(),pid);
		//int i;
		//for (i=0;i<10;i++)
		printf("My Parent is %d\n",getppid());
	}
	else{
	// wait(pid);
		printf("Hello Parent %d\n",pid);
	}
	return 0;
 }