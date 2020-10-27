#include<bits/stdc++.h>
#include<fstream>
#include<sys/stat.h>
using namespace std;

typedef long long ll;

int main(int argc, char** argv) {
	struct stat buf;
	ofstream fptr1, p_dem;
  	ifstream fptr;
	ll num,i,j;
	int x = 0;
	string input(argv[1]);
	string output(argv[2]);
	if(stat(input.c_str(),&buf)==-1) {
		cout << "FILE NOT EXISTS !!" << endl;
		return 0;
	}
	vector<ll> v;	

	string demo_inpfile = "demo_input.txt";
	ifstream demo_in;
	ofstream demo_out;
	demo_in.open(input);
	demo_out.open(demo_inpfile);
	char c;
	while(demo_in >> c) {
		if(c == ',')
			demo_out << ' ';
		else
			demo_out << c;
	}
	demo_in.close();
	demo_out.close();
	//input = "demo_input.txt";

	//remove(demo_inpfile.c_str());

	fptr.open(input);
	i = 0;
	while(fptr >> num) {
		if(i % 1000000 == 0 && i != 0) {
		  i = 0;
		  string x1 = to_string(x);
		  fptr1.open(x1, fstream::out);
		  fptr1.close();
		  fptr1.open(x1);
		  sort(v.begin(), v.end());
		  for(j = 0;j < v.size(); j++)
		    fptr1 << v[j] << endl;
		  x++;
		  fptr1.close();
		  v.clear();
		}
		v.push_back(num);
		i++;
	}
	string x1 = to_string(x);
	fptr1.open(x1, fstream::out);
	fptr1.close();
	fptr1.open(x1);
	sort(v.begin(), v.end());
	for(j = 0; j < v.size(); j++)
	 	fptr1 << v[j] << endl;
	fptr.close();
	fptr1.close();
	fptr1.open(output, fstream::out);
	fptr1.close();
	fptr1.open(output);
	for(i = x; i >= 0; i--) {
		fptr.open(to_string(i));
		while(fptr >> num)
			fptr1 << num << endl;
		fptr.close();
		char *a;
		a = new char[to_string(i).length()+1];
		strcpy(a, to_string(i).c_str());
		remove(a);
	}
	fptr1.close();
	return 0;
}