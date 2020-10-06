#include <iostream>
#include <typeinfo>
using namespace std;
int main()
{
    string def; 
    string str1 = "string";
    string strtype = typeid(def).name();
    /*if(strtype.find(str1) != string::npos)
		def = "default";
	else
		def = 0;*/
	cout << def << endl;
    return 0;
}