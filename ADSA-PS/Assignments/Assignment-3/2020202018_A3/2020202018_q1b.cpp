#include<iostream>
#include<bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef struct suffix { 
    ll index, rank[2];
}Suffix;

int suffix_comparator(Suffix first, Suffix second) {
	if(first.rank[0] == second.rank[0]) {
		if(first.rank[1] < second.rank[1])
			return 1;
		else
			return 0;
	} 
	else if(first.rank[0] < second.rank[0]) 
		return 1;
	else
		return 0;
} 

vector<ll> build_suffix_array(string s, ll len) {
	Suffix all_suff[len];
	ll i, j; 
    for(i = 0; i < len; i++) { 
        all_suff[i].index = i; 
        all_suff[i].rank[0] = s[i] - '0'; 
        if(i + 1 < len) 
        	all_suff[i].rank[1] = s[i + 1] - '0';	
        else
        	all_suff[i].rank[1] = -1;
    } 

    sort(all_suff, all_suff + len, suffix_comparator); 

  	vector<ll> index2(len);
    for(j = 4; j < 2 * len; j *= 2) { 
        ll curr_rank = 0, prev_rank = all_suff[0].rank[0]; 
        all_suff[0].rank[0] = curr_rank; 
        index2[all_suff[0].index] = 0; 
  
        for(i = 1; i < len; i++) { 
            if (all_suff[i].rank[0] == prev_rank && all_suff[i].rank[1] == all_suff[i - 1].rank[1]) { 
                prev_rank = all_suff[i].rank[0]; 
                all_suff[i].rank[0] = curr_rank; 
            } 
            else { 
                prev_rank = all_suff[i].rank[0];
                curr_rank++; 
                all_suff[i].rank[0] = curr_rank; 
            } 
            index2[all_suff[i].index] = i; 
        } 
  
        for(i = 0; i < len; i++) { 
            ll next_ind = all_suff[i].index + j / 2; 
            if(next_ind < len)
            	all_suff[i].rank[1] = all_suff[index2[next_ind]].rank[0];
            else
             	all_suff[i].rank[1] = -1;
        } 
        sort(all_suff, all_suff + len, suffix_comparator); 
    } 
  
    vector<ll> suff_arr(len); 
    for (i = 0; i < len; i++) 
        suff_arr[i] = all_suff[i].index; 
    return suff_arr;
}

ll longest_common_substr(string tmp1, string tmp2) {
    ll len = tmp1.length(), len2 = tmp2.length(), i, cnt = 0;
    for(i = 0; i < len && i < len2; i++) {
        if(tmp1[i] == tmp2[i])
            cnt++;
        else
            break;
    }
    return cnt;
}


int main() {
	string s, tmp1, tmp2;
	cin >> s;
	ll k, i, len = s.length(), sub_str_len = -1, curr_len;
	cin >> k;

	vector<ll> suf_arr = build_suffix_array(s, len);

    for(i = 0; i <= len - k; i++) {
        tmp1 = s.substr(suf_arr[i]);
        tmp2 = s.substr(suf_arr[i + k - 1]);
        curr_len = longest_common_substr(tmp1, tmp2);
        if(sub_str_len < curr_len)
            sub_str_len = curr_len;
    }
    if(sub_str_len == 0)
        sub_str_len = -1;
    cout << sub_str_len << endl;	
	return 0;
}