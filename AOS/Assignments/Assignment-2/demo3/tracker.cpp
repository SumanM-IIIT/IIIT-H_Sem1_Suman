#include <iostream>
#include <bits/stdc++.h>
#include <fstream>
using namespace std;

typedef long long ll;

#define BUFFER 1024
#define CHUNK 512*1024

typedef struct curr_tracker_info {
	int port;
	char ip[16];
} Tracker;

void* file_opr(void *args) {
	int instr, sock_fd = *((int *)args);

	recv(sock_fd, &instr, sizeof(instr), 0);

	
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

	int add_len = sizeof(addr); //sockaddr OR addr

	if(bind(server, (struct sockaddr *)&address, sizeof(address)) < 0) {
		cout << "PORT NOT AVAILABLE !!" << endl;
		exit(1);
	}
	listen(server, 100);

	while(1) {
		int sock_fd = accept(server, (struct sockaddr*)&address, (socklen_t*)&add_len);
		cout << "Connection Established..." << endl;

		pthread_t t_new;
		if(pthread_create(&t_new, NULL, file_opr, (vdoi *)&sock_fd)) {
			cout << "SOME ERROR !!" << endl;
			exit(1);
		}
		if(logout)
			break;
	}
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
		tr.ip = ip_port[0].c_str();
	}
	else if(tracker_no == 2) {
		tr.port = stoi(ip_port[3]);
		tr.ip = ip_port[2].c_str();
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