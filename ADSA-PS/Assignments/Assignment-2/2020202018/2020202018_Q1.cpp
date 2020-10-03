#include<iostream>
using namespace std;



template <typename T>
class AVL {
public:
	AVL<T> *left, *right;
	T value;
	int height = 0, count = 0;
	AVL(): left(NULL), right(NULL) {
		left = NULL;
		right = NULL;
	}

	AVL(T value): value(value), left(NULL), right(NULL) {
	}

	AVL(T value, AVL<T> *left, AVL<T> *right): value(value), left(left), right(right) {
	}

	static AVL<T>* insert_node(AVL<T>*, T);
	static AVL<T>* delete_node(AVL<T>*, T);
	static int balance(AVL<T>*);
	static AVL<T>* RR(AVL<T>*);
	static AVL<T>* RL(AVL<T>*);
	static AVL<T>* LL(AVL<T>*);
	static AVL<T>* LR(AVL<T>*);
	static AVL<T>* left_rotation(AVL<T>*);
	static AVL<T>* right_rotation(AVL<T>*);
	static int avl_height(AVL<T>*);
	static void print_inorder(AVL<T>*);
	static int count_elements(AVL<T>* node, T x, T y);
	static void k_largest(AVL<T>* node, int *cnt, int k);
	static AVL<T>* search_node(AVL<T>*, T);
	static int count_range(AVL<T>*, T, T);
	static void closest_main(AVL<T>*, T, T&, T&);
	static T closest(AVL<T>*, T);
	static AVL<T>* lower_bound(AVL<T>*, T);
	static AVL<T>* upper_bound(AVL<T>*, T);
	//static int count_occ(AVL<T>*, T);

	//static Node<T> *find(Node<T> *tree, T value);
	//static void walk(const Node<T> *tree);
};

template <typename T> AVL<T>* AVL<T>::insert_node(AVL<T>* node, T val) {
	if(!node) {
		node = (AVL<T>*)malloc(sizeof(AVL<T>));
		node->value = val;
		node->count = 1;
		node->left = node->right = NULL;
	}
	else if(val > node->value) {
		node->right = insert_node(node->right, val);
		if(balance(node) < -1) {
			if(val > node->right->value) {
				node = RR(node);
			}
			else {
				node = RL(node);
			}
		}
		//node->height = avl_height(node);
	}
	else if(val < node->value) {
		node->left = insert_node(node->left, val);
		if(balance(node) > 1) {
			if(val < node->left->value)
				node = LL(node);
			else
				node = LR(node);
		}
		//node->height = avl_height(node);
	}
	else {
		node->count++;
	}
	node->height = avl_height(node);
	return node;
}
template <typename T> AVL<T>* AVL<T>::delete_node(AVL<T>* node, T val) {
	AVL<T> *tmp;	
	if(!node)
		return NULL;
	else if(val > node->value) {
		node->right = delete_node(node->right, val);
		if(balance(node) > 1) {
			if(balance(node->left) >= 0)
				node = LL(node);
			else
				node = LR(node);
		}
	}
	else if(val < node->value) {
		node->left = delete_node(node->left, val);
		if(balance(node) < -1) {
			if(balance(node->right) <= 0)
				node = RR(node);
			else
				node = RL(node);
		}
	}
	else {
		if(node->right) {
			tmp = node->right;
			while(tmp->left)
				tmp = tmp->left;
			node->value = tmp->value;
			node->right = delete_node(node->right, tmp->value);
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
	node->height = avl_height(node);
	return node;
}
template<typename T> int AVL<T>::balance(AVL<T>* node) {
	int left_h, right_h;
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
template<typename T> AVL<T>* AVL<T>::RR(AVL<T>* node) {
	node = left_rotation(node);
	return node;	
}
template<typename T> AVL<T>* AVL<T>::RL(AVL<T>* node) {
	node->right = right_rotation(node->right);
	node = left_rotation(node);
	return node;
}
template<typename T> AVL<T>* AVL<T>::LL(AVL<T>* node) {
	node = right_rotation(node);
	return node;
}
template<typename T> AVL<T>* AVL<T>::LR(AVL<T>* node) {
	node->left = left_rotation(node->left);
	node = right_rotation(node);
	return node;
}
template<typename T> AVL<T>* AVL<T>::left_rotation(AVL<T>* node) {
	AVL<T> *tmp;
	tmp = node->right;
	node->right = tmp->left;
	tmp->left = node;
	node->height = avl_height(node);
	tmp->height = avl_height(tmp);	
	return tmp;
}
template<typename T> AVL<T>* AVL<T>::right_rotation(AVL<T>* node) {
	AVL<T>* tmp;
	tmp = node->left;
	node->left = tmp->right;
	tmp->right = node;
	node->height = avl_height(node);
	tmp->height = avl_height(tmp);
	return tmp;
}
template<typename T> int AVL<T>::avl_height(AVL<T>* node) {
	int left_h,right_h;
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
template<typename T> void AVL<T>::print_inorder(AVL<T>* node) {
	if(node) {
		print_inorder(node->left);
		if(node->count > 1) {
			int cnt = node->count;
			for(int i = 0; i < cnt; i++) 
				cout << node->value << " ";	
		}
		else
			cout << node->value << " ";
		print_inorder(node->right);
	}
}
/*template<typename T> int AVL<T>::count_occ(AVL<T>* node, T val) {
	int count = 0;

	return count;
}*/
template<typename T> AVL<T>* AVL<T>::search_node(AVL<T>* node, T val) {
	if (!node || node->value == val) 
       	return node; 
    if (node->value > val) 
       	return search_node(node->left, val);
    else 
		return search_node(node->right, val); 
}
template<typename T> void AVL<T>::k_largest(AVL<T>* node, int *cnt, int k) {
	if(node) {
		k_largest(node->right, cnt, k);
		if(node->count > 1) {
			int dem = node->count;
			for(int i = 0; i < dem; i++) {
				(*cnt)++;
				if(*cnt == k) {
					cout << node->value << endl;
					return;
				}	
			}
		}
		else {
			(*cnt)++;
			if(*cnt == k) {
				cout << node->value << endl;
				return;
			}
		}
		k_largest(node->left, cnt, k);
	}
}
template<typename T> int AVL<T>::count_range(AVL<T>* node, T x, T y) {
	if(!node)
		return 0;
	if(node->value == x && node->value == y)
		return node->count;
	if(node->value < x)
		return count_range(node->right, x, y);
	else if(node->value > y)
		return count_range(node->left, x, y);
	else {
		return (node->count + count_range(node->left, x, y) + count_range(node->right, x, y));
	}
}
template<typename T> void AVL<T>::closest_main(AVL<T>* node, T k, T &close, T &close_val) { 
	if (!node) 
		return; 
	if (node->value == k) { 
		close_val = k; 
		return; 
	} 
	if (close > abs(node->value - k)) { 
		close = abs(node->value - k); 
		close_val = node->value; 
	} 
	if (k < node->value) 
		closest_main(node->left, k, close, close_val); 
	else
		closest_main(node->right, k, close, close_val); 
} 
template<typename T> T AVL<T>::closest(AVL<T>* node, T k) { 
	AVL<T>* tmp = node;
	while(tmp->right) {
		tmp = tmp->right;
	}
	T close = tmp->value, close_val; 
	closest_main(node, k, close, close_val); 
	return close_val; 
}
template<typename T> AVL<T>* AVL<T>::lower_bound(AVL<T>* node, T val) {
	if (!node->left && !node->right && node->value < val) 
		return nullptr;  
	if ((node->value >= val && !node->left) || (node->value >= val && node->left->value < val)) 
		return node; 
	if (node->value <= val) 
		return lower_bound(node->right, val); 
	else
		return lower_bound(node->left, val);
}
template<typename T> AVL<T>* AVL<T>::upper_bound(AVL<T>* node, T val) {
	if (!node->left && !node->right && node->value <= val) 
		return nullptr;  
	if ((node->value > val && !node->left) || (node->value > val && node->left->value <= val)) 
		return node; 
	if (node->value <= val) 
		return upper_bound(node->right, val); 
	else
		return upper_bound(node->left, val);
}



int main() {
	AVL<int> *tmp, *root = NULL;
	int inp1, x, y, cnt;
	int op;
	while(1) {
		cnt = 0;
		cin >> op;
		switch(op) {
			case 1: cin >> inp1;
					root = AVL<int>::insert_node(root, inp1);	//INSERT NODE
					break;
			case 2:	cin >> inp1;
					root = AVL<int>::delete_node(root, inp1);	//DELETE NODE
					break;
			case 3: cin >> inp1;
					tmp = AVL<int>::search_node(root, inp1);	//SEARCH NODE
					if(tmp)
						cout << "PRESENT: " << tmp->value << endl;
					else
						cout << "NOT PRESENT" << endl; 
					break;
			case 4: cin >> inp1;
					tmp = AVL<int>::search_node(root, inp1);	//COUNT OCCURENCES OF AN ELEMENT
					if(tmp)
						cout << tmp->count << endl;
					else
						cout << 0 << endl;
					break;
			case 5:	cin >> inp1;	
					tmp = AVL<int>::lower_bound(root, inp1);	//LOWER BOUND
					if(tmp)
						cout << tmp->value << endl;
					else 
						cout << "NULL" << endl;
					break;
			case 6: cin >> inp1;
					tmp = AVL<int>::upper_bound(root, inp1);	//UPPER BOUND
					if(tmp)
						cout << tmp->value << endl;
					else 
						cout << "NULL" << endl;
					break;
			case 7: cin >> inp1;
					cout << AVL<int>::closest(root, inp1) << endl;		//CLOSEST ELEMENT TO SOME VALUE
					break;
			case 8: cin >> inp1;
					AVL<int>::k_largest(root, &cnt, inp1);		//K-th LARGEST ELEMENT
					break;
			case 9: cin >> x >> y;
					cout << AVL<int>::count_range(root, x, y) << endl;	//COUNT ELEMENTS IN A GIVEN RANGE
					break;
			case 0: exit(1);
			default: cout << "Enter Values b/w 0 to 9" << endl;
					break;

		}
		AVL<int>::print_inorder(root);
		cout << endl;
	}
	/**root = AVL<int>::insert_node(root, 5);
	root = AVL<int>::insert_node(root, 2);
	root = AVL<int>::insert_node(root, 2);
	root = AVL<int>::insert_node(root, 3);
	root = AVL<int>::insert_node(root, 4);
	root = AVL<int>::insert_node(root, 1);
	root = AVL<int>::insert_node(root, 7);
	root = AVL<int>::insert_node(root, 9);
	AVL<int>::print_inorder(root);
	cout << endl;
	//root = AVL<int>::delete_node(root, 5);
	//cout << endl;
	AVL<int>::print_inorder(root);
	cout << endl;

	int cnt = 0;
	//AVL<int>* tmp = AVL<int>::k_largest(root, &cnt, 2);
	AVL<int>::k_largest(root, &cnt, 6);
	//cout << tmp->value << endl;
	AVL<int>::print_inorder(root);
	cout << endl;
	cout << AVL<int>::count_range(root, 2, 5) << endl;

	cout << "Closest: " << AVL<int>::closest(root, -3) << endl;

	AVL<int> *tmp;
	tmp = AVL<int>::upper_bound(root, 9);
	if(tmp)
		cout << "Upper Bound: " << tmp->value << endl;
	else
		cout << "NULL" << endl;
	/*int a = 10;
	float b = 10.23;
	string p = "ab", q = "xyz";
	decltype(a) x;
	decltype(b) y;
	decltype(p) z1;
	decltype(q) z2;
	cout << typeid(x).name() << endl;
	cout << typeid(y).name() << endl;
	cout << typeid(z1).name() << endl;
	cout << typeid(z2).name() << endl;*/
	return 0;
}