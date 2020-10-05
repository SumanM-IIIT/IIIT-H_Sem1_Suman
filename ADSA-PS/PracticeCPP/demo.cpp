#include <iostream>
#include <typeinfo>
using namespace std;
int main()
{
    // define an integer variable named x
    //string* x = NULL; // this variable is uninitialized because we haven't given it a value
    string a = "abc", b = "def";
    // print the value of x to the screen
    /*std::cout << typeid(x).name() << endl; 
    std::cout << x << endl;// who knows what we'll get, because x is uninitialized
 	*/
 	cout << a - b << endl;
    return 0;
}