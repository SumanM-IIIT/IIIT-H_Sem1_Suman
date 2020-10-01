#include "my_header.h"
#include <bits/stdc++.h>

#define ENTER 10
#define BACKSPACE 127

using namespace std;
int main() {

	char ch;
	char dir_path[2048];
	size_t length = 2048;
	getcwd(dir_path, length);
	root_path(dir_path);
	curr_dir_path(dir_path);
	init_keys();

	while(1) {
		if(all_ok()) {	
			ch = read_char();
			//cout << ch << endl;
			switch(ch) {
				case 'A': go_up();
						break;
				case 'B': go_down();
						break;
				case 'C': go_forward();
						break;
				case 'D': go_back();
						break;
				case BACKSPACE: go_up_level();
						break;
				case 'h': go_home();
						break;
				case ENTER: open_file_dir();
						break;
				case ':': command_mode();
						break;
				case 'q': clear(); off_keys();
						exit(1);
				default:
					break; 
			}
		}
	}
	//cout << "\033[H\033[1";
	return 0;
}