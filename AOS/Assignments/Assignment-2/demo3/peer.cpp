#include <iostream>
#include <string.h>
#include <bits/stdc++.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "peer_client.h"
#include "peer_server.h"
using namespace std;

int main(int argc, char** argv) {
	if(argc < 4) {
		cout << "TOO FEW ARGUMENTS !! Exiting.." << endl;
		exit(1);
	}

	client_info c;
	strcpy(c.ip, argv[1]);
	c.port = stoi(argv[2]);
	strcpy(c.file_info, argv[3]);
	int port = c.port;

	pthread_t t1, t2;

	if(pthread_create(&t1, NULL, client_listener, (void *)&port)) {
		cout << "CLIENT THREAD ERROR 1 !! Exiting.." << endl;
		exit(1);
	}
	if(pthread_create(&t2, NULL, client_request, (void *)&c)) {
		cout << "CLIENT THREAD ERROR 2 !! Exiting.." << endl;
		exit(1);
	}
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);

	return 0;
}