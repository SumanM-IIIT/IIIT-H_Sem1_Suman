#include<bits/stdc++.h>
using namespace std;

#define LEN 100000000

typedef long long ll;

template <class T> 
class Deque {
	T *ar;
	ll first, last;
public:	
	Deque() {
		ar = new T[LEN];
		first = -1;
		last = -1;
	}
	Deque(ll n, T x) {
		ar = new T[LEN];
		first = 0;
		last = n - 1;
		for(ll i = 0; i < n; i++) {
			ar[i] = x;
		}
	}
	void push_back(T k) {
		if(empty()) 
			first = last = 0;
		else if(last == LEN - 1)
			last = 0;
		else 
			last++;
		ar[last] = k;
	}
	void pop_back() {
		//cout << "Before pop: " << last;
		if(empty()) {
			cout << "Underflow !!\n";
			return;
		}
		if(first == last) {
			first = last = -1;
		}
		else {
			if(last == 0) {
				last = LEN - 1;
			}
			else  {
				last--;
			}
		}
		//cout << ", After pop: " << last << endl;
		//length--;
	}
	void push_front(T k) {
		if(first == -1) 
			first = last = 0;
		else if(first == 0)
			first = LEN - 1;
		else 
			first--;
		ar[first] = k;
		//length++;
	}
	void pop_front() {
		if(empty()) {
			cout << "Underflow !!\n";
			return;
		}
		if(first == last)
			first = last = -1;
		else {
			if(first == LEN - 1)
				first = 0;
			else 
				first++;
		}
		//length--;
	}
	T front() {
		if(empty()) {
			cout << "Deque is EMPTY. Exiting...";
			exit(1);
		}
		return ar[first];
	}
	T back() {
		if(empty()) {
			cout << "Deque is EMPTY. Exiting...";
			exit(1);
		}
		return ar[last];
	}
	bool empty() {
		return first == -1;
	}
	ll size() {
		if(empty())
			return 0;
		if(first <= last) 
			return last - first + 1;
		else {
			return (LEN - first) + last + 1;
		}
	}
	void resize(ll x, T d) {
		ll l = size();
		if(x < l) {
			for(ll i = 0; i < (l - x); i++)
				this->pop_back();
		}
		else if(x > size()) {
			for(ll i = 0; i < (x - l); i++) 
				this->push_back(d);
		}
		//LEN = x;
	}
	void clear() {
		first = last = -1;
		//length = 0;
	}
	T operator[](ll n) {
		/*if(n > this->size() || n < 0) {
			//cout << "Invalid location. Please enter values b/w 1 & " << size() << ". Exiting....";
			exit(1);
		}
		else {
			if(first > last && n > (LEN - first - 1))
				return ar[n - (LEN - first - 1) - 1];
			else
				return ar[first + n];
			
		}*/
		if(n >= this->size() || n < 0) {
			cout << "INVALID POSITION !!!" << endl;
			exit(1);
		}
		if(first <= last) {
			return ar[first + n];
		}
		else {
			return ar[(first + n) % LEN];
		}
	}
	void display() {
		ll i, s = this->size();
		for(ll i = 0; i < s; i++) {
			cout << (*this)[i] << " ";
		}
		cout << endl;
	}
};


int main() {
	/*Deque<double> d1(10, 4.3);
	d1.push_back(5.23423);
	d1.push_back(6);
	d1.push_back(73244);
	d1.push_back(8.1234);
	d1.push_front(9.0);

//	cout << d1.front() <<endl;
//	cout << d1.back() <<endl;
//	cout << d1[6] <<endl;
//	cout << d1[5] <<endl;
//	cout << d1[3] <<endl;
//	cout << d1[1] <<endl;
	d1.resize(18, 111);
	cout << "Size: " << d1.size() << endl;
	d1.display();
	cout << d1.empty();
//	cout << d1[0];*/
	
	return 0;
}