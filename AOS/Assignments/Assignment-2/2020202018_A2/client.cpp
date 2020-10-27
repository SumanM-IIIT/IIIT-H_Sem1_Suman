#include<iostream>
#include<sstream>
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;
ll piece_size = 512*1024

string client_ip, server_ip1, server_ip2, log;

typedef struct meta_download {
	string ip, mtorrent, dest, filename;
	int port, socket;
}download_info;


int str_to_int(string port) {
	int port_int = 0;
	stringstream tmp(port);
	tmp >> port_int;
	return port_int;
}
void print_log(string msg) {
    ofstream out_f;
    out_f.open(log, ios_base::app | ios_base::out);
    out_f << msg << endl;
    out_f.close();
}
vector<string> split(string addr, char dlmt) {
    vector<string> res;
    int i = 0;
    string tmp_s = "";
    while(1) {
        if(addr[i] == '\\') {
            tmp_s += addr[i + 1];
            i++;
        }
        else if(addr[i] == dlmt) {
            res.push_back(tmp_s);
            tmp_s = "";
        }
        else if(addr[i] == '\0') {
    		res.push_back(tmp_s);
    		break;
    	}
        else
            tmp_s += addr[i];
        i++;
    }
    return res;
}


int main(int argc, char** argv) {
	if(argc > 5) {
        cout << "Arguments are MORE than required !!" << endl;
        return 0;
    }
    else if(argc < 5) {
        cout << "Arguments are LESS than required !!" << endl;
        return 0;
    }

    string IP_c, IP_S1, IP_S2;
    int PORT_C, PORT_S1, PORT_S2;
    vector<string> temp;

    client_ip = string(argv[1]);
    server_ip1 = string(argv[2]);
    server_ip2 = string(argv[3]);
    log = string(argv[4]);

    ofstream out_f;
	out_f.open(log, fstream::out);
	out_f.close();
	print_log("Client Log-File created..");

	temp = split(client_ip, ':');
	IP_C = temp[0];
	PORT_C = str_to_int(temp[1]);
	temp = split(server_ip1, ':');
	IP_S1 = temp[0];
	PORT_S1 = str_to_int(temp[1]);
	temp = split(server_ip2, ':');
	IP_S2 = temp[0];
	PORT_S2 = str_to_int(temp[1]);

    return 0;
}