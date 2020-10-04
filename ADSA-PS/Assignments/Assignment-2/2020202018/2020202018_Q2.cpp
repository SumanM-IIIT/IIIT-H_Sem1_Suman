#include <iostream>
#include <sstream>
using namespace std;

#define SIZE 100000
typedef long long ll;

template<typename K, typename V>
class unordered_map {
public:
	//ll big_prime = 99991;
	ll bucket_size = SIZE;
	class map_node {
	public:
		K key;
		V value;
		map_node* next;
	};
	map_node* umap[SIZE];

	unordered_map() {
		ll i = 0;
		while(i < SIZE) {
			umap[i++] = NULL;
		}
	}
	int char_val(char c) {
		int res;
		if(c >= 'a' && c <= 'z') 
			res = c - 'a' + 1;
		else if(c >= 'A' && c <= 'Z') 
			res = c - 'A' + 27;
		else if(c >= '0' && c <= '9') 
			res = c - '0' + 53;
		else if(c == '_')
			res = 63;
		else if(c == ' ')
			res = 64;
		else if(c == '-')
			res = 65;
		else if(c == '.')
			res = 66;
		else if(c == '@')
			res = 67;
		else if(c == '$')
			res = 68;
		else if(c == '(')
			res = 69;
		else if(c == ')')
			res = 70;
		else
			res = 71;
		return res; 
	}
	ll hash_function(K key) {
		ostringstream os;
        os << key;
        string key_str = os.str(); 
        const int p = 73, m = 99991;
		ll hash = 0, p_power = 1;
		for(char c: key_str) {
			hash = (hash + char_val(c) * p_power) % m;
			p_power = (p_power * p) % m;
		}
		return hash;
	}
	void insert_umap(K key, V value) {
		ll pos = hash_function(key);
		map_node* first = umap[pos];
		if(!first) {
			map_node* tmp = new_node(key, value);
			umap[pos] = tmp;
		}
		else if(!first->next) {
			if(first->key != key) {
				map_node* tmp = new_node(key, value);
				first->next = tmp;
			}
		}
		else {
			map_node* tmp = first;
			while(tmp->next && tmp->key != key) {
				tmp = tmp->next;
			}
			if(tmp->key != key) {
				map_node* tmp2 = new_node(key, value);
				tmp->next = tmp2;
			}
		}
	}
	void delete_umap(K key) {
		ll pos = hash_function(key);
		map_node* first = umap[pos];
		map_node* tmp = first, *prev;
		if(first) {
			if(first->key == key) {
				first = first->next;
				umap[pos] = first;
				free(tmp);
			}
			else {
				while(tmp->next && tmp->key != key) {
					prev = tmp;
					tmp = tmp->next;
				}
				if(tmp->next && tmp->key == key) {
					prev->next = tmp->next;
					free(tmp);
				}
			}
		}
	}
	bool find_umap(K key) {
		ll pos = hash_function(key);
		map_node *first = umap[pos];
		//bool res;
		if(!first) {
			return false;
		}
		else {
			map_node *tmp = first;
			while(tmp) {
				if(tmp->key == key) 
					return true;
				tmp = tmp->next;
			}
			return false;
		}
	}
	void display_umap() {
		ll i = 0;
		while(i < bucket_size) {
			map_node* tmp = umap[i++];
			while(tmp) {
				cout << "(" << tmp->key << ", " << tmp->value << ")";
				tmp = tmp->next;
			}
			cout << endl;
		}
	}
	V operator[](K key) {
		ll pos = hash_function(key);
		return umap[pos]->value;
	}
	map_node* new_node(K key, V value) {
		map_node *tmp = new map_node;
		tmp->key = key;
		tmp->value = value;
		tmp->next = NULL;
		return tmp;
	}
};


int main() {
	unordered_map<string, string> m;
	string key;
	string value;
	int op;
	bool res;

	while(1) {
		cin >> op;
		switch(op) {
			case 1: cin >> key >> value;
					m.insert_umap(key, value);
					break;
			case 2: cin >> key;
					m.delete_umap(key);
					break;
			case 3: cin >> key;
					res = m.find_umap(key);
					if(res)
						cout << "True" << endl;
					else
						cout << "False" << endl;
					break;
			case 4: cin >> key;
					if(m.find_umap(key))
						cout << m[key] << endl;
					else
						cout << "N/A" << endl;
					break;
			case 0: exit(1);
			default: break;
		}
		//m.display_umap();
	}
	return 0;
}