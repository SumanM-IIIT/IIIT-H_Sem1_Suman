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
#include <signal.h>
using namespace std;

#define SIZE 1024

int sig_val = 0;
void sigintHandler(int sig_num) {
	sig_val = 1;
}
void send_file(FILE *fp, int sockfd){
  int n;
  char data[SIZE] = {0};

  while(fgets(data, SIZE, fp) != NULL) {
    if (send(sockfd, data, sizeof(data), 0) == -1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, SIZE);
  }
}
void write_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "recv.txt";
  char buffer[SIZE];

  fp = fopen(filename, "w");
  while (1) {
    n = recv(sockfd, buffer, SIZE, 0);
    if (n <= 0){
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  return;
}

void *receive(void * arg) {
	char **argv = (char **)arg;
	char *ip = argv[1];
	int port = 6000;
	int clientSocket = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serverAddr;

	//port = stoi(tr_arr[1]);
	//ip =tr_arr[0];
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(ip);
	int flag = 0;
	if(bind(clientSocket,(struct sockaddr *) &serverAddr , sizeof(serverAddr)) == -1) 
		flag = 1;
	if(listen(clientSocket,1024) == -1) 
		flag = 1;
	if(flag == 1) {
		cout << "Peer-Server Connection failed !!" << endl;
		return 0;
	}
	cout << "Peer is listenting on port " << port << "..." << endl;




	//int clientSocket = *((int *) sockID);
	/*while(1) {
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
	}*/
	return NULL;
}

void *sending(void * arg) {
	char **argv = (char **)arg;
	char *ip = argv[1];
	int exit_flag = 0, port = stoi(argv[2]);
	fstream f_in;
	f_in.open(argv[3]);
	string tr;
	vector<string> tr_arr;
	while(f_in >> tr) {	
		tr_arr.push_back(tr);
	}
	int clientSocket = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(stoi(tr_arr[1]));
	serverAddr.sin_addr.s_addr = inet_addr(tr_arr[0].c_str());

	if(connect(clientSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) {
		cout << "Connection to Tracker-1 Failed !! Connecting to Tracker-2..." << endl;
		serverAddr.sin_port = htons(stoi(tr_arr[3]));
		serverAddr.sin_addr.s_addr = inet_addr(tr_arr[2].c_str());
		if(connect(clientSocket, (struct sockaddr*) &serverAddr, sizeof(serverAddr)) == -1) {
			cout << "Connection to Tracker-2 also Failed !! Exiting..." << endl;
			return 0;
		}
	}

	cout << "Connection established...\n";


	//int exit_flag = 0;
	//int clientSocket = *(int *)sockID;
	while(1) {
		signal(SIGINT, sigintHandler);
		//char input[1024];
		//scanf("%s",input);
		cout << "Enter Command: ";
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
		
		if(cmd[0] == "create_user") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		else if(cmd[0] == "login") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		else if(cmd[0] == "logout") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		else if(cmd[0] == "create_group") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		else if(cmd[0] == "fetch_groups") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		else if(cmd[0] == "request_group_join") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		else if(cmd[0] == "leave_group") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		else if(cmd[0] == "upload_file") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		else if(cmd[0] == "show_files") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		else if(cmd[0] == "download_file") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		else if(cmd[0] == "yes") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		else if(cmd[0] == "no") {
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << d;
		}
		else if(cmd[0] == "exit" || cmd[0] == "quit" || sig_val) {
			//lose(clientSocket);
			//cout << "Both Tracker & Peer are exiting..." << endl;
			send(clientSocket,input.c_str(),1024,0);
			int read = recv(clientSocket,data,2048,0);
			data[read] = '\0';
			string d(data);
			cout << "Peer <" << d << "> exiting..." << endl;
			exit_flag = 1;
			//close(clientSocket);
			break;
		}
		else {
			cout << "INVALID COMMAND !! Please enter again..." << endl;
			continue;
		}
	}
	sigintHandler(2);
	return NULL;
}

int main(int argc, char** argv) {
	
	pthread_t thread1, thread2;
	pthread_create(&thread2, NULL, sending, (void *) argv);
	//pthread_create(&thread2, NULL, receive, (void *) argv);

	
	//pthread_join(thread, NULL);
	/*if(exit_flag == 1) {
		pthread_exit();
	}*/
	while(1) {
		if(sig_val == 1) {
			cout << "Bye\n";
			break;
		}
	}
	return 0;
}