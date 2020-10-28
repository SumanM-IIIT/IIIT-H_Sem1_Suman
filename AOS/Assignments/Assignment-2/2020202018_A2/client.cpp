#include <bits/stdc++.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <unistd.h> 
#include <string.h> 
using namespace std;

#define PORT 8080 
   
int main(int argc, char** argv) { 
    int sock = 0, valread; 
    struct sockaddr_in serv_addr; 
    string client_msg = "Hello !! from client"; 
    
    char buffer[1024] = {0}; 
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    { 
        printf("\n Socket creation error \n"); 
        return -1; 
    } 
   
    serv_addr.sin_family = AF_INET; 
    serv_addr.sin_port = htons(PORT); 
       
    // Convert IPv4 and IPv6 addresses from text to binary form 
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)  
    { 
        printf("\nInvalid address/ Address not supported \n"); 
        return -1; 
    } 
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    { 
        printf("\nConnection Failed \n"); 
        return -1; 
    } 
    string client_no(argv[1]);
    while(client_msg != "quit") {
        cout << "Enter Message: ";
        cin >> client_msg;
        string tmp = "Client-" + client_no + ": " + client_msg;
        send(sock , tmp.c_str(), tmp.length(), 0 ); 
        cout << "Message sent to server: " << client_msg << endl; 
        //valread = read(sock, buffer, 1024); 
        //cout << "Message from Server: " << buffer << endl; 
    }
    return 0; 
} 