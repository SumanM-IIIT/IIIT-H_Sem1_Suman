#include <iostream>
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <math.h>
using namespace std;

#define BUFFER 1024
#define CHUNK 512*1024

int logout = 0;

void* file_opr(void* f_arg) {
	int instr, sock_fd = *((int *)f_arg);

	recv(sock_fd, &instr, sizeof(instr), 0);

	if(instr == 3) {
		recv(sock_fd, &logout, sizeof(logout), 0);
	}
	if(instr == 1) {
		char buff_tmp[100], file_server[100], actual_buffer[BUFFER];
		memset(buff_tmp, '\0', 100);
		recv(sock_fd, buff_tmp, sizeof(buff_tmp), 0);
		sprintf(file_server, "copy_%s", buff_tmp);

		FILE *f_tmp = fopen(file_server, "wb");
		
		int f_len, f_len2, k, going = 0;
		recv(sock_fd, &f_len, sizeof(f_len), 0);

		cout << "Downloading file..." << endl;

		while(f_len > 0 && (k = recv(sock_fd, actual_buffer, BUFFER, 0)) > 0) {
			cout << "actual_buffer (peer_client.h file_opr(): ): " << actual_buffer << endl;
			fwrite(actual_buffer, sizeof(char), k, f_tmp);
			memset(actual_buffer, '\0', BUFFER);
			f_len -= k;
			going += k;
		}
		cout << "FILE DOWNLOADED SUCCESSFULLY !!" << endl;
		fclose(f_tmp);
		pthread_exit(NULL);
	}
	if(instr == 2) {
		int pos, peer_no, ind, k, going = 0;
		recv(sock_fd, &peer_no, sizeof(peer_no), 0);
		char actual_buffer[100], buff2[BUFFER];
		memset(actual_buffer, '\0', sizeof(actual_buffer));

		recv(sock_fd, &ind, sizeof(ind), 0);
		sleep(0.5);
		recv(sock_fd, actual_buffer, sizeof(actual_buffer), 0);

		//cout << actual_buffer << ", " << peer_no << ", " << ind << endl;

		FILE* f_tmp = fopen(actual_buffer, "rb");
		if(!f_tmp) {
			cout << "FILE NOT EXIST !!" << endl;
			return NULL;
		}
		fseek(f_tmp, 0, SEEK_END);
		pos = ftell(f_tmp);
		rewind(f_tmp);

		memset(buff2, '\0', BUFFER);
		int ackn = 0, size = pos, file_size = pos / CHUNK;
		int file_size_tmp = file_size;

		if(pos % CHUNK != 0)
			file_size_tmp += 1;

		size = ceil(size / peer_no);
		cout << "Uploading File..." << endl;
		
		for(int i = ind; i < file_size_tmp * CHUNK; i += peer_no * CHUNK) {
			for(int j = i; j < pos && j < i + CHUNK; j += BUFFER) {
				fseek(f_tmp, j, SEEK_SET);
				k = fread(buff2, sizeof(char), BUFFER, f_tmp);
				send(sock_fd, buff2, k, 0);
				size -= k;
				recv(sock_fd, &ackn, sizeof(ackn), 0);
				memset(buff2, '\0', BUFFER);
			}
		}
		cout << "FILE UPLOAD SUCCESSFUL !!" << endl;
		fclose(f_tmp);
 	}
 	close(sock_fd);
	return NULL;
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
	return NULL;
}