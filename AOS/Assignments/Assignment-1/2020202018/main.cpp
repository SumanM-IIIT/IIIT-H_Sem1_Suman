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
		if(keyboard_press()) {	
			ch = read_char();
			//cout << ch << endl;
			if(ch == 'A' || ch == 'k')
				go_up();
			else if(ch == 'B' || ch == 'l')
				go_down();
			else if(ch == 'D')
				go_back();
			else if(ch == 'C')
				go_forward();
			else if(ch == BACKSPACE)
				go_up_level();
			else if(ch == 'h')
				go_home();
			else if(ch == ENTER)
				open_file_dir();
			else if(ch == ':')
				command_mode();
			else if(ch == 'q') {
				clear();
				off_keys();
				exit(1);
			}
			/*switch(ch) {
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
			}*/
		}
	}
	//cout << "\033[H\033[1";
	return 0;
}