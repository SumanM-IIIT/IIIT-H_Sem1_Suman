#include <iostream>
#include <string.h>
#include <bits/stdc++.h>
using namespace std;

int main() {
	string s, tmp;
	getline(cin, s);

	vector<string> cmd;
	for(int i = 0; i < s.length(); i++) {
		if(s[i] == ' ') {
			if(tmp != "")
				cmd.push_back(tmp);
			tmp = "";
			continue;
		}
		tmp += s[i];
	}	
	if(tmp != "")
		cmd.push_back(tmp);
	for(int i = 0; i < cmd.size(); i++) {
		cout << cmd[i] << endl;
	}
	return 0;
}