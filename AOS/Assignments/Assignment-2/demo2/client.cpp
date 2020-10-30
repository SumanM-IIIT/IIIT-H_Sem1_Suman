#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <bits/stdc++.h>
#include <iostream>
using namespace std;

void * doRecieving(void * sockID){

	int clientSocket = *((int *) sockID);

	while(1){

		char data[1024];
		int read = recv(clientSocket,data,1024,0);
		data[read] = '\0';
		printf("%s\n",data);
		string tmp = "", input(data);
		vector<string> cmd;
		for(int i = 0; i < input.length(); i++) {
			if(input[i] == ' ') {
				if(tmp != "")
					cmd.push_back(tmp);
				tmp = "";
				continue;
			}
			tmp += input[i];
		}	
		if(tmp != "")
			cmd.push_back(tmp);

		if(cmd.size() == 4) {
			cout << "clientSocket: " << clientSocket << endl;
			cout << "cmd[3]: " << cmd[3] << endl;
			if(cmd[0] == "accept_group_join") {
				//send(clientSocket,input.c_str(),1024,0);
				//int read = recv(clientSocket,data,2048,0);
				//data[read] = '\0';
				//string d(data);
				//cout << d;
				string c;
				cout << cmd[1] << " wants to join group: " << cmd[2] << "; Allow? (yes/no): \n";
				cin >> c;
				send(stoi(cmd[3]),c.c_str(),1024,0);
			}
		}
		/*if(strcmp(data, "yes") == 0) {
			cout << "My clientSocket: " << clientSocket << endl;
		}*/
	}

}

int main(int argc, char** argv) {

	int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[2]));
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);

	if(connect(clientSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) 
		return 0;

	printf("Connection established ............\n");

	pthread_t thread;
	pthread_create(&thread, NULL, doRecieving, (void *) &clientSocket );

	while(1){

		//char input[1024];
		//scanf("%s",input);
		string input, tmp = "";
		getline(cin, input);
		char data[2048];
		

		vector<string> cmd;
		for(int i = 0; i < input.length(); i++) {
			if(input[i] == ' ') {
				if(tmp != "")
					cmd.push_back(tmp);
				tmp = "";
				continue;
			}
			tmp += input[i];
		}	
		if(tmp != "")
			cmd.push_back(tmp);
		/*string inp, tmp = "";
		vector<string> cmd;
		getline(cin, inp);
		for(int i = 0; inp[i] != '\0'; i++) {
			if(inp[i] == ' ') {
				if(tmp != "")
					cmd.push_back(tmp);
				tmp = "";
				continue;
			}
			tmp += inp[i];
		}

		if(cmd[0] == "create_user") {
			send(clientSocket, inp.c_str(), inp.length(), 0);
		}
		

		else */
		if(cmd[0] == "create_user") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		if(cmd[0] == "login") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		if(cmd[0] == "logout") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		if(cmd[0] == "create_group") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		if(cmd[0] == "fetch_groups") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		if(cmd[0] == "request_group_join") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		if(cmd[0] == "leave_group") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		if(cmd[0] == "yes") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		if(cmd[0] == "no") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		
		if(cmd[0] == "LIST"){

			send(clientSocket,input.c_str(),1024,0);

		}
		if(cmd[0] == "exit") {
			break;
		}
		else if(cmd[0] == "SEND"){

			send(clientSocket,input.c_str(),1024,0);
			
			//scanf("%s",input);
			getline(cin, input);
			send(clientSocket,input.c_str(),1024,0);
			
			//scanf("%[^\n]s",input);
			getline(cin, input);
			send(clientSocket,input.c_str(),1024,0);

		}
		else
			continue;

	}


}