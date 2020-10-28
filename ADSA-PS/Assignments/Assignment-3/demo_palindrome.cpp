#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <string>
#include <cstring>
#include <cmath>
#include <bits/stdc++.h>
using namespace std;

#define SIZE 1000000
typedef long long ll;

string str; //input
ll rnk[SIZE], suf_arr[SIZE]; //output
ll cnt[SIZE], Next[SIZE]; //internal
bool bh[SIZE], b2h[SIZE];
ll Height[SIZE];

bool suffix_comparator(ll a, ll b) {
	return str[a] < str[b];
}
void build_suffix_tree(ll n) {
	for (ll i=0; i<n; ++i) {
		suf_arr[i] = i;
	}
	sort(suf_arr, suf_arr + n, suffix_comparator);
	
	for (ll i=0; i<n; ++i) {
		bh[i] = i == 0 || str[suf_arr[i]] != str[suf_arr[i-1]];
		b2h[i] = false;
	}

	for (ll h = 1; h < n; h <<= 1) {
		ll buckets = 0;
		for (ll i=0, j; i < n; i = j) {
			j = i + 1;
			while (j < n && !bh[j]) j++;
			Next[i] = j;
			buckets++;
		}
		if (buckets == n) break;
		
		for (ll i = 0; i < n; i = Next[i]) {
			cnt[i] = 0;
			for (ll j = i; j < Next[i]; ++j) {
				rnk[suf_arr[j]] = i;
			}
		}

		cnt[rnk[n - h]]++;
		b2h[rnk[n - h]] = true;
		for (ll i = 0; i < n; i = Next[i]) {
			for (ll j = i; j < Next[i]; ++j) {
				ll s = suf_arr[j] - h;
				if (s >= 0){
					ll head = rnk[s];
					rnk[s] = head + cnt[head]++;
					b2h[rnk[s]] = true;
				}
			}
			for (ll j = i; j < Next[i]; ++j) {
				ll s = suf_arr[j] - h;
				if (s >= 0 && b2h[rnk[s]]){
					for (ll k = rnk[s]+1; !bh[k] && b2h[k]; k++) b2h[k] = false;
				}
			}
		}
		for (ll i=0; i<n; ++i) {
			suf_arr[rnk[i]] = i;
			bh[i] |= b2h[i];
		}
	}
	for (ll i=0; i<n; ++i) {
		rnk[suf_arr[i]] = i;
	}
}

void getHeight(ll n) {
	for (ll i=0; i<n; ++i) rnk[suf_arr[i]] = i;
	Height[0] = 0;
	for (ll i=0, h=0; i<n; ++i) {
		if (rnk[i] > 0) {
			ll j = suf_arr[rnk[i]-1];
			while (i + h < n && j + h < n && str[i+h] == str[j+h]) {
				h++;
			}
			Height[rnk[i]] = h;
			if (h > 0) h--;
		}
	}
}

int main() {
	ll new_len, len;
	string tmp, new_str;
	cin >> str;

	len = str.length();
	tmp = str;
	reverse(tmp.begin(), tmp.end());
	str = str + "#" + tmp;
	new_len = str.length();
	
	build_suffix_tree(new_len);

	getHeight(new_len);

	ll i, res = 0;
	ll startptr = 0;
	for(i = 1; i < new_len; i++) {
		if((Height[i] > res)) {
			if((suf_arr[i - 1] < len && suf_arr[i] > len) || (suf_arr[i] < len && suf_arr[i - 1] > len)) {
				res = Height[i];
				startptr = suf_arr[i];
			}
		}
	}
	cout << "str: " << str << endl;
	cout << "start: " << startptr << ", " << str[startptr] << endl;
	cout << "End: " << res - 1 << ", " << str[startptr + res - 1] << endl;
	cout << str.substr(startptr, res) << endl;
	cout << "Suf_Arr: ";
	for(i = 0; i < new_len; i++)  {
		cout << suf_arr[i] << " ";
	}
	cout << endl;
	cout << "Height: ";
	for(i = 0; i < new_len; i++)  {
		cout << Height[i] << " ";
	}
	cout << endl;
	return 0;
}

