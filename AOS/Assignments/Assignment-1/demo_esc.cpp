#include <bits/stdc++.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
using namespace std;

int main() {
	int kb = open(kbd_dev, O_RDONLY);
	string str;
	char c;
	if(kb != -1) {
		c = read(kb, &ie, sizeof(ie));
		while(c != '\n'){
			cout <<"code: " << ie.code << ", value: " << ie.value << endl;
			c = read(kb, &ie, sizeof(ie));
		}				
		cout << str;
	}
	return 0;
}