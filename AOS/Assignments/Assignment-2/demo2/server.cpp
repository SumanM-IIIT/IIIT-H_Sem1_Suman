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
	string username = "", password = "";
	//bool is_login = true;
	//string own_group = "";
};
typedef struct group {
	string owner, name;
	vector<string> members;
} Group;

typedef struct user {
	string username = "", password = "";
	bool is_login = true;
	vector<string> group_part, own_group;
} User;

struct client Client[1024];
vector<Group*> groups;
pthread_t my_thread[1024];
vector<User*> usr;
map<string, string> group_map;

void * doNetworking(void * ClientDetail){

	struct client* clientDetail = (struct client*) ClientDetail;
	int index = clientDetail->index;
	int clientSocket = clientDetail->sockID;

	printf("Client %d connected.\n", index + 1);

	while(1) {
		char data[1024], output[1024];
		int read = recv(clientSocket,data,1024,0);
		data[read] = '\0';	
		string tmp = "";
		vector<string> cmd;
		for(int i = 0; data[i] != '\0'; i++) {
			if(data[i] == ' ') {
				if(tmp != "")
					cmd.push_back(tmp);
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
				if(usr[i]->username == cmd[1]) {
					flag = 1; 
					break;
				}
			}
			string msg = "";
			if(flag == 0) {
				User *u = new User();
				u->username = cmd[1];
				u->password = cmd[2];
				usr.push_back(u);
				msg += "Client-" + to_string(Client[index].index + 1) + ": User: <" + cmd[1] + "> created successfully !!\n";
				cout << msg;
			}
			else 
				msg += "Client-" + to_string(Client[index].index + 1) + ": GIVEN USER ALREADY PRESENT !!\n";
			send(Client[index].sockID, msg.c_str(), 2048, 0);
		}
		if(cmd[0] == "login") {
			int flag = 0;
			string msg = "";
			for(int i = 0; i < usr.size(); i++) {
				if(usr[i]->username == cmd[1] && usr[i]->password == cmd[2]) {
					if(usr[i]->is_login) {
						usr[i]->is_login = false;
						Client[index].username = cmd[1];
						Client[index].password = cmd[2];
						msg += "Client-" + to_string(Client[index].index + 1) + ": User: <" + usr[i]->username + "> LOGIN SUCCESSFUL !!\n";
					}
					else
						msg += "User: <" + usr[i]->username + "> ALREADY LOGGED IN SOMEWHERE...\n";
					cout << msg;
					flag = 1; 
					break;
				}
			}
			if(flag == 0)
				msg += "Client-" + to_string(Client[index].index + 1) + ": INVALID USERNAME/PASSWORD...\n";
			send(Client[index].sockID, msg.c_str(), 2048, 0);
		}
		if(cmd[0] == "logout") {
			int flag = 0;
			string msg = "";
			for(int i = 0; i < usr.size(); i++) {
				if(usr[i]->username == Client[index].username) {
					if(!usr[i]->is_login) {
						usr[i]->is_login = true;
						Client[index].username = "";
						Client[index].password = "";
						msg += "Client-" + to_string(Client[index].index + 1) + ": User: <" + usr[i]->username + "> LOGOUT SUCCESSFUL !!\n";
						cout << msg;
					}
					else 
						msg += "User: <" + usr[i]->username + "> ALREADY LOGGED OUT...\n";
					flag = 1;
					break;
				}
			}
			if(flag == 0)
				msg += "No User was LOGGED IN...\n";
			send(Client[index].sockID, msg.c_str(), 2048, 0);
		}
		if(cmd[0] == "create_group") {
			string msg = "";
			//int flag = 0;
			if(Client[index].username != "") {
				//int flag = 0;
				for(int i = 0; i < usr.size(); i++) {
					if(Client[index].username == usr[i]->username) {
						//if(usr[i]->own_group == "") {
							usr[i]->own_group.push_back(cmd[1]);
							usr[i]->group_part.push_back(cmd[1]);
							//usr[i]->my_group_members.push_back(usr[i]->username);
							Group* g = new Group();
							g->name = cmd[1];
							g->owner = usr[i]->username;
							g->members.push_back(usr[i]->username);
							groups.push_back(g);
							group_map.insert(pair<string, string>(cmd[1], usr[i]->username));
						/*}
						else {
							cout << "Client-" << Client[index].index + 1 << ": User: <" << usr[i]->username << "> already CREATED/OWNS a group..." << endl;
						}*/
						msg += "Client-" + to_string(Client[index].index + 1) + ": User: <" + usr[i]->username + "> has CREATED and OWNS the group: " + cmd[1] + "\n";
						//flag = 1;
						break;
					}
				}
			} 
			else 
				msg += "Client-" + to_string(Client[index].index + 1) + ": NO USER LOGGED IN currently to create a group...\n";
			send(Client[index].sockID, msg.c_str(), 2048, 0);
		}
		if(cmd[0] == "fetch_groups") {
			string msg = "";
			int flag = 0;
			for(int i = 0; i < usr.size(); i++) {
				if(Client[index].username == usr[i]->username) {
					map<string, string>::iterator itr;
					msg += "All Groups in Server: \nGROUP\tOWNER\n";
					//cout << "All Groups in Server: " << endl;
					//cout << "GROUP\tOWNER" << endl;
					for(itr = group_map.begin(); itr != group_map.end(); itr++) {
						msg += itr->first + "\t" + itr->second + "\n";
						//cout << itr->first << "\t" << itr->second << endl;
					}
					flag = 1;
					break;
				}
			}
			if(flag == 0)
				msg += "LOGIN first to see all the groups...\n";
			send(Client[index].sockID, msg.c_str(), 2048, 0);
		}
		if(cmd[0] == "yes") {
			cout << "Server Side YES::::\n";
		}
		if(cmd[0] == "no") {
			cout << "Server Side NO::::\n";
		}
		if(cmd[0] == "request_group_join") {
			string msg = "";
			int flag = 0, dem = 0;
			for(int i = 0; i < usr.size(); i++) {
				if(Client[index].username == usr[i]->username) {
					map<string, string>::iterator itr;
					for(itr = group_map.begin(); itr != group_map.end(); itr++) {
						if(itr->first == cmd[1]) {
							string owner = itr->second;
							int flag2 = 0, owner_sockid;
							for(int j = 0; j < 1024; j++) {
								if(Client[j].username == owner) {
									owner_sockid = Client[j].sockID;
									flag2 = 1;
									break;
								}
							}
							if(flag2 == 1) {
								char outp[1024];
								msg += "accept_group_join " + Client[index].username + " " + cmd[1] + " " + to_string(clientSocket) + "\n";
								send(owner_sockid, msg.c_str(), 2048, 0);
								int rcv = recv(owner_sockid, outp, 1024, 0);
								outp[rcv] = '\0';
								string t(outp);
								cout << "Faltu outp: " << t << endl;
								if(strcmp(outp, "yes") == 0) {
									for(int k = 0; k < groups.size(); k++) {
										if(groups[k]->name == cmd[1] && groups[k]->owner == owner) {
											groups[k]->members.push_back(Client[index].username);
											usr[i]->group_part.push_back(cmd[1]);
											msg += "Group Owner: <" + owner + "> ACCEPTED your Request !!\n";
											break;
										}
									}
								}
								else if(strcmp(outp, "no") == 0) {
									msg += "Group Owner: <" + owner + "> REJECTED your Request. Please try again later...\n";
								}
								else
									msg += "Faltu Option !!\n";
								/*char data[1024], output[1024];
								int read = recv(clientSocket,data,1024,0);
								data[read] = '\0';	
								string tmp = "";
								vector<string> cmd;
								for(int i = 0; data[i] != '\0'; i++) {
									if(data[i] == ' ') {
										if(tmp != "")
											cmd.push_back(tmp);
										tmp = "";
										continue;
									}
									tmp += data[i];
								}
								if(tmp != "")
									cmd.push_back(tmp);*/

							}
							else {
								msg += "Group Owner: <" + owner + "> is currently OFFLINE. Please try again later...\n";
								//send(Client[index].sockID, msg.c_str(), 2048, 0);
							}
							flag = 1;
							break;
						}
					}
					if(flag == 0) {
						msg += "Group NOT Present...\n";
					}
					dem = 1;
					break;
				}
			}
			if(dem == 0)
				msg += "LOGIN first to enter a group...\n";
			//msg += "last\n";
			send(Client[index].sockID, msg.c_str(), 2048, 0);
		}
		/*if(cmd[0] == "leave_group") {
			
		}
		if(cmd[0] == "accept_group_join") {
			
		}*/
		if(strcmp(data,"LIST") == 0) {

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