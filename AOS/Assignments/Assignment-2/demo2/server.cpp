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

int clientCount = 0;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

struct client {
	int index;
	int sockID;
	struct sockaddr_in clientAddr;
	unsigned int len;
	//string username = "", password = "";
	//bool is_login = true;
	//string own_group = "";
};

typedef struct user {
	string username = "", password = "";
	bool is_login = true;
	string own_group = "";
	vector<string> group_part;
}User;

struct client Client[1024];
pthread_t my_thread[1024];
vector<User> usr;

void * doNetworking(void * ClientDetail){

	struct client* clientDetail = (struct client*) ClientDetail;
	int index = clientDetail -> index;
	int clientSocket = clientDetail -> sockID;

	printf("Client %d connected.\n",index + 1);

	while(1){

		char data[1024], output[1024];
		int read = recv(clientSocket,data,1024,0);
		data[read] = '\0';
		
		string tmp = "";
		vector<string> cmd;
		for(int i = 0; data[i] != '\0'; i++) {
			if(data[i] == ' ') {
				if(tmp != "")
					cmd.push_back(tmp);
				//cout << "Aise hi: " << tmp << endl;
				tmp = "";
				continue;
			}
			tmp += data[i];
		}
		if(tmp != "")
			cmd.push_back(tmp);



		if(cmd[0] == "create_user") {
			int flag = 0;
			for(int i = 0; i < usr.size(); i++) {
				if(usr[i].username == cmd[1]) {
					flag = 1; 
					break;
				}
			}
			if(flag == 0) {
				User u = new User();
				u.username = cmd[1];
				u.password = cmd[2];
				usr.push_back(u);
				cout << "Client-" << Client[index].index + 1 << ": User: <" << cmd[1] << "> created successfully !!" << endl;
			}
			else 
				cout << "Client-" << Client[index].index + 1 << ": GIVEN USER ALREADY PRESENT !!" << endl;
			/*if(Client[index].username == "" && Client[index].password == "") {
				Client[index].username = cmd[1];
				Client[index].password = cmd[2];
				cout << "Client-" << Client[index].index + 1 << ": User: <" << cmd[1] << "> created successfully !!" << endl;
			}
			else 
				continue;*/
		}
		if(cmd[0] == "login") {
			int flag = 0;
			for(int i = 0; i < usr.size(); i++) {
				if(usr[i].username == cmd[1] && usr[i].password == cmd[2]) {
					if(usr[i].is_login) {
						usr[i].is_login = false;
						cout << "Client-" << Client[index].index + 1 << ": User: <" << usr[i].username << "> LOGIN SUCCESSFUL !!" << endl;
					}
					else
						cout << "User: <" << usr[i].username << "> ALREADY LOGGED IN SOMEWHERE..." << endl;
					flag = 1; 
					break;
				}
			}
			if(flag == 0)
				cout << "Client-" << Client[index].index + 1 << ": INVALID USERNAME/PASSWORD..." << endl;


			/*if(Client[index].username == cmd[1] && Client[index].password == cmd[2]) {
				if(Client[index].is_login) {
					cout << "Client-" << Client[index].index + 1 << ": User: <" << cmd[1] << ">: LOGGED IN successfully !!" << endl;
					Client[index].is_login = false;
				}
				//cout << "User: <" << cmd[1] << "> created successfully !!" << endl;
				else
					cout << "Client-" << Client[index].index + 1 << ": User: <" << cmd[1] << ">: Already LOGGED In..." << endl;
			}
			else {
				cout << "Client-" << Client[index].index + 1 << "Invalid Credentials !!! Please try again..." << endl;
				continue;
			}*/
		}
		if(cmd[0] == "logout") {
			int flag = 0;
			for(int i = 0; i < usr.size(); i++) {
				if(usr[i].username == cmd[1]) {
					if(!usr[i].is_login) {
						usr[i].is_login = true;
						cout << "Client-" << Client[index].index + 1 << ": User: <" << usr[i].username << "> LOGOUT SUCCESSFUL !!" << endl;
					}
					else 
						cout << "User: <" << usr[i].username << "> ALREADY LOGGED OUT..." << endl;
					flag = 1;
					break;
				}
			}
			if(flag == 0)
				cout << "Client-" << Client[index].index + 1 << ": INVALID USERNAME/PASSWORD..." << endl;
			/*if(!Client[index].is_login) {
				Client[index].is_login = true;
				cout << "Client-" << Client[index].index + 1 << ": User: <" << cmd[1] << ">: LOGOUT Successful !!" << endl;
			}
			else {
				cout << "Client-" << Client[index].index + 1 << ": User: <" << cmd[1] << ">: Already LOGGED OUT" << endl;
			}*/
		}
		if(cmd[0] == "create_group") {
			if(Client[index].own_group == "") {
				Client[index].own_group = cmd[1];
				cout << "Client-" << Client[index].index + 1 << ": User: <" << cmd[1] << ">: Group Created: " << cmd[1] << endl;
			}
			else {
				cout << "CANNOT create group. GROUP already present for this Client !!" << endl;
			}
		}
		/*
		if(cmd[0] == "login") {
			if(Client[index].is_login) {
				if(cmd[1] == Client[index].username && cmd[2] == Client[index].password)
					cout << Client[index].username << " LOGGED IN successfully !!" << endl;
				else {
					cout << "Invalid Username/Password !!" << endl;
					continue;
				}
			}
			else {
				cout << "Already Logged In..." << endl;
				continue;
			}
		}*/


		if(strcmp(data,"LIST") == 0){

			int l = 0;

			for(int i = 0 ; i < clientCount ; i ++){

				if(i != index)
					l += snprintf(output + l,1024,"Client %d is at socket %d.\n",i + 1,Client[i].sockID);

			}

			send(clientSocket,output,1024,0);
			continue;

		}
		if(strcmp(data,"SEND") == 0){

			read = recv(clientSocket,data,1024,0);
			data[read] = '\0';

			int id = atoi(data) - 1;

			read = recv(clientSocket,data,1024,0);
			data[read] = '\0';

			send(Client[id].sockID,data,1024,0);			

		}

	}

	return NULL;

}

int main(int argc, char** argv) {

	int serverSocket = socket(PF_INET, SOCK_STREAM, 0);

	struct sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(atoi(argv[2]));
	serverAddr.sin_addr.s_addr = inet_addr(argv[1]);


	if(bind(serverSocket,(struct sockaddr *) &serverAddr , sizeof(serverAddr)) == -1) 
		return 0;

	if(listen(serverSocket,1024) == -1) 
		return 0;

	printf("Server started listenting on port %d...........\n", atoi(argv[2]));

	while(1){

		Client[clientCount].sockID = accept(serverSocket, (struct sockaddr*) &Client[clientCount].clientAddr, &Client[clientCount].len);
		Client[clientCount].index = clientCount;

		pthread_create(&my_thread[clientCount], NULL, doNetworking, (void *) &Client[clientCount]);

		clientCount++;
 
	}

	for(int i = 0 ; i < clientCount ; i++)
		pthread_join(my_thread[i],NULL);

}