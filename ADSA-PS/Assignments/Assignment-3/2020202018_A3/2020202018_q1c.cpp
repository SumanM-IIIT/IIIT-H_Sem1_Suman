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
        all_suff[i].rank[0] = s[i] - 'a'; 
        if(i + 1 < len) 
            all_suff[i].rank[1] = s[i + 1] - 'a';   
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
vector<ll> calculate_lcp(string str, vector<ll> suf_arr) { 
    ll i, j = 0, k, n = suf_arr.size(); 
    vector<ll> lcp_arr(n), inv_suf(n); 
    lcp_arr[0] = 0;

    for(i = 0; i < n; i++) 
        inv_suf[suf_arr[i]] = i; 
    
    for (i = 0; i < n; i++) { 
        if (inv_suf[i] == n - 1) { 
            j = 0; 
            continue; 
        }
        k = suf_arr[inv_suf[i] + 1]; 
        while (i + j < n && k + j < n && str[i + j] == str[k + j]) 
            j++; 
  
        lcp_arr[inv_suf[i] + 1] = j;
        if (j > 0) 
            j--; 
    }
    return lcp_arr; 
} 
string palindrome(vector<ll> suf_arr, vector<ll> lcp_arr, ll len, ll new_len, string str) {
    ll i, pal_len = 0, pos = 0;
    for(i = 1; i < new_len; i++) {
        if(lcp_arr[i] > pal_len) {
            if((suf_arr[i - 1] < len && suf_arr[i] > len) || (suf_arr[i] < len && suf_arr[i - 1] > len)) {
                pal_len = lcp_arr[i];
                pos = suf_arr[i];
            }
        }
    }
    //cout << "Pos: " << pos << endl;
    //cout << "Pal_Len: " << pal_len << endl;
    return str.substr(pos, pal_len);
}

int main() {
    string s, tmp1;
    cin >> s;
    ll len = s.length(); 
    tmp1 = s;
    reverse(tmp1.begin(), tmp1.end());
    s = s + "#" + tmp1;
    ll i, new_len = s.length();

    vector<ll> suf_arr = build_suffix_array(s, new_len);
    vector<ll> lcp_arr = calculate_lcp(s, suf_arr);

    /*cout << "Suffix Array: ";
    for(i = 0; i < suf_arr.size(); i++) 
        cout << suf_arr[i] << " ";
    cout << "\nLCP Array: ";
    for(i = 0; i < lcp_arr.size(); i++)
        cout << lcp_arr[i] << " ";
    cout << endl;*/

    s = palindrome(suf_arr, lcp_arr, len, new_len, s);
    cout << s << endl;
    return 0;
}