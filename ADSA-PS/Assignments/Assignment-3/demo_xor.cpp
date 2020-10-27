#include <iostream>
#include <bits/stdc++.h>
using namespace std;

typedef long long ll;

typedef struct trie_node {
   struct trie_node *left, *right;
} Node;

Node* new_node() {
   Node* tmp = (Node *)malloc(sizeof(struct trie_node));
   tmp->left = tmp->right = NULL;
   return tmp;
}
void insert_trie_node(Node* root, ll val) {
   ll i, bit, byte = 8 * sizeof(ll);
   for(i = byte - 1; i >= 0; i--) {
      bit = val & (1 << i);
      if(bit) {
         if(!root->right)
            root->right = new_node();
         root = root->right;
      }
      else {
         if(!root->left)
            root->left = new_node();
         root = root->left;
      }
   }
}
Node* build_trie(vector<ll> A, int n) {
   Node* root = new_node();
   ll i;
   for(i = 0; i < n; i++)
      insert_trie_node(root, A[i]);
   return root;
}
ll max_xor(Node* root, ll x, ll pos_bit) {
   ll bit = 0, res = 0;
   if(!root || pos_bit < 0)
      return 0;
   if(pos_bit == 0) {
      if(x & (1 << pos_bit) == 0) {
         if(root->right)
            return 1;
         return 0;
      }
      else {
         if(root->left)
            return 1;
         return 0;
      }
   }
   if((x & (1 << pos_bit)) == 0) {
      if(root->right) {
         res = max_xor(root->right, x, pos_bit - 1);
         bit = 1;
      }
      else {
         res = max_xor(root->left, x, pos_bit - 1);
         bit = 0;
      }
   }
   else {
      if(root->left) {
         res = max_xor(root->left, x, pos_bit - 1);
         bit = 1;
      }
      else {
         res = max_xor(root->right, x, pos_bit - 1);
         bit = 0;
      }
   }
   if(bit)
      res |= (1 << pos_bit);
   else
      res &= ~(1 << pos_bit);
   return res;
}

int main() {
   ll i, n, q, k, query;
   cin >> n >> q;
   vector<ll> A(n);
   for(i = 0; i < n; i++) {
      cin >> k;
      A[i] = k;
   }
   Node *root = build_trie(A, n);
   for(i = 0; i < q; i++) {
      cin >> query;
      cout << "Max: " << max_xor(root, query, (8 * sizeof(ll)) - 1) << endl;
   }
   return 0;
}