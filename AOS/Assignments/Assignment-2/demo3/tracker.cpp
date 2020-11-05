#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
#include <pthread.h>
#include <sys/socket.h>
#include <arpa/inet.h>
using namespace std;

typedef long long ll;

#define BUFFER 1024
#define CHUNK 512*1024

typedef struct curr_tracker_info {
	int port;
	char ip[20];
} Tracker;

typedef struct peer_info {
	char ip[20];
	int port;
	vector<char> chunk_v;
} peers;

typedef struct user_info {
	char username[100], password[100], ip[20];
	int port;
} UserInfo;

int logout = 0, curr_user = -1, curr_group = -1, group_count = 0, user_count = 0;
map<int, int> login_status_map;
map<string, vector<peers>> file_peer_map; 
map<int, UserInfo> user_info_map;

void* file_opr(void *args) {
	int instr, sock_fd = *((int *)args);

	recv(sock_fd, &instr, sizeof(instr), 0);

	if(instr == 3) { //LOGOUT
		login_status_map[curr_user] = 0;
		curr_user = -1;
		send(sock_fd, &curr_user, sizeof(curr_user), 0);
		return NULL;
	}
	if(instr == 10) { //SHOW FILES
		int size_m = file_peer_map.size();
		send(sock_fd, &size_m, sizeof(size_m), 0);
		for(auto i = file_peer_map.begin(); i != file_peer_map.end(); i++) {
			send(sock_fd, i->first.c_str(), sizeof(i->first.c_str()), 0);
		}
	}
	if(instr == 4) { //CREATE USER
		user_count++;
		int ackn, port_tmp;
		char username[100], password[100], ip_tmp[20];

		recv(sock_fd, &port_tmp, sizeof(port_tmp), 0);
		recv(sock_fd, ip_tmp, sizeof(ip_tmp), 0);
		send(sock_fd, &ackn, sizeof(ackn), 0);
		recv(sock_fd, username, sizeof(username), 0);
		send(sock_fd, &ackn, sizeof(ackn), 0);
		recv(sock_fd, password, sizeof(password), 0);
		send(sock_fd, &ackn, sizeof(ackn), 0);

		cout << ip_tmp << ":" << port_tmp << " Uname: " << username << ", Pswd: " << password << endl;

		UserInfo u1;
		u1.port = port_tmp;
		strcpy(u1.ip, ip_tmp);
		strcpy(u1.username, username);
		strcpy(u1.password, password);

		user_info_map.insert(make_pair(user_count, u1));
	}
	if(instr == 5) { //LOGIN
		char username[100], password[100], ip_tmp2[20];
		int ackn, port_tmp2;

		recv(sock_fd, &port_tmp2, sizeof(port_tmp2), 0);
		recv(sock_fd, ip_tmp2, sizeof(ip_tmp2), 0);
		send(sock_fd, &ackn, sizeof(ackn), 0);
		recv(sock_fd, username, sizeof(username), 0);
		send(sock_fd, &ackn, sizeof(ackn), 0);
		recv(sock_fd, password, sizeof(password), 0);
		
		cout << ip_tmp2 << ":" << port_tmp2 << " Uname: " << username << ", Pswd: " << password << endl;		

		for(auto i = user_info_map.begin(); i != user_info_map.end(); i++) {
			int u_no = i->first;
			UserInfo &u2 = i->second;
			strcpy(u2.ip, ip_tmp2);
			u2.port = port_tmp2;

			if(strcmp(username, u2.username) == 0 && strcmp(password, u2.password) == 0) {
				if(login_status_map.find(u_no) == login_status_map.end()) {
					login_status_map.insert(make_pair(u_no, 1));
					curr_user = u_no;
					send(sock_fd, &curr_user, sizeof(curr_user), 0);
				}
				else {
					if(login_status_map[u_no] == 1) {
						int ack_tmp = 0;
						send(sock_fd, &ack_tmp, sizeof(ack_tmp), 0);
					}
					else {
						login_status_map[u_no] = 1;
						curr_user = u_no;
						send(sock_fd, &curr_user, sizeof(curr_user), 0);
					}
				}
				return NULL;
			}
		}
		int ack_tmp = -1;
		send(sock_fd, &ack_tmp, sizeof(ack_tmp), 0);
		return NULL;
	}
	if(instr == 1) { //UPLOAD
		cout << "Upload !!" << endl;
	}
	if(instr == 2) { //DOWNLOAD
		cout << "Download !!" << endl;
	}
	return NULL;	
}


void* tracker_exit(void* client_args) {
	string tmp;
	while(1) {
		cin >> tmp;
		if(tmp == "exit") {
			cout << "Tracker is Exiting..." << endl;
			exit(0);
		}
	}
}

void* listener(void* server_args) {
	Tracker *t = (Tracker*)server_args;
	char ip[16];
	strcpy(ip, t->ip);
	int port = t->port;

	cout << "Listening on Port: " << port << "..." << endl;

	int server = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in address;

	address.sin_family = AF_INET;
	address.sin_port = htons(port);
	address.sin_addr.s_addr = INADDR_ANY;

	int add_len = sizeof(sockaddr); //sockaddr OR addr

	if(bind(server, (struct sockaddr *)&address, sizeof(address)) < 0) {
		cout << "PORT NOT AVAILABLE !!" << endl;
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


int main(int argc, char** argv) {
	if(argc <= 2) {
		cout << "TOO FEW ARGUMENTS !!" << endl;
		exit(1);
	}

	int tracker_no = atoi(argv[2]);
	ifstream f_in;
	f_in.open(argv[1]);
	if(!f_in) {
		cout << "UNABLE TO OPEN THE TRACKER FILE !!" << endl;
		exit(1);
	}
	string a;
	vector<string> ip_port;
	while(f_in >> a) {
		ip_port.push_back(a);
	}
	f_in.close();

	Tracker tr;
	if(tracker_no == 1) {
		tr.port = stoi(ip_port[1]);
		//tr.ip = ip_port[0].c_str();
		strcpy(tr.ip, ip_port[0].c_str());
	}
	else if(tracker_no == 2) {
		tr.port = stoi(ip_port[3]);
		//tr.ip = ip_port[2].c_str();
		strcpy(tr.ip, ip_port[2].c_str());
	}
	else {
		cout << "INVALID TRACKER NUMBER !!" << endl;
		exit(1);
	}

	pthread_t t1, t2;
	if(pthread_create(&t1, NULL, tracker_exit, NULL)) {
		cout << "THREAD-1 ERROR !!" << endl;
		exit(1);
	}
	if(pthread_create(&t2, NULL, listener, (void*)&tr)) {
		cout << "THREAD-2 ERROR !!" << endl;
		exit(1);
	}
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	cout << "Tracker OUT OF all the threads.." << endl;
	return 0;
}