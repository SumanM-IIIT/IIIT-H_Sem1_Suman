/*#include <sys/ioctl.h>
#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;
int main() {
  string str = "";
  char ch;
  while ((ch = std::cin.get()) != '\033[') {
    str += ch;
  }

 cout << str;
return 0;
}*/
//#include <stdio.h>
//#include <stdlib.h>
#include <bits/stdc++.h>
using namespace std;
/*int main()
{
    char exit;
    while(1) {
        printf("PLEASE INSERT OPTION:");
        exit = getchar();
        if(exit == '\033')
        	cin.ignore();
    }//while(exit != '\033');
    return 0;
}//main*/
#define BACKSLASH 92

void init_cmd_arr(string);

vector<string> cmd_str_arr;

int main() {
	string str = "/home/bug_sm87/IIIT-H_Sem1_Suman/AOS/Assignments/Assignment-1/2020202018/demo2.txt";
	//getline(cin, str, '\n');
	//cout << "Hello";
	//init_cmd_arr(str);

	/*int i = 0;
	while(i < str.size()) {
		tmp = "";
		while(str[i] != ' ' && i < str.size()) {
			tmp += str[i];
			i++;
		}
		i++;
		cout << tmp << endl;
	}*/

	/*for(int i = 0; i < cmd_str_arr.size(); i++) {
		cout << cmd_str_arr[i];
	}*/

	int c = remove(str.c_str());
	cout << c;
	return 0;
}
void init_cmd_arr(string cmd) {
	cmd_str_arr.clear();
	int i = 0, len = cmd.size();
	string tmp;
	//cout << "cmd: " << cmd; 
	while(i < len) {
		tmp = "";
		//j = i;
		while(cmd[i] != ' ' && i < len) {
			if(cmd[i] == BACKSLASH) {
				tmp += " ";
				i += 2;
			}
			else {
				tmp += cmd[i];
				i++;
			}
		}
		cout << tmp << endl;
		cmd_str_arr.push_back(tmp);
		i++;
	}
}
