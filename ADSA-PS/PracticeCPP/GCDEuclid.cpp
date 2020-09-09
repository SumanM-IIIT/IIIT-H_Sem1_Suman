#include <bits/stdc++.h>
using namespace std;
template <class T>
class demo {
	T x, y;
public:
	demo(T a, T b) {
		x = a;
		y = b;
	}
	void print() {
		cout << x << " " << y << endl;
	}
};
int main(int argc, char const *argv[])
{
	/*int a, b, swap;
	cin >> a >> b;
	if(b > a) {
		swap = a;
		a = b;
		b = swap;
	}
	int tmp;
	while(b != 0) {
		tmp = a % b;
		a = b;
		b = tmp;
	}
	cout << "The GCD: " << a << endl;
	*/
	demo<char> dm('a', 'b');
	dm.print();

	return 0;
}