#include<iostream>
using namespace std;

typedef long long ll;

template <typename K, typename V>
class ordered_map {
public:
	class map_node {
	public:
		K key;
		V value;
		ll height;// count = 0;
		map_node *left, *right;
	};
	map_node *root;
	ll count = 0;
	//V def;// = (V*)malloc(sizeof(V));

	ordered_map() {
		root = NULL;
	}

	ordered_map(K key, V value) {
		root->key = key;
		root->value = value;
		root->left = root->right = NULL;
	}

	map_node* insert_omap_util(map_node* node, K key, V value) {
		//cout << " insert " << key << value << endl;
		//def = value;
		if(!node) {
			node = (map_node*)malloc(sizeof(map_node));
			node->key = key;
			node->value = value;
			count++;
			node->left = node->right = NULL;
		}
		else if(key > node->key) {
			node->right = insert_omap_util(node->right, key, value);
			if(balance(node) < -1) {
				if(key > node->right->key) {
					node = RR(node);
				}
				else {
					node = RL(node);
				}
			}
		}
		else if(key < node->key) {
			node->left = insert_omap_util(node->left, key, value);
			if(balance(node) > 1) {
				if(key < node->left->key)
					node = LL(node);
				else
					node = LR(node);
			}
		}
		else 
			node->value = value;
		node->height = omap_height(node);
		return node;
	}
	void insert_omap(K key, V value) {
		root = insert_omap_util(root, key, value);
	}
	/*map_node* min_key(map_node* node) {  
	    map_node* curr = node;  
	  	while (curr->left)  
	        curr = curr->left;  
	    return curr;  
	} */ 
	map_node* erase_omap(map_node* node, K key) {
		map_node *tmp;	
		if(!node)
			return NULL;
		else if(key > node->key) {
			node->right = erase_omap(node->right, key);
			if(balance(node) > 1) {
				if(balance(node->left) >= 0)
					node = LL(node);
				else
					node = LR(node);
			}
		}
		else if(key < node->key) {
			node->left = erase_omap(node->left, key);
			if(balance(node) < -1) {
				if(balance(node->right) <= 0)
					node = RR(node);
				else
					node = RL(node);
			}
		}
		else {
			count--;
			if(node->right) {
				tmp = node->right;
				while(tmp->left)
					tmp = tmp->left;
				node->key = tmp->key;
				node->value = tmp->value;
				node->right = erase_omap(node->right, tmp->key);
				if(balance(node) > 1) {
					if(balance(node->left) >= 0)
						node = LL(node);
					else
						node = LR(node);
				}
			}
			else
				return node->left;
		}
		node->height = omap_height(node);
		return node;
	}
	void erase_omap(K key) {
		root = erase_omap(root, key);
	}
	bool find_omap(map_node* root, K key) {
		if (!root)
			return false;
		if (root->key == key) 
	       	return true; 
	    if (root->key > key) 
	       	return find_omap(root->left, key);
	    else 
			return find_omap(root->right, key);
	}
	bool find_omap(K key) {
		return find_omap(root, key);
	}
	map_node* binary_search(map_node* root, K key) {
		if (root->key == key) 
	       	return root; 
	    if (root->key > key) 
	       	return binary_search(root->left, key);
	    else 
			return binary_search(root->right, key);
	}
	/*V& find_omap_val(map_node* root, K key) {
		if (!root) {
			V def = (*this).def;//*((V*)malloc(sizeof(V)));
			//return def;
			//cout << " hi " << endl;
			(*this).root = insert_omap_util((*this).root, key, def);
			map_node* tmp = proxy((*this).root, key);
			return tmp->value;
		}
		if (root->key == key) 
	       	return root->value; 
	    if (root->key > key) 
	       	return find_omap_val(root->left, key);
	    else 
			return find_omap_val(root->right, key);
	}*/
	V& operator[](K key) {
		if (!find_omap(key)) {
			int tmp_val = 0;
			string s;
			V def;
			if(typeid(s).hash_code() == typeid(V).hash_code()) 
				def = V(to_string(tmp_val));///(*this).def;//*((V*)malloc(sizeof(V)));
			else
				def = tmp_val;
			//return def;
			//cout << " hi " << endl;
			(*this).root = insert_omap_util((*this).root, key, def);
		}
		map_node* tmp = binary_search((*this).root, key);
		return tmp->value;
	}
	ll size_omap() {
		return count;
	}
	map_node* clear_omap(map_node* root) {
		if(!root)
			return NULL;
		clear_omap(root->left);
		clear_omap(root->right);
		//free(root);
		erase_omap(root->key);
		//count--;
		return NULL;
	}
	void clear_omap() {
		root = clear_omap(root);
	}
	void print_omap_inorder(map_node* root) {
		if(root) {
			print_omap_inorder(root->left);
			cout << "(" << root->key << ": " << root->value << ") ";
			print_omap_inorder(root->right);
		}
	}
	/*map_node* new_node(K key, V value) {
		map_node *tmp = new map_node;
		tmp->key = key;
		tmp->value = value;
		tmp->left = tmp->right = NULL;
		return tmp;
	}*/

	map_node* RR(map_node* node) {
		node = left_rotation(node);
		return node;	
	}
	map_node* RL(map_node* node) {
		node->right = right_rotation(node->right);
		node = left_rotation(node);
		return node;
	}
	map_node* LL(map_node* node) {
		node = right_rotation(node);
		return node;
	}
	map_node* LR(map_node* node) {
		node->left = left_rotation(node->left);
		node = right_rotation(node);
		return node;
	}
	map_node* left_rotation(map_node* node) {
		map_node* tmp;
		tmp = node->right;
		node->right = tmp->left;
		tmp->left = node;
		node->height = omap_height(node);
		tmp->height = omap_height(tmp);	
		return tmp;
	}
	map_node* right_rotation(map_node* node) {
		map_node* tmp;
		tmp = node->left;
		node->left = tmp->right;
		tmp->right = node;
		node->height = omap_height(node);
		tmp->height = omap_height(tmp);
		return tmp;
	}
	ll omap_height(map_node* node) {
		ll left_h,right_h;
		if(!node)
			return 0;
		if(!node->left)
			left_h = 0;
		else
			left_h = 1 + node->left->height;	
		if(!node->right)
			right_h = 0;
		else
			right_h = 1 + node->right->height;
		return max(left_h, right_h);
	}
	ll balance(map_node* node) {
		ll left_h, right_h;
		if(!node)
			return 0;
		if(!node->left)
			left_h = 0;
		else
			left_h = 1 + node->left->height;
		if(!node->right)
			right_h = 0;
		else
			right_h = 1 + node->right->height;
		return (left_h - right_h);
	}
};


int main() {
	ordered_map<int, string> m;
	int key;
	string value;
	int op;
	bool res;

	while(1) {
		cin >> op;
		switch(op) {
			case 1: cin >> key >> value;
					m.insert_omap(key, value);
					break;
			case 2: cin >> key;
					m.erase_omap(key);
					break;
			case 3: cin >> key;
					res = m.find_omap(key);
					if(res)
						cout << "True" << endl;
					else
						cout << "False" << endl;
					break;
			case 4: cin >> key >> value;
					//m[key] = value;
					cout << m[key] << endl;
					m[key] = value;
					break;
			case 5: cout << m.size_omap() << endl;
					break;
			case 6: m.clear_omap();
					break;
			case 0: exit(1);
			default: break;
		}
		m.print_omap_inorder(m.root);
		cout << endl;
	}
	return 0;
}