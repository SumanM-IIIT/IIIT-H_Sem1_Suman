#include <unistd.h>

#include <stdio.h>

#include <sys/socket.h>

#include <stdlib.h>

#include <netinet/in.h>

#include <string.h>

#include<pthread.h>

#include<bits/stdc++.h>

#define MAX_THREADS 10

using namespace std;

map < string, int > sockmap;
map < int, string > sockrevmap;

int cnt = 0, thread_id[MAX_THREADS + 10];

pthread_t clients[MAX_THREADS];
pthread_cond_t read_cond = PTHREAD_COND_INITIALIZER;

pair < int, int > thread_data[MAX_THREADS];
struct sockaddr_in address;
int server_fd, new_sockets[MAX_THREADS];
int addrlen = sizeof(address);

int opt = 1;
bool ongoing[MAX_THREADS + 10];

bool exited() {
  bool is_return = true;
  for (int i = 0; i < MAX_THREADS; i++) is_return = is_return & (!ongoing[i]);
  return !is_return;
}

int avialable_threads() {
  int ptr = 0;
  while (ongoing[ptr++]);

  if (ptr == MAX_THREADS + 1) return -1;
  else return ptr - 1;
}

void * handler(void * arg) {
    int valread;
    char buffer[524288];

    pair < int, int > *new_socket_data = (pair < int, int > * ) arg;
    int new_socket = ( *new_socket_data).second;
    int ID = ( *new_socket_data).first;

    map < string, string > credentials;
    string username, password, file_path, user_id;

    ifstream infile("login.txt");

    string line;
    while (getline(infile, line)) {
      stringstream ss(line);
      string un, pass;
      ss >> un;
      ss >> pass;
      credentials[un] = pass;
    }
    infile.close();

    while (1) {
      valread = read(new_socket, buffer, 524288);
      string b(buffer);
      cout << buffer << endl;
      stringstream ss(b);
      string tok, gid;
      ss >> tok;
      if (tok == "create_user") {
          ss >> username;
          ss >> password;
          if (credentials.find(username) == credentials.end()) {
            ofstream log("login.txt", std::ios_base::app | std::ios_base::out);
            log << username << " " << password << "\n";
            credentials[username] = password;
            sockmap[username] = new_socket;
            sockrevmap[new_socket] = username;
            log.close();
            send(new_socket, "User sucessfully created\n", 30, 0);
          } else {
            send(new_socket, "User id already exits\n", 30, 0);
          }
        } else if (tok == "login") {
          ss >> username;
          ss >> password;

          if (credentials.find(username) != credentials.end()) {
            string t = (credentials.find(username))->second;
            if (t != password) {
              send(new_socket, "Invalid Password\n", 30, 0);
            } else {
              sockmap[username] = new_socket;
              sockrevmap[new_socket] = username;
              send(new_socket, "Welcome", 10, 0);
            }
          }
          else {
          	 send(new_socket, "User not exits\n", 30, 0);
          }
        } else if (tok == "logout") {
          sockmap.erase(sockrevmap[new_socket]);
          sockrevmap.erase(new_socket);
          send(new_socket, "Logout Sussessfully\n", 30, 0);
        } else if (tok == "upload_file") {
          string sh;
          ss >> file_path;
          ss >> gid;
          //
          {
          int flag=0;
          ifstream ipgfile( & gid[0]);
          while (getline(ipgfile, line)) {
            if (line == sockrevmap[new_socket]) {
            	flag = 1;
            	break;
            }
          }
          if (flag == 0) {
          	send(new_socket, "Invalid group\n", 30, 0);
          	memset(buffer, '\0', 524288);
          	continue;
          }
          ipgfile.close();       
          }
          //
          ss >> sh;
          ofstream gfile;
          gfile.open("knowfile.txt", std::ofstream::out | std::ofstream::app);
          gfile << file_path << " " << gid << " " << sh << " ";
          ifstream igfile("tracker.txt");
          string stemp;
          while (getline(igfile, line)) {
            stringstream st(line);
            st >> stemp;
            if (stemp == sockrevmap[new_socket]) {
              st >> stemp;
              gfile << stemp << endl;
            }
          }
          igfile.close();
          gfile.close();

          send(new_socket, "File Sussessfully uploaded\n", 30, 0);
        } else if (tok == "download_file") {
          string filename, dest_path;
          ss >> gid;
          //
          {
          int flag=0;
          ifstream ipgfile( & gid[0]);
          while (getline(ipgfile, line)) {
            if (line == sockrevmap[new_socket]) {
            	flag = 1;
            	break;
            }
          }
          if (flag == 0) {
          	send(new_socket, "Invalid group\n", 30, 0);
          	memset(buffer, '\0', 524288);
          	continue;
          }
          ipgfile.close();        
          }
          //
          ss >> filename;
          ss >> dest_path;
          int flag = 1;
          ifstream igfile("knowfile.txt");
          string stemp, ftemp, tftemp;
          while (getline(igfile, line)) {
            stringstream st(line);
            st >> stemp;
            ftemp = "";
            int i = stemp.length() - 1;
            while (i > -1 && stemp[i] != '/') {
              ftemp += stemp[i];
              i--;
            }
            reverse(ftemp.begin(), ftemp.end());

            if (ftemp == filename) {
              tftemp = "@" + stemp;
              st >> stemp;
              if (stemp == gid) {
                st >> stemp >> stemp;
                //
                ifstream tigfile("tracker.txt");
                string tline, tname, tport;
                while (getline(tigfile, tline)) {
                  stringstream tst(tline);
                  tst >> tname >> tport;
                  if (tport == stemp) {
                    send(sockmap[tname], & tftemp[0], tftemp.length(), 0);
                    break;
                  }
                }
                tigfile.close();
                //
                stemp = "#" + stemp;

                flag = 0;
                send(new_socket, & stemp[0], stemp.length(), 0);
                break;
              }
            }
          }
          igfile.close();
          if (flag)
            send(new_socket, "File Not present\n", 30, 0);
        } else if (tok == "create_group") {
          string gid;
          ss >> gid;
          ifstream ipgfile( & gid[0]);
          if (ipgfile.good()) {
            ipgfile.close();
            send(new_socket, "Group Already Exits\n", 30, 0);
          } else {
            ipgfile.close();
            ofstream gfile;
            gfile.open("grpfile.txt", std::ofstream::out | std::ofstream::app);
            gfile << gid << endl;
            gfile.close();
            ofstream opgfile;
            opgfile.open( & gid[0], std::ofstream::out | std::ofstream::app);
            opgfile << sockrevmap[new_socket] << endl;
            opgfile.close();
            send(new_socket, "Group Sucessfully created\n", 30, 0);
          }
        } else if (tok == "list_groups") {
          ifstream igfile("grpfile.txt");
          string stemp = "";
          while (getline(igfile, line)) {
            stemp += line;
            stemp += "\n";
          }
          igfile.close();
          send(new_socket, & stemp[0], 100, 0);
        } else if (tok == "join_group") {
          string gid;
          ss >> gid;
          ifstream ipgfile( & gid[0]);
          if (ipgfile.good()) {
            ipgfile.close();
            ofstream opgfile;
            opgfile.open( & gid[0], std::ofstream::out | std::ofstream::app);
            opgfile << sockrevmap[new_socket] << endl;
            opgfile.close();
            send(new_socket, "Joined Sucessfully\n", 20, 0);
          } else {
            ipgfile.close();
            send(new_socket, "Group Not Exits\n", 20, 0);
          }
        } else if (tok == "leave_group") {
          string gid;
          int flag = 0;
          ss >> gid;
          ifstream igfile( & gid[0]);
          ofstream ogfile("ngrpfile.txt");
          while (getline(igfile, line)) {
            if (line != sockrevmap[new_socket]) {
              ogfile << line << "\n";
            } else {
              flag = 1;
            }
          }
          igfile.close();
          ogfile.close();
          if (flag == 0) {
            send(new_socket, "Not present in the group\n", 100, 0);
          } else {
            igfile.open("ngrpfile.txt");
            ogfile.open( & gid[0]);
            while (getline(igfile, line)) {
              ogfile << line << "\n";
            }
            igfile.close();
            ogfile.close();
            send(new_socket, "Sucessfully removed\n", 100, 0);
          }
        } else if (tok == "list_files") {
          string gid, stemp = "";
          ss >> gid;
          //
          {
          int flag=0;
          ifstream ipgfile( & gid[0]);
          while (getline(ipgfile, line)) {
            if (line == sockrevmap[new_socket]) {
            	flag = 1;
            	break;
            }
          }
          if (flag == 0) {
          	send(new_socket, "Invalid group\n", 30, 0);
          	memset(buffer, '\0', 524288);
          	continue;
          }
          ipgfile.close();        
          }
          //
          ifstream igfile("knowfile.txt");
          while (getline(igfile, line)) {
            string filename, gname;
            stringstream st(line);
            st >> filename >> gname;
            if (gname == gid) {
              stemp += filename + "\n";
            }
          }
          char tempBuff[524288];
          strcpy(tempBuff, stemp.c_str());
          tempBuff[stemp.length()] = '\n';
          tempBuff[stemp.length() + 1] = '\0';
          send(new_socket, tempBuff, stemp.length() + 2, 0);
          igfile.close();
        } else {
          send(new_socket, "Invalid Command\n", 100, 0);
        }
        memset(buffer, '\0', 524288);

      }

      ongoing[ID] = false;
      pthread_cond_signal( & read_cond);
      pthread_exit(0);
  }

int main(int argc, char * argv[]) {
  ofstream fogfile("tracker.txt");
  fogfile.close();
  ofstream ogfile("knowfile.txt");
  ogfile.close();
  int PORT = atoi(argv[1]);

  for (int i = 0; i < MAX_THREADS; i++) {
    ongoing[i] = false;
    thread_id[i] = i;
  }

  do {
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
      perror("socket failed");
      exit(EXIT_FAILURE);
    }
    if (setsockopt(server_fd,
        SOL_SOCKET,
        SO_REUSEADDR | SO_REUSEPORT, &
        opt, sizeof(opt))) {
      perror("setsockopt");
      exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd,
        (struct sockaddr * ) & address,
        sizeof(address)) < 0) {
      perror("bind failed");
      exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
      perror("listen");
      exit(EXIT_FAILURE);
    }
    pthread_mutex_t *mutex = NULL;
    while (avialable_threads() == -1)
      pthread_cond_wait( & read_cond, mutex);

    int e = avialable_threads();

    ongoing[e] = true;

    if ((new_sockets[e] = accept(server_fd,
        (struct sockaddr * ) & address,
        (socklen_t * ) & addrlen)) < 0) {
      perror("accept");
      exit(EXIT_FAILURE);
    }

    thread_data[e] = {
      thread_id[e],
      new_sockets[e]
    };
    pthread_create( & clients[e], NULL, handler, & thread_data[e]);
    pthread_detach(clients[e]);

  } while (exited());

  return 0;
}
