#include<iostream>
using namespace std;

#define SIZE 100000
typedef long long ll;

template<typename K, typename V>
class unordered_map {
public:
	ll big_prime = 99991;
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
	ll hash_function(K key) {
		ostringstream os;
        os << key;
        string key_str = os.str(); 
		ll hash = 0, prod = 1;
		for(ll i = 0; i < key_str.size(); i++) {
			hash += (key_str[i] * prod);
            hash %= bucket_size;
            prod *= big_prime;
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
		return umap[key]->value;
	}
	map_node* new_node(K key, V value) {
		map_node *tmp = new map_node;
		tmp->key = key;
		tmp->value = value;
		tmp->next = NULL;
		return tmp;
	}
};

/*template<typename K, typename V> ll unordered_map::hash_function(K key) {
	return key % hash_mod;
}
template<typename K, typename V> void unordered_map::insert_umap(K key, V value) {
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
template<typename K, typename V> void unordered_map::delete_umap(K key) {
	
}
template<typename K, typename V> bool unordered_map::find_umap(K key) {
	ll pos = hash_function(key);
	node *first = umap[pos];
	//bool res;
	if(!first) {
		return false;
	}
	else {
		node *tmp = first;
		while(tmp) {
			if(tmp->key == key) 
				return true;
			tmp = tmp->next;
		}
		return false;
	}
}
template<typename K, typename V> void unordered_map::display_umap() {
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
template<typename K, typename V> V unordered_map::operator[](K key) {  
	return umap[key]->value;
}
template<typename K, typename V> map_node* unordered_map::new_node(K key, V value) {
	map_node *tmp = new map_node;
	tmp->key = key;
	tmp->value = value;
	tmp->next = nullptr;
	return tmp;
}*/


int main() {
	ll key;
	string value;
	unordered_map<ll, string> m;
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
		m.display_umap();
	}
	return 0;
}