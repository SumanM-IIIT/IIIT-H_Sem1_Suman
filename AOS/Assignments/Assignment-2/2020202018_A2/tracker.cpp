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
#include <fstream>
using namespace std;

int clientCount = 0, tmpClientCount = 0;

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
typedef struct file_info {
	string filename;
	string sha1;
	vector<string> owners;
} FileInfo;
typedef struct group {
	string owner, name;
	vector<string> members;
	vector<FileInfo> group_files;
} Group;

typedef struct user {
	string username = "", password = "";
	bool is_login = true;
	vector<string> group_part, own_group;
	vector<FileInfo*> usr_file_arr;
} User;

struct client Client[1024];
vector<Group*> groups;
vector<FileInfo*> files_arr;
pthread_t my_thread[1024];
vector<User*> usr;
map<string, string> group_map;
map<string, FileInfo> user_file_map;

void * doNetworking(void * ClientDetail) {

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
			sleep(1);
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
			sleep(1);
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
			sleep(1);
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
			sleep(1);
			send(Client[index].sockID, msg.c_str(), 2048, 0);
		}
		if(cmd[0] == "fetch_groups") {
			string msg = "";
			int flag = 0;
			for(int i = 0; i < usr.size(); i++) {
				if(Client[index].username == usr[i]->username) {
					map<string, string>::iterator itr;
					msg += "All Groups in Server: \nGROUP\tOWNER\tMEMBERS\n";
					//cout << "All Groups in Server: " << endl;
					//cout << "GROUP\tOWNER" << endl;
					for(itr = group_map.begin(); itr != group_map.end(); itr++) {
						msg += itr->first + "\t" + itr->second + "\t";
						for(int j = 0; j < usr.size(); j++) {
							int len = usr[j]->group_part.size();
							for(int k = 0; k < len; k++) {
								if(usr[j]->group_part[k] == itr->first) {
									msg += usr[j]->username + ",";
								}
							}
						}
						msg += "\n";
						//cout << itr->first << "\t" << itr->second << endl;
					}
					flag = 1;
					break;
				}
			}
			if(flag == 0)
				msg += "LOGIN first to see all the groups...\n";
			sleep(1);
			send(Client[index].sockID, msg.c_str(), 2048, 0);
		}
		if(cmd[0] == "upload_file") {
			string msg = "";
			int flag = 0;
			for(int i = 0; i < usr.size(); i++) {
				if(Client[index].username == usr[i]->username) {
					ifstream f_in;
					f_in.open(cmd[1]);
					if (!f_in) { 
						msg += "CAN'T OPEN THE FILE: " + cmd[1] + " !!\n"; 
					}
					else {
						int flag_fu = 0;
						for(int k = 0; k < files_arr.size(); k++) {
							if(files_arr[k]->filename == cmd[1]) {
								files_arr[k]->owners.push_back(usr[i]->username);
								flag_fu = 1;
								break;
							}
						}
						if(flag_fu == 0) {
							FileInfo *fi;
							fi = (FileInfo *)malloc(sizeof(FileInfo));
							fi->filename = cmd[1];
							fi->sha1 = "hello";
							fi->owners.push_back(usr[i]->username);
							usr[i]->usr_file_arr.push_back(fi);
							files_arr.push_back(fi);
							//cout << "File: " << fi.filename << endl;
						}
						msg += "File Uploaded successfully..\n";
					}
					flag = 1;
					f_in.close();
					sleep(1);
					break;
				}
			}
			if(flag == 0)
				msg += "LOGIN first to upload files...\n";
			send(Client[index].sockID, msg.c_str(), 2048, 0);
		}
		if(cmd[0] == "download_file") {
			string msg = "";
			int flag = 0;
			for(int i = 0; i < usr.size(); i++) {
				if(Client[index].username == usr[i]->username) {
					ofstream f_out;
					ifstream f_in;
					f_in.open(cmd[1]);
					f_out.open("downloaded_" + cmd[1]);
					if (!f_in) { 
						msg += "CAN'T DOWNLOAD THE FILE: " + cmd[1] + " !!\n"; 
					}
					else {
						char f_data;
						while(f_in >> f_data) {
							//cout << "Hello" << f_data << " ";
							f_out << f_data;
							//msg += f_data;
						}	
						msg += "File Downloaded successfully...\n";
					}
					flag = 1;
					f_in.close();
					f_out.close();
					break;
				}
			}
			if(flag == 0)
				msg += "LOGIN first to upload files...\n";
			send(Client[index].sockID, msg.c_str(), 2048, 0);
		}
		if(cmd[0] == "show_files") {
			string msg = "";
			//int flag = 0;
			//for(int i = 0; i < usr.size(); i++) {
			//	if(Client[index].username == usr[i]->username) {
					for(int j = 0; j < files_arr.size(); j++) {
						msg += to_string(j + 1) + ". " + files_arr[j]->filename + "\tOwners: ";
						for(int k = 0; k < files_arr[j]->owners.size(); k++) {
							msg += " " + files_arr[j]->owners[k];
						}
						msg += "\n";
					}
			//		flag = 1;
			//		break;
			//	}
			//}
			//if(flag == 0)
			//	msg += "LOGIN first to view files...\n";
			send(Client[index].sockID, msg.c_str(), 2048, 0);
		}
		if(cmd[0] == "exit" || cmd[0] == "quit") {
			string msg = Client[index].username;
			int val = 100;
			send(Client[index].sockID, msg.c_str(), 2048, 0);
			cout << "Client-" << index + 1 << ": <" << Client[index].username << "> exiting...\n";
			pthread_exit(&val);
			sleep(1);
			return NULL;
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
								//cout << "Faltu outp: " << t << endl;
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
		//cout << "Sleeping for 1 sec..\nFiles_arrSize: " << files_arr.size() << endl;
		sleep(1);
		/*for(int i = 0; i < files_arr.size(); i++) {
			cout << files_arr[i]->filename << endl;
		}*/

	}
	return NULL;
}

int main(int argc, char** argv) {
	fstream f_in;
	f_in.open(argv[1]);
	string tr;
	vector<string> tr_arr;
	while(f_in >> tr) {	
		tr_arr.push_back(tr);
	}
	string ip;
	int port, tracker_no = stoi(argv[2]);
	int serverSocket = socket(PF_INET, SOCK_STREAM, 0);
	struct sockaddr_in serverAddr;

	if(tracker_no == 1) {
		port = stoi(tr_arr[1]);
		ip =tr_arr[0];
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(port);
		serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
		int flag = 0;
		if(bind(serverSocket,(struct sockaddr *) &serverAddr , sizeof(serverAddr)) == -1) 
			flag = 1;
		if(listen(serverSocket,1024) == -1) 
			flag = 1;
		if(flag == 1) {
			cout << "Tracker-1 Connection failed !! Tracker-2 connecting..." << endl;
			port = stoi(tr_arr[3]);
			ip = tr_arr[2].c_str();
			serverAddr.sin_port = htons(port);
			serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
			if(bind(serverSocket,(struct sockaddr *) &serverAddr , sizeof(serverAddr)) == -1) 
				flag = 2;
			if(listen(serverSocket,1024) == -1) 
				flag = 2;
			if(flag == 2) {
				cout << "Tracker-2 Connection also failed !! Exiting.." << endl;
				return 0;
			}
		}
		cout << "Tracker is listenting on port " << port << "..." << endl;
	}
	else if(tracker_no == 2) {
		port = stoi(tr_arr[3]);
		ip =tr_arr[2]; 
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(port);
		serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
		int flag = 0;
		if(bind(serverSocket,(struct sockaddr *) &serverAddr , sizeof(serverAddr)) == -1) 
			flag = 1;
		if(listen(serverSocket,1024) == -1) 
			flag = 1;
		if(flag == 1) {
			cout << "Tracker-2 Connection failed !! Tracker-1 connecting..." << endl;
			port = stoi(tr_arr[1]);
			ip = tr_arr[0].c_str();
			serverAddr.sin_port = htons(port);
			serverAddr.sin_addr.s_addr = inet_addr(ip.c_str());
			if(bind(serverSocket,(struct sockaddr *) &serverAddr , sizeof(serverAddr)) == -1) 
				flag = 2;
			if(listen(serverSocket,1024) == -1) 
				flag = 2;
			if(flag == 2) {
				cout << "Tracker-1 Connection also failed !! Exiting.." << endl;
				return 0;
			}
		}
		cout << "Tracker is listenting on port " << port << "..." << endl;
	}
	else {
		cout << "Please enter correct tracker number !! Exiting..." << endl;
		return 0;
	}

	while(1) {
		Client[clientCount].sockID = accept(serverSocket, (struct sockaddr*) &Client[clientCount].clientAddr, &Client[clientCount].len);
		Client[clientCount].index = clientCount;

		pthread_create(&my_thread[clientCount], NULL, doNetworking, (void *) &Client[clientCount]);
		clientCount++;
		tmpClientCount++;
	}

	for(int i = 0; i < clientCount; i++)
		pthread_join(my_thread[i],NULL);
	return 0;
}