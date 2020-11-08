#include <sys/socket.h>

#include <sys/types.h>

#include <netinet/in.h>

#include <netdb.h>

#include <stdio.h>

#include <string.h>

#include <stdlib.h>

#include <unistd.h>

#include <errno.h>

#include <arpa/inet.h>

#include <pthread.h>

#include <bits/stdc++.h>

#include <time.h>

#include <fcntl.h>

#include <openssl/sha.h>

using namespace std;

int POT, dest_POT;
int server_fd, new_socket, value_read;

char buffer[524288];
char fileBuff[524288];

pthread_t thread4;
int listenfd = 0, connfd = 0;
struct sockaddr_in sserv_addr, cserv_addr;
char readBuff[524288];

int sockfd = 0, n = 0, csockfd = 0;
char recvBuff[524288];
int temp;
struct sockaddr_in serv_addr;
char sendBuff[524288];
pthread_t thread1, thread2, thread3, thread5;
string dfilename, dfiledest;
string upfile;
void * download(void * arg) {
  string df = dfiledest + "/" + dfilename;
  int out = open( & df[0], O_CREAT | O_RDWR, 0666);
  if (out == -1) {

    perror("Unable to open file");
    return NULL;
  }
  for (;;) {
    int bytesRead = read(csockfd, fileBuff, sizeof(fileBuff));
    if (bytesRead <= 0) {
      break;
    }
    int bytesWrite = write(out, fileBuff, bytesRead);
    if (bytesWrite <= 0) {
      break;
    }
  }
  close(out);

  int in = open( & df[0], O_RDONLY);
  if ( in < 0) {
    perror("Unable to open file");
    return NULL;
  }

  SHA_CTX ctx;
  SHA1_Init( & ctx);

  for (;;) {
    ssize_t bytesRead = read( in , buffer, sizeof(buffer));
    if (bytesRead <= 0) {
      break;
    }
    SHA1_Update( & ctx, buffer, bytesRead);
  }
  close( in );

  unsigned char hash[SHA_DIGEST_LENGTH];
  SHA1_Final(hash, & ctx);

  string sh(reinterpret_cast < char * > (hash));
  stringstream st(sh);
  st >> sh;

  string tsh, line;
  ifstream igfile("knowfile.txt");
  string stemp, ftemp;

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
    if (ftemp == dfilename) {
      st >> tsh >> tsh;
      break;
    }
  }

  if (sh == tsh)
    cout << "File Sucessfully downloaded\n";
  else
    cout << "Error in downloading\n";
  close(out);

}
void * proc_command(void * arg) {
  while (1) {
    read(sockfd, recvBuff, 524288);
    string st(recvBuff);
    memset(recvBuff, '\0', sizeof(recvBuff));
    if (st[0] == '@') {
      st = st.substr(1, st.length() - 1);
      upfile = st;
    }
    else if (st[0] == '#') {
      st = st.substr(1, 4);
      stringstream ss(st);
      ss >> dest_POT;

      memset(recvBuff, '0', sizeof(recvBuff));
      if ((csockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Error : Could not create socket \n");
        return NULL;
      }

      memset( & sserv_addr, '0', sizeof(sserv_addr));

      sserv_addr.sin_family = AF_INET;
      sserv_addr.sin_port = htons(dest_POT);

      if (inet_pton(AF_INET, "127.0.0.1", & sserv_addr.sin_addr) <= 0) {
        printf("\n inet_pton error occured\n");
        return NULL;
      }

      if (connect(csockfd, (struct sockaddr * ) & sserv_addr, sizeof(sserv_addr)) < 0) {
        printf("\n Error : Connect Failed \n");
        return NULL;
      }
      pthread_attr_t type5;
      pthread_attr_init( & type5);
      int * p;
      pthread_create( & thread5, & type5, download, (void * ) p);

    }
    else {
    	cout<<st<<endl;
    }

  }
}
void * get_command(void * arg) {

  int flag = 1;
  while (1) {
    string sb;
    getline(cin, sb);

    stringstream ss(sb);
    string tok;
    ss >> tok;
    if (tok != "create_user" && tok != "login" && flag) {
      cout << "Login or Create New User and then Login\n";
      continue;
    } else {
      if (tok == "login") {
      	strcpy(sendBuff, sb.c_str());
    	write(sockfd, sendBuff, sb.length());
    	read(sockfd, recvBuff, 524288);
    	string st(recvBuff);
    	memset(recvBuff, '\0', sizeof(recvBuff));
    	if(st=="Welcome") {
        ss >> tok;
        ofstream trac("tracker.txt", std::ios_base::app | std::ios_base::out);
        trac << tok << " " << POT << "\n";
        trac.close();
        flag = 0;
    	}
    	cout<<st<<endl<<endl;
    	continue;
      }
      if (tok == "download_file") {
        ss >> tok >> dfilename >> dfiledest;
      }
      if (tok == "upload_file") {
        ss >> tok;
        string ftemp = "", stemp = tok;
        int i = stemp.length() - 1;
        while (i > -1 && stemp[i] != '/') {
          ftemp += stemp[i];
          i--;
        }
        reverse(ftemp.begin(), ftemp.end());
        ftemp = "HASH" + ftemp;
        int in = open( & tok[0], O_RDONLY);
        if ( in < 0) {
          perror("Unable to open file");
          continue;
        }

        SHA_CTX ctx;
        SHA1_Init( & ctx);

        for (;;) {
          ssize_t bytesRead = read( in , buffer, sizeof(buffer));
          if (bytesRead <= 0) {
            break;
          }
          SHA1_Update( & ctx, buffer, bytesRead);
        }
        unsigned char hash[SHA_DIGEST_LENGTH];
        SHA1_Final(hash, & ctx);

        string sh(reinterpret_cast < char * > (hash));
        sb += " " + sh;
        close( in );

      }
     if (tok == "logout") {
     	flag = 1;
     }
    }
    strcpy(sendBuff, sb.c_str());
    write(sockfd, sendBuff, sb.length());
  }
}
void * clth(void * para) {
  int * port = (int * )(para);
  int portno = port[0];
  pthread_attr_t type1, type2;
  pthread_attr_init( & type1);
  pthread_attr_init( & type2);
  int * p;

  memset(recvBuff, '0', sizeof(recvBuff));
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Error : Could not create socket \n");
    return NULL;
  }

  memset( & serv_addr, '0', sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;

  serv_addr.sin_port = htons(portno);

  if (inet_pton(AF_INET, "127.0.0.1", & serv_addr.sin_addr) <= 0) {
    printf("\n inet_pton error occured\n");
    return NULL;
  }

  if (connect(sockfd, (struct sockaddr * ) & serv_addr, sizeof(serv_addr)) < 0) {
    printf("\n Error : Connect Failed \n");
    return NULL;
  }
  pthread_create( & thread1, & type1, proc_command, (void * ) p);
  pthread_create( & thread2, & type2, get_command, (void * ) p);
  pthread_join(thread2, NULL);
  pthread_join(thread1, NULL);
}
void * upload(void * arg) {
  int in = open( & upfile[0], O_RDONLY);
  if ( in < 0) {
    perror("Unable to open data");
    exit(1);
  }
  for (;;) {
    ssize_t bytesRead = read( in , buffer, sizeof(buffer));
    if (bytesRead <= 0) {
      break;
    }
    write(connfd, buffer, bytesRead);

  }
  close(connfd);
}
int main(int argc, char * argv[]) {
  srand(time(0));
  POT = 4000 + rand() % 4000;

  pthread_attr_t cleintth;
  pthread_attr_init( & cleintth);
  int portno = atoi(argv[1]);
  int * para = (int * ) malloc(sizeof(int));
  para[0] = portno;
  pthread_create( & thread3, & cleintth, clth, (void * ) para);

  pthread_attr_t type1;
  pthread_attr_init( & type1);
  int * p;
  listenfd = socket(AF_INET, SOCK_STREAM, 0);
  memset( & cserv_addr, '0', sizeof(cserv_addr));
  memset(sendBuff, '0', sizeof(sendBuff));

  cserv_addr.sin_family = AF_INET;
  cserv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  cserv_addr.sin_port = htons(POT);

  bind(listenfd, (struct sockaddr * ) & cserv_addr, sizeof(cserv_addr));

  listen(listenfd, 1000);
  while (1) {
    connfd = accept(listenfd, (struct sockaddr * ) NULL, NULL);
    pthread_create( & thread4, & type1, upload, (void * ) p);
  }
  pthread_join(thread4, NULL);
  close(connfd);

  return 0;
}