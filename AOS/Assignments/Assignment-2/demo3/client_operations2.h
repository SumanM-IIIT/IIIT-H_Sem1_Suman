#include <stdio.h>
#include <iostream>
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include <openssl/sha.h>
#include <math.h>
using namespace std;

#define BUFFER 1024
#define CHUNK 512*1024

typedef struct Server_Conn {
	int port, user_id;
	char ip[20];
} user_server_conn;

typedef struct ClientInfo {
	int port;
	char ip[20], file_info[100];
} client_info;

typedef struct ChunkInfo {
	int sock_fd, port, len, peer_no, start;
	char ip[20], rd[100], wr[100];
} chunk_info; 

int curr_user = -1;

void* client_request(void* arg) {
	client_info* c = (client_info*)arg;
	char tracker_ip[20], tracker_ip2[20];
	char* tmp;
	int tracker_port, tracker_port2, demo;
	size_t size;

	FILE* f1 = fopen(c->file_info, "r");
	if(!f1) {
		cout << "FILE NOT EXIST !!";
		return NULL;
	}

	demo = getline(&tmp, &size, f1); 
	strcpy(tracker_ip, tmp);
	tracker_ip[strlen(tracker_ip) - 1] = '\0';
	demo = getline(&tmp, &size, f1); 
	tracker_port = stoi(tmp);
	
	demo = getline(&tmp, &size, f1); 
	strcpy(tracker_ip2, tmp);
	tracker_ip2[strlen(tracker_ip2) - 1] = '\0';
	demo = getline(&tmp, &size, f1); 
	tracker_port2 = stoi(tmp); 

	cout << "Connection Established..." << endl;

	while(1) {
		cout << "Client: " << endl;
		int peer_no, len;
		string command, s = "";
		vector<string> cmd; 

		cout << "Enter your Command here: ";
		getline(cin, command);

		for(int i = 0; command[i] != '\0'; i++) {
			if(command[i] == ' ') {
				if(s != "")
					cmd.push_back(s);
				tmp = "";
				continue;
			}
			s += command[i];
		}
		if(s != "")
			cmd.push_back(s);

		struct sockaddr_in address_server;
		address_server.sin_family = AF_INET;
		address_server.sin_port = htons(tracker_port);
		address_server.sin_addr.s_addr = inet_addr(tracker_ip);

		int flag, flag2, sock = socket(AF_INET, SOCK_STREAM, 0);
		if(connect(sock, (struct sockaddr *)&address_server, sizeof(address_server)) < 0) {
			cout << "Tracker-1 Connection Failed !!\nConnecting to Tracker-2..." << endl;
			flag = 1;
		}
		if(flag) {
			address_server.sin_port = htons(tracker_port2);
			address_server.sin_addr.s_addr = inet_addr(tracker_ip2);			
			if(connect(sock, (struct sockaddr *)&address_server, sizeof(address_server)) < 0) 
				cout << "Tracker-2 Connection also Failed !!\nExiting..." << endl;
		}

		if(cmd[0] == "logout") {
			flag2 = 3;
			send(sock, &flag2, sizeof(flag2), 0);
			recv(sock, &curr_user, sizeof(curr_user), 0);
			cout << "Logged Out Successfully.." << endl << endl;
		}
		if(cmd[0] == "show_files") {
			if(curr_user < 0) {
				cout << "You MUST LOG IN to do this operation.." << endl;
				continue;
			}
			flag2 = 10;
			int tmp_len;
			send(sock, &flag2, sizeof(flag2), 0);
			recv(sock, &tmp_len, sizeof(tmp_len), 0);

			char file_name[100];
			for(int i = 0; i < tmp_len; i++) {
				memset(file_name, '\0', sizeof(file_name));
				recv(sock, file_name, sizeof(file_name), 0);
				cout << i + 1 << ". " << file_name << endl;
			}
		}
		if(cmd[0] == "create_user") {
			if(cmd.size() < 3) {
				cout << "Please enter BOTH username & password..." << endl;
				continue;
			}
			flag2 = 4;
			int ackn;
			send(sock, &flag2, sizeof(flag2), 0);
			send(sock, &(c->port), sizeof(c->port), 0);
			send(sock, c->ip, strlen(c->ip), 0);
			recv(sock, &ackn, sizeof(ackn), 0);
			send(sock, cmd[1].c_str(), cmd[1].length(), 0);
			recv(sock, &ackn, sizeof(ackn), 0);
			send(sock, cmd[2].c_str(), cmd[2].length(), 0);
			recv(sock, &ackn, sizeof(ackn), 0);
		}
		if(cmd[0] == "login") {
			if(cmd.size() < 3) {
				cout << "Please enter BOTH username & password..." << endl;
				continue;
			}
			flag2 = 5;
			int ackn;
			send(sock, &flag2, sizeof(flag2), 0);
			send(sock, &(c->port), sizeof(c->port), 0);
			send(sock, c->ip, strlen(c->ip), 0);
			recv(sock, &ackn, sizeof(ackn), 0);
			send(sock, cmd[1].c_str(), cmd[1].length(), 0);
			recv(sock, &ackn, sizeof(ackn), 0);
			send(sock, cmd[2].c_str(), cmd[2].length(), 0);
			recv(sock, &ackn, sizeof(ackn), 0);
			curr_user = ackn;

			if(ackn < 0)
				cout << "INVALID USER !!" << endl;
			else if(ackn == 0) {
				curr_user = -1;
				cout << "LOGIN FAILED !! User already logged in somewhere else.." << endl;
			}
			else {
				cout << "LOGIN SUCCESSFUL !!" << endl;
			}
		}
		if(cmd[0] == "upload") {
			if(curr_user < 0) {
				cout << "You MUST LOG IN to do this operation.." << endl;
				continue;
			}
			flag2 = 1;
			string dem = cmd[1];
			int size, pos, ackn, group_id = stoi(cmd[2]);

			FILE *f2 = fopen(dem.c_str(), "rb");
			if(!f2) {
				cout << "FILE NOT EXIST !!" << endl;
				continue; 
			}
			fseek(f2, 0, SEEK_END);
			pos = ftell();
			rewind(f2);

			size = ceil(s * 1.0 / CHUNK);

			char tmp_n[size + 1];
			memset(tmp_n, '1', size);
			tmp_n[size] = '\0';

			send(sock, &flag2, sizeof(flag2), 0);
			send(sock, &(c->port), sizeof(c->port), 0);
			send(sock, &group_id, sizeof(group_id), 0);
			recv(sock, &ackn, sizeof(ackn), 0);
			send(sock, c->ip, strlen(c->ip), 0);
			recv(sock, &ackn, sizeof(ackn), 0);
			send(sock, dem.c_str(), dem.length(), 0);
			recv(sock, &ackn, sizeof(ackn), 0);
			send(sock, &s, sizeof(s), 0);
			recv(sock, &ackn, sizeof(ackn), 0);

			cout << tmp_n << endl;
			send(sock, tmp_n, size, 0);
			recv(sock, &ackn, sizeof(ackn), 0);

			cout << "File SUCCESSFULLY Uploaded !!" << endl;
			fclose(f2);
		}
		if(cmd[0] == "download") {
			continue;
		}
	}
}