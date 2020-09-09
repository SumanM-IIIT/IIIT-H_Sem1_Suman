#include<bits/stdc++.h>
#include<iostream>
#include<string>
using namespace std;

typedef unsigned long long ull;

class BigInt {
	string ar; 
	//int size;
public:
	BigInt() {
		ar = "";
		//size = LEN;
	}
	/*BigInt(int x) {
		ar = "";
		size = x;
	}*/
	BigInt(string s) {
		ar = s;
	}
	void append(int n) {
		ar += to_string(n);
	}
	int get_size() {
		return ar.length();
	}
	void print() {
		cout << ar;
	}
	/*BigInt operator+(BigInt b) {
		int tmp_len = max(b.get_size(), ar.get_size());	
	}*/
	void ar_reverse() {
		reverse(ar.begin(), ar.end());
	}
	string get_string() {
		return ar;
	}
	void set_string(string s) {
		ar = s;
	}
	friend ostream& operator << (ostream& d, BigInt& num) {
		d << num.get_string();
		return d;
	}
	int operator[](int n) {
		return ar[n] - 48;
	}
	/*BigInt operator+(BigInt b) {
		BigInt sum;
		//cout << "a: " << ar << ", b: " << b.get_string() << endl;
		int dm, carry = 0, as = this->get_size(), bs = b.get_size();
		int n = min(as, bs) - 1, ass = as - 1, bss = bs - 1;
		while(n >= 0) {
			dm = (ar[ass--] - 48) + b[bss--] + carry; 
			if(dm > 9) {
				dm = dm % 10;
				carry = 1;
			}
			else
				carry = 0;
			sum.append(dm);
			n--;
		}
		if(as > bs) {
			n = as - bs - 1;
			while(n >= 0) {
				dm = (ar[ass--] - 48) + carry;
				if(dm > 9) {
					carry = 1;
					dm = dm % 10;
				}
				else 
					carry = 0;
				sum.append(dm);
				n--;
			}
		}
		else if(bs > as) {
			n = bs - as - 1;
			while(n >= 0) {
				dm = b[bss--] + carry;
				if(dm > 9) {
					carry = 1;
					dm = dm % 10;
				}
				else 
					carry = 0;
				sum.append(dm);
				n--;
			}
		}
		if(carry == 1)
			sum.append(1);
		sum.ar_reverse();
		return sum;
	}*/
	void trim_zero_right() {
		int count = 0, n = get_size() - 1;
		while(n >= 0 && ar[n] == '0') {
			count++;
			n--;
		}
		ar.erase(get_size() - count);
	}
	void trim_zero_left() {
		int i, count = 0, n = get_size() - 1;
		for(i = 0; i < n; i++) {
			if(ar[n] == '0') 
				count++;
			else
				break;
		}
		ar.erase(0, count);
	}
	BigInt operator-(BigInt b) {
		BigInt diff;
		int as = this->get_size(), bs = b.get_size(), ass = as - 1, bss = bs - 1;
		int n = min(as, bs) - 1, borrow = 0, x, y;
		while(n >= 0) {
			//cout << "n: " << n << endl;
			x = ar[ass] - 48;
			y = b[bss];
			//cout << "x: " << x << ", y: " << y;
			if(borrow == 1) {
				x--;
			}
			if(x >= y) {
				diff.append(x - y);
				borrow = 0;
			}
			else {
				diff.append(x + 10 - y);
				borrow = 1;
			}
			ass--;
			bss--;
			n--;
			//cout << ", ans: " << diff.get_string() << endl;
		}
		if(as > bs) {
			n = as - bs - 1;
			while(n >= 0) {
				x = ar[ass] - 48;
				y = 0;
				if(borrow == 1) {
					x--;
				}
				if(x >= y) {
					diff.append(x - y);
					borrow = 0;
				}
				else {
					diff.append(x + 10 - y);
					borrow = 1;
				}
				ass--;
				//bss--;
				n--;
			}
		}
		diff.trim_zero_right();
		diff.ar_reverse();
		return diff;
	}
	/*BigInt operator%(BigInt b) {
		BigInt ans;
		string div = divide(ar, b.get_string());
		string mul = mult(b.get_string(), div);
		BigInt m(mul);
		return (this - m);	
	}*/
	int operator>(BigInt b) {
		int as = this->get_size(), bs = b.get_size();
		if(as > bs)
			return 1;
		else if(bs > as)
			return 0;
		else {
			for(int i = 0; i < as; i++) {
				//cout << " Inside for ";
				if((ar[i] - 48) > b[i]) {
					//cout << " Inside for if ";
					return 1;
				}
				else if((ar[i] - 48) < b[i]) {
					return 0;
				}
			}
			return 0;
		}
	}
	int operator>=(BigInt b) {
		int as = this->get_size(), bs = b.get_size();
		if(as > bs)
			return 1;
		else if(bs > as)
			return 0;
		else {
			for(int i = 0; i < as; i++) {
				//cout << " Inside for ";
				if((ar[i] - 48) > b[i]) {
					//cout << " Inside for if ";
					return 1;
				}
				else if((ar[i] - 48) < b[i]) {
					return 0;
				}
			}
			return 1;
		}
	}
	bool is_zero() {
		int n = this->get_size();
		for(int i = 0; i < n; i++) {
			if(ar[i] != '0')
				return false;
		}
		return true;
	}
};

/*string add(string a, string b) {
	string sum = "";
	//cout << "a: " << ar << ", b: " << b.get_string() << endl;
	int dm, carry = 0, as = a.length(), bs = b.length();
	int n = min(as, bs) - 1, ass = as - 1, bss = bs - 1;
	while(n >= 0) {
		dm = (a[ass--] - 48) + b[bss--] + carry; 
		if(dm > 9) {
			dm = dm % 10;
			carry = 1;
		}
		else
			carry = 0;
		sum += to_string(dm);
		n--;
	}
	if(as > bs) {
		n = as - bs - 1;
		while(n >= 0) {
			dm = (a[ass--] - 48) + carry;
			if(dm > 9) {
				carry = 1;
				dm = dm % 10;
			}
			else 
				carry = 0;
			sum += to_string(dm);
			n--;
		}
	}
	else if(bs > as) {
		n = bs - as - 1;
		while(n >= 0) {
			dm = b[bss--] + carry;
			if(dm > 9) {
				carry = 1;
				dm = dm % 10;
			}
			else 
				carry = 0;
			sum += to_string(dm);
			n--;
		}
	}
	if(carry == 1)
		sum += to_string(1);
	reverse(sum.begin(), sum.end());
	return sum;
}
*/

BigInt factorial(int n) {
	string res;
    res.append(to_string(1));
    int res_s, i, j, carry, mul;
    for(i = 2; i <= n; i++) {
        carry = 0;
        res_s = res.length();
        for(j = 0; j < res_s; j++) {
            mul = (res[j] - 48) * i + carry;
            res.replace(j, 1, to_string(mul % 10));
            carry = mul / 10;
        }
        while(carry) {
            res.append(to_string(carry % 10));
            carry /= 10;
        }        
    }
    reverse(res.begin(), res.end());
    BigInt ans(res);
	return ans;
}


string mult(string a, string b) { 
    int i, sa = a.length(), sb = b.length(); 
    if(sa == 0 || sb == 0) 
    	return "0"; 
  
    int res[sa + sb] = {0};   
    int itr1 = 0, itr2 = 0;        
    for(i = sa - 1; i >= 0; i--) { 
        int carry = 0; 
        int n1 = a[i] - '0'; 
  
        itr2 = 0;  
        for (int j=sb-1; j>=0; j--) { 
            int n2 = b[j] - '0'; 
            int sum = n1*n2 + res[itr1 + itr2] + carry; 
  
            carry = sum/10; 
            res[itr1 + itr2] = sum % 10; 
            itr2++; 
        } 
        if (carry > 0) 
            res[itr1 + itr2] += carry; 
        itr1++; 
    }   
    i = sa + sb - 1; 
    while(i >= 0 && res[i] == 0) 
    	i--;   
    if (i == -1) 
  		return "0"; 
  
    string smul = ""; 
    while (i >= 0) 
        smul += to_string(res[i--]); 
  
    return smul; 
} 

/*BigInt muldiff(BigInt a, BigInt b) {
	string i = "1";
	long long j = 1;
	BigInt d, c(mult(i, b.get_string()));
	while(a >= c) {
		d.set_string(c.get_string());
		i = mult(i, to_string(j));
		j++;
		c.set_string(mult(i, b.get_string()));
	}
	//cout << " (( Inside muldiff a: " << a << ", d: " << d << " )) ";
	//string s = findDiff(a.get_string(), d.get_string());
	//cout << " diff: " << s;
	//demo.trim_zero_left();
	//int k, count = 0, n = s.length();
	//for(k = 0; k < n; k++) {
	//	if(s[k] == '0')
	//		count++;
	//	else
	//		break;
	//}
	//s.erase(0, count);
	//cout << " diff after trim: " << s;
	//BigInt demo(s);
	//return demo;
	BigInt demo2 = a - d;
	return demo2;
}*/

/*BigInt GCD(BigInt a, BigInt b) {
	string as = a.get_string(), bs = b.get_string();
	BigInt x, a, b;
	if(as.compare(bs) == 0)
		return a;
	if(aa > bb) {
		a = aa;
		b = bb;
	}
	else {
		a = bb;
		b = aa;
	}
	cout << "INSIDE GCD(): \n";
	while(!b.is_zero()) {
		cout << "a: " << a << ", b: " << b;
		if(a > b) {
			//a = a - b;
			x.set_string(a.get_string());
			a = muldiff(a, b);
			cout << ", if a: " << a;
		}
		else {
			//b = b - a;
			x.set_string(b.get_string());
			b = muldiff(a, b);
			cout << ", else b: " << b;
		}
		cout << endl;
	}
	return x;
}*/
/*
BigInt GCD2(BigInt a, BigInt b) {
	if(a.is_zero())
		return b;
	if(a > b)
		return GCD2(muldiff(a, b), b);
	else
		return GCD2(muldiff(b, a), a);
}*/
/*

BigInt GCD3(BigInt a, BigInt b) {
	string as = a.get_string(), bs = b.get_string();
	
	if(as.compare(bs) == 0)
		return a;
	cout << "INSIDE GCD3(): \n";
	while(!b.is_zero()) {
		cout << "a: " << a << ", b: " << b;
		if(a > b) {
			a = muldiff(a, b); 
			cout << ", if a: " << a << " aiwee";
		}
		else {
			b = muldiff(b, a); 
			cout << ", else b: " << b;
		}
		cout << endl;
	}
	return a;
}*/
/*
BigInt GCD4(BigInt a, BigInt b) {
	BigInt large, small;
	if(a > b) {
		large = a;
		small = b;
	}
	else {
		large = b;
		small = a;
	}
	string i;
	long long k;
	string prev, sm, lr;
	BigInt s, l, tmp1, tmp2;
	
	while(1) {
		cout << "large: " << large.get_string();
		cout << ", small: " << small.get_string() << endl;
		s.set_string(small.get_string());
		l.set_string(large.get_string());
		i = "1";
		k = 1;
		while(large >= small) {
			prev = small.get_string();
			cout << "Inside while: prev: " << prev << ", i: " << i << endl;
			sm = mult(s.get_string(), i);
			i = mult(i, to_string(k));
			k++;
			small.set_string(sm);
		}
		if(prev.compare(large.get_string()) == 0) 
			return s;
		else {
			tmp1.set_string(prev);
			tmp2 = large - tmp1;
			if(tmp1 > tmp2) {
				large.set_string(tmp1.get_string());
				small.set_string(tmp2.get_string());
			}
			else {
				small.set_string(tmp1.get_string());
				large.set_string(tmp2.get_string());
			}
		}
	}
}*/

BigInt exp(BigInt a, long long x) {
	string as = a.get_string();
	string tmp = "1";
	while(x > 0) {
		if(x&1) {
			tmp = mult(tmp, as);
		}
		as = mult(as, as);
		x = x>>1;
	}
	BigInt ans(tmp);
	return ans;
}



string incr_one(string st) {
	int i, k, carry = 1, len = st.length();
	string res = "";

	for(i = len - 1; i >= 0; i--) {
		k = st[i] - 48 + carry;
		if(k >= 10) {
			res.append("0");
			carry = 1;
		}
		else {
			res.append(to_string(k));
			carry = 0;
		}
	}
	if(carry == 1) 
		res.append("1");
	reverse(res.begin(), res.end());
	return res;
}



BigInt minimize_big(BigInt large, BigInt small) {
	string res, i = "1", j = "1";
	BigInt tmp(mult(i, small.get_string()));
	while(large > tmp) {
		res = tmp.get_string();
		i = incr_one(i);
		//j = findSum(j, to_string(1));
		//cout << "Inside min_big() res: " << res << ", i: " << i << endl;
		tmp.set_string(mult(i, small.get_string()));
	}
	BigInt p(res), d;
	d = large - p;
	//cout << "Large: " << large << ", res: " << p << ", Min Version: " << d << endl;
	return d;
}

BigInt gcd_act(BigInt a, BigInt b) {
	string res;
	//cout << "Inside gcd_act(), a: " << a.get_string() << ", b: " << b.get_string() << endl;
	while(a.get_string().compare(b.get_string()) != 0) {
	     while(a > b) {
	         a = a - b;
	     }
	     while(b > a) {
	         b = b - a;
	     }
	}
	return a;
}

BigInt gcd_main(BigInt a, BigInt b) {
	BigInt large, small;
	if(a > b) {
		large = a;
		small = b;
	}
	else {
		large = b;
		small = a;
	}
	BigInt mini = minimize_big(large, small);
	BigInt res = gcd_act(small, mini);
	return res;
}


ull gcd_int(ull a, ull b) {
	if(b == 0)
		return a;
	return gcd_int(b, a % b);
}

ull min_large(string big, ull b) {
	ull res = 0;
	int i, size = big.length();
	for(i = 0; i < size; i++) 
		res = (res * 10 + big[i] - 48) % b;
	return res;
}

ull gcd_one_big(string big, ull b) {
	ull mini = min_large(big, b);
	return gcd_int(b, mini);
}

int main() {

	long long q;
	int n, fact_inp;
	long long exp_inp;
	string exp_base, gcd_inp1, gcd_inp2;

	cin >> q;
	while(q--) {
		cin >> n;
		if(n == 1) {
			cin >> exp_base >> exp_inp;
			BigInt ans1, tmp(exp_base);
			ans1 = exp(tmp, exp_inp);
			cout << ans1 << endl;
		}
		if(n == 2) {
			cin >> gcd_inp1 >> gcd_inp2;
			BigInt first(gcd_inp1), second(gcd_inp2), large, small;
			if(first > second) {
				large = first;
				small = second;
			}
			else {
				large = second;
				small = first;
			}
			if(large.get_size() <= 18 && small.get_size() <= 18) {
				ull a = stoull(large.get_string(), nullptr, 10), b = stoull(small.get_string(), nullptr, 10);
				cout << gcd_int(a, b) << endl;
			}
			else if(small.get_size() <= 18) {
				ull b = stoull(small.get_string(), nullptr, 10);
				if(b == 0) {
					cout << large << endl;
				}
				else {
					cout << gcd_one_big(large.get_string(), b) << endl;
				}
			}
			else {
				BigInt res3 = gcd_main(first, second);
				cout << res3 << endl;
			}
		}
		if(n == 3) {
			cin >> fact_inp;
			BigInt ans3 = factorial(fact_inp);
			cout << ans3 << endl;
		}
	}
	return 0;
}