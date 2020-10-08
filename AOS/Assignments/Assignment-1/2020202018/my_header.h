#include <iostream>
#include <bits/stdc++.h>
#include <termios.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <dirent.h>
#include <time.h>
#include <iomanip>
#include <sys/wait.h>
using namespace std;

#define BACKSLASH 92
#define ESC 27
int maxi_rows;
static int search_count;


void cursor_movement(int, int);
void clear();
void go_up();
void go_down();
void go_home();
void go_up_level();
void go_back();
void go_forward();
int read_char();
void print_all();
void root_path(char const*);
void curr_dir_path(char const*);
void init_keys();
void off_keys();
int keyboard_press();
void command_mode();
void clear_cmd_prompt();
void clear_input_line();
string file_perm(struct stat);
void init_cmd_arr(string);
void copy(string, string);
//void move(string, string);
void delete_dir(string);
void search(string, string);


static struct termios initial_t, new_t;
static int top_c = -1;
int cursor = 1, first = 0, last;
int start_line, op_line, inp_line, status_line;
char const* root;
struct winsize win;
char curr_dir[2048];
size_t dir_size = 2048;
vector<dirent*> dir_file_arr;
vector<string> cmd_str_arr;
stack<string> go_back_stack, go_forward_stack, search_stack;



void clear() {
	cout << "\033[H\033[J";
}

string file_perm(struct stat perm) {
	string res;
	(S_ISDIR(perm.st_mode)) ? res += "d" : res += "-";
    (perm.st_mode & S_IRUSR) ? res += "r" : res += "-";
    (perm.st_mode & S_IWUSR) ? res += "w" : res += "-";
    (perm.st_mode & S_IXUSR) ? res += "x" : res += "-";
    (perm.st_mode & S_IRGRP) ? res += "r" : res += "-";
    (perm.st_mode & S_IWGRP) ? res += "w" : res += "-";
    (perm.st_mode & S_IXGRP) ? res += "x" : res += "-";
    (perm.st_mode & S_IROTH) ? res += "r" : res += "-";
    (perm.st_mode & S_IWOTH) ? res += "w" : res += "-";
    (perm.st_mode & S_IXOTH) ? res += "x" : res += "-";
	return res;
}

void print_all() {
	clear();
	struct stat meta;
	int i;
	for(i = first; i < last && i < dir_file_arr.size(); i++) {
		lstat(dir_file_arr[i]->d_name, &meta);
		char date[50];
		strftime(date, 20, "%d-%m-%y %H:%M:%S", localtime(&(meta.st_mtime)));
		if(S_ISDIR(meta.st_mode)) 
			cout << "\033[1;31m" << left << setw(15) << file_perm(meta) << setw(15) << to_string(meta.st_size/1024) + "KB" << setw(20) << date << dir_file_arr[i]->d_name << "\033[0m" << endl;
		else
			cout << "\033[1;34m" << left << setw(15) << file_perm(meta) << setw(15) << to_string(meta.st_size/1024) + "KB" << setw(20) << date << dir_file_arr[i]->d_name << "\033[0m" << endl;
	}
	cursor_movement(start_line, 0);
	for(int i = 0; i < win.ws_col; i++) {
		cout << "-";
	}
	cursor_movement(first, 0);
}

void open_file_dir() {
	struct stat temp;
	char *dir_file = dir_file_arr[cursor + first - 1]->d_name; 
	lstat(dir_file, &temp);

	if(S_ISDIR(temp.st_mode)) {
		if(dir_file == ".")
			return;
		if(dir_file == ".." && curr_dir == root)
			return;
		go_back_stack.push(string(curr_dir));
		curr_dir_path((string(curr_dir) + '/' + string(dir_file)).c_str());
	}
	else {
		pid_t pid = fork();
		//int status;
		//pid_t result = waitpid(pid, &status, WNOHANG);
		if(pid == 0) {
			//off_keys();
			execl("/usr/bin/xdg-open", "xdg-open", dir_file, NULL);
			//init_keys();
			//exit(1);
		}
		//if(result != 0 || result != -1)
		//	init_keys();
	}
}

void root_path(char const* path) {
	root = path;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &win);
	maxi_rows = win.ws_row - 10;
	last = first + maxi_rows;
	start_line = maxi_rows + 1;
	inp_line = start_line + 2;
	op_line = inp_line + 1;
	status_line = win.ws_row - 1;
	//inp_line = maxi_rows - 1;
}

void curr_dir_path(char const* dir_path) {
	DIR *dir;
	struct dirent *entry;
	dir = opendir(dir_path);
	if(dir) {
		chdir(dir_path);
		getcwd(curr_dir, dir_size);
		dir_file_arr.clear();

		entry = readdir(dir);
		while(entry) {
			dir_file_arr.push_back(entry);
			entry = readdir(dir);
		}
		closedir(dir);
		first = 0;
		cursor = 1;//min(int(dir_file_arr.size()), maxi_rows);
		last = min(int(dir_file_arr.size()), maxi_rows);
		print_all();
		cursor_movement(cursor, 0);
	}
	else {
		//cout << "No such directory: " << dir_path;
		return;
	}
}

void init_keys() {
	tcgetattr(0, &initial_t);
	new_t = initial_t;
	new_t.c_lflag &= ~ICANON;
	new_t.c_lflag &= ~ECHO;
	new_t.c_cc[VMIN] = 1;
	new_t.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &new_t);
}

void off_keys() {
	tcsetattr(0, TCSANOW, &initial_t);
}

int read_char() {
	char c;
	/*if(top_c != -1) {
		c = top_c;
		top_c--;
		return c;
	}*/
	read(0, &c, 1);
	return c;
}

void cursor_movement(int x, int y) {
	cout << "\033[" << x << ";" << y << "H";
	fflush(stdout);
}

void go_up() {
	if(cursor > 1) {
		cursor--;
		cursor_movement(cursor, 0);
		return;
	}
	if(first == 0)
		return;
	first--;
	last--;
	print_all();
	cursor_movement(cursor, 0);
}

void go_down() {
	if(cursor < dir_file_arr.size() && cursor < maxi_rows) {
		cursor++;
		cursor_movement(cursor, 0);
		return;
	}
	if(last == dir_file_arr.size())
		return;
	first++;
	last++;
	print_all();
	cursor_movement(cursor, 0);
}

int keyboard_press() {
	if(top_c != -1)
		return 1;
	new_t.c_cc[VMIN] = 0;
	tcsetattr(0, TCSANOW, &new_t);
	char c;
	int readc;

	readc = read(0, &c, 1);
	new_t.c_cc[VMIN] = 1;
	tcsetattr(0, TCSANOW, &new_t);
	if(readc == 1) {
		top_c = c;
		return 1;
	}
	return 0;
}

void go_home() {
	if(root == curr_dir)
		return;
	go_back_stack.push(string(curr_dir));
	curr_dir_path(root);
}

void go_back() {
	if(go_back_stack.size() == 0)
		return;
	string back = go_back_stack.top();
	go_back_stack.pop();
	go_forward_stack.push(string(curr_dir));
	curr_dir_path(back.c_str());
}

void go_forward() {
	if(go_forward_stack.size() == 0)
		return;
	string forward = go_forward_stack.top();
	go_forward_stack.pop();
	go_back_stack.push(string(curr_dir));
	curr_dir_path(forward.c_str());
}

void go_up_level() {
	if(root == curr_dir)
		return;
	go_back_stack.push(string(curr_dir));
	curr_dir_path("../");	
}

void clear_cmd_prompt() {
	cursor_movement(inp_line, 0);
	int i, j;
	for(i = inp_line; i < win.ws_row; i++) {
		for(j = 0; j < win.ws_col; j++) {
			cout << " ";
		}
	}
	cursor_movement(inp_line, 0);
}
void clear_input_line() {
	cursor_movement(inp_line, 0);
	int i, j;
	for(j = 0; j < win.ws_col; j++) {
		cout << " ";
	}
	cursor_movement(inp_line, 0);
}

void init_cmd_arr(string cmd) {
	cmd_str_arr.clear();
	int i = 0, len = cmd.size();
	string tmp;

	while(i < len) {
		tmp = "";
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
		i++;
		cmd_str_arr.push_back(tmp);
	}
}

void copy_dir(string current_dir, string dest_dir) {
	DIR *dp;
	struct dirent *entry;
	struct stat meta;

	//cursor_movement(status_line, 0);
	if(!(dp = opendir(current_dir.c_str()))) { 
		//cout << "INVALID DIRECTORY PATH: " << current_dir.c_str();
		return;
	}
	chdir(current_dir.c_str());
	//cout << "Hi" << endl;

	while((entry = readdir(dp))) {
		lstat(entry->d_name,&meta);
		//cout << "Hi2" << endl;
		//cout << 0;
		if(S_ISDIR(meta.st_mode)) {
			//cout << "Hi3" << endl;
			string a(entry->d_name);
			if(a/*entry->d_name*/ == "." || a/*entry->d_name*/ == "..") 
				continue;
			mkdir((dest_dir + "/" + a/*entry->d_name*/).c_str(), S_IRUSR|S_IWUSR|S_IXUSR);
			//cout << "Hi4: " << dest_dir + "/" + a << endl;
			copy_dir(current_dir + "/" + a/*entry->d_name*/, dest_dir + "/" + a/*entry->d_name*/);
			//cout << "Hi5" << current_dir + "/" + a << endl;
		}
		else
			copy(entry->d_name, dest_dir);
	}
	chdir("..");
	closedir(dp);
}

void copy(string file, string position) {
	char word[2048];
	string dest = position + "/" + file;
	int read_c, open_in, open_out;

	struct stat meta;
	stat(file.c_str(), &meta);
	
	if(!S_ISDIR(meta.st_mode)) {
		//cursor_movement(status_line, 0);
		//cout << "Inside if";
		open_in = open(file.c_str(), O_RDONLY);
		open_out = open(dest.c_str(), O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);

		read_c = read(open_in, word, sizeof(word));
		while(read_c > 0) {
			write(open_out, word, read_c);
			read_c = read(open_in, word, sizeof(word));
		}
	}
	else {
		cursor_movement(status_line, 0);
		//cout << "Inside else: " << dest;
		mkdir(dest.c_str(), S_IRUSR|S_IWUSR|S_IXUSR);
		string curr(curr_dir);
		copy_dir(curr + "/" + file, dest);
	}
	
	chmod(dest.c_str(), meta.st_mode);
	//cursor_movement(status_line, 0);
	//cout << meta.st_mode;
}

//vector<string> temp_path;
void delete_dir(string dir_path) {
	DIR *dir;
	struct dirent *entry;
	struct stat meta;
	dir = opendir(dir_path.c_str());
	if(!dir) {
		cursor_movement(status_line, 0);
		//cout << "Current Directory NOT found !!: " << dir_path << endl;
		return;
	}
	chdir(dir_path.c_str());
	
	while((entry = readdir(dir)) != NULL) {
		//cursor_movement(status_line, 0);
		//cout << "Inside delete_dir: " << entry->d_name;
	
		lstat(entry->d_name, &meta);
		string a(entry->d_name);
		if(S_ISDIR(meta.st_mode)) {
			if(a/*entry->d_name*/ == "." || a/*entry->d_name*/ == "..") {
				continue;
			}
			delete_dir(dir_path + "/" + a/*entry->d_name*/);
			//cout << dir_path + "/" + a/*entry->d_name*/ << endl;
			//temp_path.push_back(entry->d_name);
			remove((dir_path + "/" + a/*entry->d_name*/).c_str());
		}
		else {
			//cout << dir_path + "/" + a/*entry->d_name*/ << endl;
			remove((dir_path + "/" + a/*entry->d_name*/).c_str());
			//temp_path.push_back(entry->d_name);
		}
		//entry = readdir(dir);
	}
	chdir("..");
	//remove(dir_path);
	closedir(dir);
}

void search(string file_name, string current_dir) {
	//int res = 0;
	DIR *dir;
	struct dirent *entry;
	struct stat meta;
	dir = opendir(current_dir.c_str());
	if(!dir) {
		cursor_movement(status_line, 0);
		//cout << "Current Directory NOT found: " << current_dir << endl;
		return;
	}
	//entry = readdir(dir);
	chdir(current_dir.c_str());
	while((entry = readdir(dir)) != NULL) {
		lstat(entry->d_name, &meta);

		//string a(file_name);
		string b(entry->d_name);
		//string c(current_dir);
		//cout << current_dir + "/" + b << endl;
		if(S_ISDIR(meta.st_mode)) {
			if(b/*entry->d_name*/ == "." || b/*entry->d_name*/ == "..") {
				continue;
			}
			if(file_name == b/*entry->d_name*/) {
				clear_cmd_prompt();
				cursor_movement(op_line, 0);
				cout << "True";
				search_count++;
				return;
				//break;
			}
			search(file_name, (current_dir + "/" + b)/*entry->d_name*/);
		}
		else {
			if(file_name == b/*entry->d_name*/) {
				clear_cmd_prompt();
				cursor_movement(op_line, 0);
				cout << "True";
				search_count++;
				return;
				//break;
			}
		}
		//entry = readdir(dir);
	}
	chdir("..");
	closedir(dir);
	if(search_count == 0) {
		clear_cmd_prompt();
		cursor_movement(op_line, 0);
		cout << "False";
	}
	//return res;
}

void command_mode() {
	off_keys();
	clear_cmd_prompt();
	string cmd;
	char c;
	int flag = 0;
	while(1) {
		clear_input_line();
		cursor_movement(inp_line, 1);
		cout << "~:";
		cursor_movement(inp_line, 3);
		
			
		getline(cin>>ws, cmd);
		/*cmd = "";
		c = getchar();
		while(c != '\n') {
			if(c == ESC) {
				init_keys();
				curr_dir_path(curr_dir);
				cursor_movement(cursor, 0);
				return;
			}
			cmd += c;
			c = getchar();
		}*/

		init_cmd_arr(cmd);

		/*cursor_movement(op_line, 0);
		for(int i = 0; i < cmd_str_arr.size(); i++) {
			cout << cmd_str_arr[i] << endl;
		}*/
		//init_keys();
		//curr_dir_path(curr_dir);
		//cursor_movement(cursor, 0);
		//break;

		/*string str = "";
	  	char ch;
	  	while (1) {
	  		ch = cin.get();
	  		if(ch == 27) {
	  			curr_dir_path(curr_dir);
	  			cursor_movement(first, 0);
	  			flag = 1;
	  			break;
	  		}
	  		str += ch;
	  	}
	  	if(flag == 1)
	  		break;*/
		if(cmd_str_arr[0] == "copy") {
			int len = cmd_str_arr.size();
			string file, position;
			/*if(cmd_str_arr[len - 1][0] == '~') {
				pos = root + cmd_str_arr[len - 1].substr(1);
			}
			else if(cmd_str_arr[len - 1][0] == '/') {
				pos = curr_dir + cmd_str_arr[len - 1];
			}
			else {
				clear_cmd_prompt();
				cursor_movement(status_line, 0);
				cout << "INVALID PATH !!";
				continue;
			}*/
			if(cmd_str_arr[len - 1][0] == '~') {
				position = root + cmd_str_arr[len - 1].substr(1);
			}
			else if(cmd_str_arr[len - 1][0] == '/') {
				position = curr_dir + cmd_str_arr[len - 1];
			}
			else if(cmd_str_arr[len - 1][0] == '.') 
				position = curr_dir;
			else {
				clear_cmd_prompt();
				cursor_movement(status_line, 0);
				cout << "INVALID PATH GIVEN !!";
				continue;
			}
			for(int i = 1; i < len - 1; i++) {
				file = cmd_str_arr[i];
				copy(file, position);
			}
			//clear_cmd_prompt();
			cursor_movement(op_line, 0);
			cout << "Copied the desired files to the directory: " << position;
		}
		else if(cmd_str_arr[0] == "move") {
			int flag, len = cmd_str_arr.size();
			string file, pos;
			if(cmd_str_arr[len - 1][0] == '~') {
				pos = root + cmd_str_arr[len - 1].substr(1);
			}
			else if(cmd_str_arr[len - 1][0] == '/') {
				pos = curr_dir + cmd_str_arr[len - 1];
			}
			else {
				clear_cmd_prompt();
				cursor_movement(status_line, 0);
				cout << "INVALID PATH !!";
				continue;
			}
			struct stat meta;
			for(int i = 1; i < len - 1; i++) {
				file = cmd_str_arr[i];
				copy(file, pos);

				const char* temp = file.c_str();
				lstat(temp, &meta);
				if(S_ISDIR(meta.st_mode)) {
					delete_dir(string(curr_dir) + "/" + file);
				}
				//else
				/*flag = */remove((string(curr_dir) + "/" + file).c_str());
			}
			clear_cmd_prompt();
			cursor_movement(op_line, 0);
			//string t = string(curr_dir) + '/' + file;
			cout << "Moved the desired files to the directory: " << pos;
		}
		else if(cmd_str_arr[0] == "rename") {
			if(cmd_str_arr.size() == 3) {
				string file1 = cmd_str_arr[1], file2 = cmd_str_arr[2];
				rename(file1.c_str(), file2.c_str());
				clear_cmd_prompt();
				cursor_movement(op_line, 0);
				cout << "RENAMED '" << file1 << "' to '" << file2 << "' SUCCESSFULLY !!";		
			}
			else {
				clear_cmd_prompt();
				cursor_movement(status_line, 0);
				cout << "INVALID Expression !!";		
			}
		}
		else if(cmd_str_arr[0] == "create_file" || cmd_str_arr[0] == "create_dir") {
			string position;
			int i, len = cmd_str_arr.size();
			if(cmd_str_arr[len - 1][0] == '~') {
				position = root + cmd_str_arr[len - 1].substr(1);
			}
			else if(cmd_str_arr[len - 1][0] == '/') {
				position = curr_dir + cmd_str_arr[len - 1];
			}
			else if(cmd_str_arr[len - 1][0] == '.') 
				position = curr_dir;
			else {
				clear_cmd_prompt();
				cursor_movement(status_line, 0);
				cout << "INVALID PATH GIVEN !!";
				continue;
			}

			for(i = 1; i < len - 1; i++) {
				if(cmd_str_arr[0] == "create_file") {
					open((position + '/' + cmd_str_arr[i]).c_str(), O_WRONLY|O_CREAT, S_IRUSR|S_IWUSR);
				}
				else {
					mkdir((position + '/' + cmd_str_arr[i]).c_str(), S_IRUSR|S_IWUSR|S_IXUSR);
				}
			}
			clear_cmd_prompt();
			cursor_movement(status_line, 0);
			cout << "DESIRED FILE(S)/DIR(S) CREATED SUCCESSFULLY !!";
		}
		else if(cmd_str_arr[0] == "delete_file") {
			//int len = cmd_str_arr.size();
			string path_file;
			if(cmd_str_arr[1][0] == '~') {
				path_file = root + cmd_str_arr[1].substr(1);
			}
			else if(cmd_str_arr[1][0] == '/') {
				path_file = curr_dir + cmd_str_arr[1];
			}
			else {
				clear_cmd_prompt();
				cursor_movement(status_line, 0);
				cout << "INVALID PATH: " << cmd_str_arr[1];
				continue;
			}
			clear_cmd_prompt();
			int flag = remove(path_file.c_str());
			if(flag == 0) { 
				cursor_movement(op_line, 0);
				cout << "DELETED the desired file SUCCESSFULLY !!";	
			}
			else {
				cursor_movement(status_line, 0);
				cout << "remove() ERROR: " << path_file;		
			}
		}
		else if(cmd_str_arr[0] == "delete_dir") {
			string path_dir;
			if(cmd_str_arr[1][0] == '~') {
				path_dir = root + cmd_str_arr[1].substr(1);
			}
			else if(cmd_str_arr[1][0] == '/') {
				path_dir = curr_dir + cmd_str_arr[1];
			}
			else {
				clear_cmd_prompt();
				cursor_movement(status_line, 0);
				cout << "INVALID PATH !!";
				continue;
			}
			clear_cmd_prompt();
			cursor_movement(status_line, 0);
			delete_dir(path_dir);
			remove(path_dir.c_str());
			//int flag = 0;//remove(path_dir.c_str());
			//if(flag == 0) { 
			cursor_movement(op_line, 0);
			cout << "DELETED the desired directory SUCCESSFULLY !!: " << path_dir;	
			//cout << temp_path.size() << path_dir;
				/*for(int i = 0; i < temp_path.size(); i++) {
					cout << temp_path[i] << endl;
				}*/
			//}
			//else {
			//	cursor_movement(status_line, 0);
			//	cout << "remove() ERROR !!";		
			//}
		}
		else if(cmd_str_arr[0] == "goto") {
			if(cmd_str_arr[1][0] == '/') {
				//init_keys();
				clear_cmd_prompt();
				curr_dir_path((root + cmd_str_arr[1]).c_str());
				//cursor_movement(inp_line, 0);
				//break;
			}	
		}
		else if(cmd_str_arr[0] == "search") {
			search_count = 0;
			string name = cmd_str_arr[1];
			//search_stack = stack<string>();
			string a(curr_dir);
			search(name, a/*curr_dir*/);
			
			/*clear_cmd_prompt();
			cursor_movement(op_line, 0);
			if(res == 1) {
				cout << "True";
			}
			else 
				cout << "False";*/
		}
		else if(cmd_str_arr[0] == "exit") {
			init_keys();
			curr_dir_path(curr_dir);
			cursor_movement(cursor, 0);
			break;
		}
	}
}