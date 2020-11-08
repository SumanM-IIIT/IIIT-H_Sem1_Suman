#include <unistd.h>
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

int all_ports[50] = {-1};
char all_ips[50][20];

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

bool all_port_empty(int* all_ports) {
	int flag = 0;
	for(int i = 0; i < 50; i++) {
		if(all_ports[i] != -1) {
			flag = 1;
			break;
		}
	}
	if(flag == 0) 
		return true;
	else
		return false;
}

void* file_copy(void* arg) {
	chunk_info *ci = (chunk_info*)arg;
	int file_size, flag, size_dem = ci->len, port_dem = ci->port, start_dem = ci->start, peer_no_dem = ci->peer_no;
	char *ip_dem = ci->ip;
	char *file_src = ci->rd, *file_dest = ci->wr;

	struct sockaddr_in address_server;
	address_server.sin_family = AF_INET;
	address_server.sin_port = htons(port_dem);
	address_server.sin_addr.s_addr = inet_addr(ip_dem);

	int sock_dem = socket(AF_INET, SOCK_STREAM, 0);
	if(connect(sock_dem, (struct sockaddr *)&address_server, sizeof(address_server)) < 0) {
		cout << "CONNECTION FAILED WHILE DOWNLOADING !! Exiting.." << endl;
		exit(1);
	}
	flag = 2;
	send(sock_dem, &flag, sizeof(flag), 0);
	sleep(0.5);
	send(sock_dem, &peer_no_dem, sizeof(peer_no_dem), 0);
	sleep(0.5);
	send(sock_dem, &start_dem, sizeof(start_dem), 0);
	sleep(0.5);
	send(sock_dem, file_src, strlen(file_src), 0);
	sleep(0.5);

	FILE* f_dem = fopen(file_dest, "rb+");
	file_size = size_dem / CHUNK;

	int size2, file_size_act = file_size;
	if(size_dem % CHUNK != 0)
		file_size_act++;

	char actual_buffer[CHUNK];
	memset(actual_buffer, '\0', CHUNK);
	size2 = ceil(size_dem / peer_no_dem);

	for(int i = start_dem; i < file_size_act * CHUNK; i += peer_no_dem * CHUNK) {
		int ackn = 0, tmp = 0;
		for(int j = i; (j < i + CHUNK) && (j < size_dem); j += BUFFER) {
			char buff2[BUFFER];
			memset(buff2, '\0', BUFFER);
			int k = recv(sock_dem, buff2, BUFFER, 0);

			for(int m = 0; m < k; m++) 
				actual_buffer[tmp++] = buff2[m];

			size2 -= k;
			send(sock_dem, &ackn, sizeof(ackn), 0);
		}
		fseek(f_dem, i, SEEK_SET);
		fwrite(actual_buffer, sizeof(char), tmp, f_dem);
	}
	fclose(f_dem);
	cout << "FILE COPY THREAD DONE !!" << endl;
	return NULL;
}
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
		//cout << "Client: " << endl;
		int peer_no, len;
		string command, s = "";
		vector<string> cmd; 

		cout << "Enter your Command here: ";
		getline(cin, command);

		for(int i = 0; command[i] != '\0'; i++) {
			if(command[i] == ' ') {
				if(s != "")
					cmd.push_back(s);
				s = "";
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

		int flag, flag2;
		int sock = socket(AF_INET, SOCK_STREAM, 0);

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
			int tmp_len = 0;
			send(sock, &flag2, sizeof(flag2), 0);
			recv(sock, &tmp_len, sizeof(tmp_len), 0);

			cout << "tmp_len: " << tmp_len << endl;
			for(int i = 0; i < tmp_len; i++) {
				char file_name[105];
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
			pos = ftell(f2);
			rewind(f2);

			size = ceil(pos * 1.0 / CHUNK);

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

			//cout << tmp_n << endl;
			send(sock, tmp_n, size, 0);
			recv(sock, &ackn, sizeof(ackn), 0);

			cout << "File SUCCESSFULLY Uploaded !!" << endl;
			fclose(f2);
		}
		if(cmd[0] == "download") {
			if(curr_user < 0) {
				cout << "You MUST LOG IN to do this operation.." << endl;
				continue;
			}
			flag2 = 2;
			int pos, group_id = stoi(cmd[1]);
			char *ch1, *ch2;

			strcpy(ch1, cmd[2].c_str());
			strcpy(ch2, cmd[3].c_str());

			send(sock, &flag2, sizeof(flag2), 0);
			send(sock, ch1, strlen(ch1), 0);
			recv(sock, &peer_no, sizeof(peer_no), 0);
			recv(sock, &pos, sizeof(pos), 0);

			cout << "Peers: " << peer_no << ", Size: " << pos << endl;

			for(int i = 0; i < peer_no; i++) {
				int dem2, ackn = 0, port_tmp3;
				char ip_tmp3[20];
				memset(ip_tmp3, '\0', 20);
				recv(sock, &port_tmp3, sizeof(port_tmp3), 0);
				send(sock, &ackn, sizeof(ackn), 0);
				dem2 = recv(sock, ip_tmp3, 20, 0);
				send(sock, &ackn, sizeof(ackn), 0);
				
				all_ports[i] = port_tmp3;
				strcpy(all_ips[i], ip_tmp3);
				cout << "Dem2: " << dem2 << ", IP: " << all_ips[i] << ", Port: " << all_ports[i] << endl;
			}

			if(pos == 0 || all_port_empty(all_ports)) { 
				cout << "FILE ABSENT !!" << endl;
				continue;
			} 

			char ch1_tmp[100], f_cli[100], actual_buffer[BUFFER];
			strcpy(ch1_tmp, ch1);

			sprintf(f_cli, "%s%s", ch2, ch1_tmp);

			FILE *file1 = fopen(f_cli, "wb");
			memset(actual_buffer, '\0', BUFFER);

			int f_size_act, file_size = pos / BUFFER;
			f_size_act = file_size;
			cout << "File Size: " << file_size << endl;

			while(file_size > 0) {
				fwrite(actual_buffer, 1, BUFFER, file1);
				file_size--;
			}
			file_size = pos % BUFFER;

			char c_tmp = '\0';
			while(file_size > 0) {
				fwrite(&c_tmp, 1, 1, file1);
				file_size--;
			}
			fclose(file1);

			memset(ch1_tmp, '\0', 100);
			strcpy(ch1_tmp, ch1);

			int size_ac = ceil(pos * 1.0 / CHUNK);
			char ch_arr[size_ac + 1];
			memset(ch_arr, '0', size_ac);
			ch_arr[size_ac] = '\0';

			int ackn = 0;

			send(sock, &(c->port), sizeof(c->port), 0);
			send(sock, &group_id, sizeof(group_id), 0);
			recv(sock, &ackn, sizeof(ackn), 0);
			send(sock, c->ip, strlen(c->ip), 0);
			recv(sock, &ackn, sizeof(ackn), 0);
			send(sock, ch1_tmp, strlen(ch1_tmp), 0);
			recv(sock, &ackn, sizeof(ackn), 0);
			send(sock, &pos, sizeof(pos), 0);
			recv(sock, &ackn, sizeof(ackn), 0);
			
			send(sock, ch_arr, size_ac, 0);
			recv(sock, &ackn, sizeof(ackn), 0);

			pthread_t th[peer_no];
			chunk_info ci[peer_no];		

			for(int i = 0; i < peer_no; i++) {
				ci[i].port = all_ports[i];
				strcpy(ci[i].ip, all_ips[i]);
				ci[i].len = pos;
				ci[i].start = i * CHUNK;
				ci[i].peer_no = peer_no;

				strcpy(ci[i].rd, ch1_tmp);
				strcpy(ci[i].wr, f_cli);

				if(pthread_create(&th[i], NULL, file_copy, (void *)&ci[i])) {
					cout << "CHUNK THREAD ERROR !! Exiting.." << endl;
					exit(1);
				}
			}
			for(int i = 0; i < peer_no; i++) {
				pthread_join(th[i], NULL);
			}
			cout << "FILE DOWNLOADED SUCCESSFULLY !!" << endl;
		}
		close(sock);
	}
}