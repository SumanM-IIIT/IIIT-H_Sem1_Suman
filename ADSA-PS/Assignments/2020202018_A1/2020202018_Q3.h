#include<iostream>
using namespace std;

#define LEN 100000000

template <class T> 
class Deque {
	T *ar = new T[LEN];
	int first, last;
public:	
	Deque() {
		first = -1;
		last = -1;
		//ar = new T[LEN];
	}
	Deque(int n, T x) {
		first = 0;
		last = n - 1;
		//ar = new T[LEN];
		for(int i = 0; i < n; i++) {
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
		if(empty()) {
			cout << "Underflow !!\n";
			return;
		}
		if(first == last)
			first = last = -1;
		else {
			if(last = 0)
				last = LEN - 1;
			else 
				last--;
		}
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
			if(first = LEN - 1)
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
	int size() {
		if(empty())
			return 0;
		if(first <= last) 
			return last - first + 1;
		else {
			return (LEN - first) + last + 1;
		}
	}
	void resize(int x, T d) {
		int l = size();
		if(x < l) {
			for(int i = 0; i < (l - x); i++)
				pop_back();
		}
		else if(x > size()) {
			/*if(x <= LEN) { 
				

			}*/
			for(int i = 0; i < (x - l); i++) 
				push_back(d);
		}
		//LEN = x;
	}
	void clear() {
		first = last = -1;
		//length = 0;
	}
	T operator[](int n) {
		if(n > size() || n < 1) {
			cout << "Invalid location. Please enter values b/w 1 & " << size() << ". Exiting....";
			exit(1);
		}
		else {
			if(first > last && n > (LEN - first))
				return ar[n - (LEN - first) - 1];
			else
				return ar[first + n - 1];
			
		}
	}
};