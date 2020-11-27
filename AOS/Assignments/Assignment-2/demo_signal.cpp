#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
using namespace std;

int sig_val = 0;

void sigintHandler(int sig_num) {
	printf("exiting..\n");
	fflush(stdout);
	sig_val = 1;
	//return (void*)1;
	//return 1;
}

int main() {
	while(1) {
		signal(SIGINT, sigintHandler);
		if(sig_val == 1)
			exit(1);
	}
	return 0;
}