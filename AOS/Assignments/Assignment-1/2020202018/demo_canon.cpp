#include "my_header.h"
#include <iostream>
using namespace std;

#define ENTER 10
#define ESC 27
#define BACKSPACE 127

int main() {
	init_keys();
	string s;
	char c;
	while(1) {
        c = getchar();
        if(c == ESC)
        	break;
        if(c == ENTER) {
        	break;
        }
        if(c == BACKSPACE) {
        	//c = '\b';
        	cout << "\b \b";
        	s.pop_back();
        }
        else {
	        s += c;
	        cout << c;
	    }        
    }
	cout << s << endl;
	off_keys();
	return 0;
}