#include <iostream>
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;

#define BUFFER 1024
#define CHUNK 512*1024

int logout = 0;

void* file_opr(void* f_arg) {
	int instr, sock_fd = *((int *)arg);

	recv(sock_fd, &instr, sizeof(instr), 0);
}

void* client_listener(void* cli_arg) {
	int add_len, port_no = *((int *)cli_arg);
	int server = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in address;
	address.sin_family = AF_INET;
	address. sin_port = htons(port_no);
	address.sin_addr.s_addr = INADDR_ANY;
	add_len = sizeof(sockaddr);

	if(bind(server, (struct sockaddr *)&address, sizeof(address)) < 0) {
		cout << "BINDING ERROR !! Exiting.." << endl;
		exit(1);
	}
	listen(server, 100);

	while(1) {
		int sock_fd = accept(server, (struct sockaddr*)&address, (socklen_t*)&add_len);
		cout << "Connection Established..." << endl;

		pthread_t t_new;
		if(pthread_create(&t_new, NULL, file_opr, (void *)&sock_fd)) {
			cout << "SOME ERROR !!" << endl;
			exit(1);
		}
		if(logout)
			break;
	}   
}