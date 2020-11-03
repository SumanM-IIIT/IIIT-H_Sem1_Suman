#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int main(int argc, char** argv) {
	cout << "Argc: " << argc << endl;
	cout << "Argv: ";
	for(int i = 0; i < argc; i++) {
		cout << argv[i] << " ";
	}
	cout << endl;
	return 0;
}