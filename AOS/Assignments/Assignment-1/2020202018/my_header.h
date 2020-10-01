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
using namespace std;


int maxi_rows;


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
int all_ok();
void command_mode();
void clear_cmd_prompt();
string file_perm(struct stat);
void init_input_arr();


static struct termios initial_t, new_t;
static int top_c = -1;
int cursor = 1, first = 0, last;
int start_line, op_line, inp_line, status_line;
char const* root;
struct winsize win;
char curr_dir[2048];
size_t dir_size = 2048;
vector<dirent*> dir_file_arr;
vector<string> input_str_arr;
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
		if(S_ISDIR(meta.st_mode)) 
			cout << "\033[1;31m" << left << setw(30) << dir_file_arr[i]->d_name << setw(20) << meta.st_size/1024 << setw(20) << file_perm(meta) << "\033[0m" << endl;
		else
			cout << "\033[1;34m" << left << setw(30) << dir_file_arr[i]->d_name << setw(20) << meta.st_size/1024 << setw(20) << file_perm(meta) << "\033[0m" << endl;
	}
	cursor_movement(start_line, 0);
	for(int i = 0; i < win.ws_col; i++) {
		cout << "-";
	}
	//cursor_movement(first, 0);
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
		if(pid == 0) {
			execl("/usr/bin/gedit", "gedit", dir_file, NULL);
			exit(1);
		}
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
	status_line = op_line + 1;
	inp_line = maxi_rows - 1;
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

int all_ok() {
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

void command_mode() {
	off_keys();
	clear_cmd_prompt();
	string cmd;
	int flag = 0;
	while(1) {
		cursor_movement(inp_line, 3);
		cout << "~:";
			
		getline(cin, cmd, '\n');
		init_input_arr();

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
	}
}