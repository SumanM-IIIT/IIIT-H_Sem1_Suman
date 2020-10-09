#include<iostream>
#include<climits>
using namespace std;

class Student {
public:
    int rollno;
  	string name;
  	Student() {
  		rollno = 0;
  		name = "";
  	}
  	Student(string n, int r) {
  		name = n;
  		rollno = r;
  	}
  
  	bool operator==(const Student &s) const {  
        return rollno == s.rollno; 
  	}
  	bool operator>(const Student &s) const {  
        return rollno > s.rollno; 
  	}
  	bool operator<(const Student &s) const { 
        return rollno < s.rollno; 
  	}
  	bool operator!=(const Student &s) const {  
        return rollno != s.rollno; 
  	}
  	bool operator>=(const Student &s) const {  
        return rollno >= s.rollno; 
  	}
  	bool operator<=(const Student &s) const {  
        return rollno <= s.rollno; 
  	}
  	bool operator-(const Student &s) const {  
        return rollno - s.rollno; 
  	}
  	friend ostream& operator<<(ostream& o, const Student &s) {
		o << "(" << s.name << ", Roll: " << s.rollno << ")";
		return o;
	}
};

/*class Compare { 
public: 
   	bool operator()(Student a, Student b) { 
        return a.name == b.name;
    } 
};*/
typedef long long ll;
template <typename T>
class AVL {
public:
	class AVL_node {
	public:
		T value;
		ll height, cnt;
		AVL_node *left, *right;
	};
	AVL_node* root;
	AVL() {
		root = NULL;
	}

	AVL(T value) {
		root->value = value;
		root->left = root->right = NULL;
	}
	AVL_node* avl_insert(AVL_node* node, T value) {
		if(!node) {
			node = (AVL_node*)malloc(sizeof(AVL_node));
			node->value = value;
			node->cnt = 1;
			node->left = node->right = NULL;
		}
		else if(value > node->value) {
			node->right = avl_insert(node->right, value);
			if(balance(node) < -1) {
				if(value > node->right->value) {
					node = RR(node);
				}
				else {
					node = RL(node);
				}
			}
		}
		else if(value < node->value) {
			node->left = avl_insert(node->left, value);
			if(balance(node) > 1) {
				if(value < node->left->value)
					node = LL(node);
				else
					node = LR(node);
			}
		}
		else 
			node->cnt++;
		node->height = avl_height(node);
		return node;
	}
	void avl_insert(T value) {
		root = avl_insert(root, value);
	}
	AVL_node* avl_delete(AVL_node* node, T val) {
		AVL_node *tmp;	
		if(!node)
			return NULL;
		else if(val > node->value) {
			node->right = avl_delete(node->right, val);
			if(balance(node) > 1) {
				if(balance(node->left) >= 0)
					node = LL(node);
				else
					node = LR(node);
			}
		}
		else if(val < node->value) {
			node->left = avl_delete(node->left, val);
			if(balance(node) < -1) {
				if(balance(node->right) <= 0)
					node = RR(node);
				else
					node = RL(node);
			}
		}
		else {
			if(node->cnt > 1) {
				node->cnt--;
				return node;
			}
			if(node->right) {
				tmp = node->right;
				while(tmp->left)
					tmp = tmp->left;
				node->value = tmp->value;
				node->right = avl_delete(node->right, tmp->value);
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
	void avl_delete(T value) {
		root = avl_delete(root, value);
	}
	ll avl_height(AVL_node* node) {
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
		return max(left_h, right_h);
	}
	ll balance(AVL_node* node) {
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
	AVL_node* RR(AVL_node* node) {
		node = left_rotation(node);
		return node;	
	}
	AVL_node* RL(AVL_node* node) {
		node->right = right_rotation(node->right);
		node = left_rotation(node);
		return node;
	}
	AVL_node* LL(AVL_node* node) {
		node = right_rotation(node);
		return node;
	}
	AVL_node* LR(AVL_node* node) {
		node->left = left_rotation(node->left);
		node = right_rotation(node);
		return node;
	}
	AVL_node* left_rotation(AVL_node* node) {
		AVL_node* tmp;
		tmp = node->right;
		node->right = tmp->left;
		tmp->left = node;
		node->height = avl_height(node);
		tmp->height = avl_height(tmp);	
		return tmp;
	}
	AVL_node* right_rotation(AVL_node* node) {
		AVL_node* tmp;
		tmp = node->left;
		node->left = tmp->right;
		tmp->right = node;
		node->height = avl_height(node);
		tmp->height = avl_height(tmp);
		return tmp;
	}
	void print_avl_inorder(AVL_node* root) {
		if(root) {
			print_avl_inorder(root->left);
			if(root->cnt > 1) {
				int cnt = root->cnt;
				for(int i = 0; i < cnt; i++) 
					cout << root->value << " ";	
			}
			else
				cout << root->value << " ";
			print_avl_inorder(root->right);
		}
	}
	void print_avl_inorder() {
		print_avl_inorder(root);
	}
	AVL_node* avl_search(AVL_node* node, T val) {
		if (!node || node->value == val) 
	       	return node; 
	    if (node->value > val) 
	       	return avl_search(node->left, val);
	    else 
			return avl_search(node->right, val); 
	}
	bool avl_search(T val) {
		AVL_node* tmp = avl_search(root, val);
		if(tmp)
			return true;
		return false;
	}
	ll avl_count_occur(T val) {
		AVL_node* tmp = avl_search(root, val);
		if(tmp)
			return tmp->cnt;
		return 0;
	}
	ll count_range(AVL_node* node, T x, T y) {
		if(!node)
			return 0;
		if(node->value == x && node->value == y)
			return node->cnt;
		if(node->value < x)
			return count_range(node->right, x, y);
		else if(node->value > y)
			return count_range(node->left, x, y);
		else 
			return (node->cnt + count_range(node->left, x, y) + count_range(node->right, x, y));
	}
	ll count_range(T x, T y) {
		return count_range(root, x, y);
	}
	void k_largest(AVL_node* node, ll *cnt, ll k) {
		if(node) {
			k_largest(node->right, cnt, k);
			if(node->cnt > 1) {
				ll dem = node->cnt;
				for(ll i = 0; i < dem; i++) {
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
	void k_largest(ll *cnt, ll k) {
		k_largest(root, cnt, k);
	}
	AVL_node* lower_bound(AVL_node* node, T val) {
		if (!node->left && !node->right && node->value < val) 
			return NULL;  
		if ((node->value >= val && !node->left) || (node->value >= val && node->left->value < val)) 
			return node; 
		if (node->value <= val) 
			return lower_bound(node->right, val); 
		else
			return lower_bound(node->left, val);
	}
	void lower_bound(T val) {
		AVL_node* node = lower_bound(root, val);
		if(node)
			cout << node->value << endl;
		else
			cout << "N/A" << endl;
	}
	AVL_node* upper_bound(AVL_node* node, T val) {
		if (!node->left && !node->right && node->value <= val) 
			return NULL;  
		if ((node->value > val && !node->left) || (node->value > val && node->left->value <= val)) 
			return node; 
		if (node->value <= val) 
			return upper_bound(node->right, val); 
		else
			return upper_bound(node->left, val);
	}
	void upper_bound(T val) {
		AVL_node* node = upper_bound(root, val);
		if(node)
			cout << node->value << endl;
		else
			cout << "N/A" << endl;
	}
	void closest(AVL_node* node, T k, T &close, T &close_val) { 
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
			closest(node->left, k, close, close_val); 
		else
			closest(node->right, k, close, close_val); 
	} 
	T closest(T k) { 
		AVL_node* tmp = root;
		while(tmp->right) {
			tmp = tmp->right;
		}
		T close = tmp->value, close_val; 
		closest(root, k, close, close_val); 
		return close_val; 
	}
	void closest_student(AVL_node* node, int k, int &close, int &close_val) { 
		if (!node) 
			return; 
		if ((node->value).rollno == k) { 
			close_val = k; 
			return; 
		} 
		if (close > abs((node->value).rollno - k)) { 
			close = abs((node->value).rollno - k); 
			close_val = (node->value).rollno; 
		} 
		if (k < (node->value).rollno) 
			closest_student(node->left, k, close, close_val); 
		else
			closest_student(node->right, k, close, close_val); 
	} 
	T closest_student(int k) { 
		/*AVL_node* tmp = root;
		while(tmp->right) {
			tmp = tmp->right;
		}*/
		int close = INT_MAX, close_val; 
		closest_student(root, k, close, close_val); 
		T *s_tmp;
		s_tmp = new Student("", close_val);
		AVL_node *tmp2 = avl_search(root, *s_tmp);
		return tmp2->value; 
	}
};


int main() {
	AVL<Student> a;
	//string inp1, x, y; 
	int inp1, x, y; 
	ll cnt, k;
	Student *s, *s2;
	string s_name;
	int s_roll;
	int op;
	bool tmp;
	//Student s(25, "Suman Mitra");
	//cout << s << endl;
	while(1) {
		cnt = 0;
		cin >> op;
		switch(op) {
			case 1: cin >> s_name >> s_roll;
					s = new Student(s_name, s_roll);
					a.avl_insert(*s);
					//cin >> inp1;								
					//a.avl_insert(inp1);													//INSERT NODE
					break;
			case 2:	cin >> s_roll;
					s = new Student("", s_roll);
					a.avl_delete(*s);														//DELETE NODE
					//cin >> inp1;
					//a.avl_delete(inp1);				
					break;
			case 3: cin >> s_roll;
					s = new Student("", s_roll);
					tmp = a.avl_search(*s);													//SEARCH NODE
					//cin >> inp1;
					//tmp = a.avl_search(inp1);
					if(tmp)
						cout << "PRESENT" << endl;
					else
						cout << "NOT PRESENT" << endl; 
					break;
			case 4: cin >> s_roll;
					s = new Student("", s_roll);
					cout << a.avl_count_occur(*s) << endl;									//COUNT OCCURENCES OF AN ELEMENT
					//cin >> inp1;
					//cout << a.avl_count_occur(inp1) << endl;
					break;
			case 5:	cin >> s_roll;
					s = new Student("", s_roll);	
					a.lower_bound(*s);														//LOWER BOUND
					//cin >> inp1;
					//a.lower_bound(inp1);
					break;
			case 6: cin >> s_roll;
					s = new Student("", s_roll);	
					a.upper_bound(*s);														//UPPER BOUND
					//cin >> inp1;
					//a.upper_bound(inp1);
					break;
			case 7: cin >> s_roll;
					//s = new Student("", s_roll);	
					cout << a.closest_student(s_roll) << endl;								//CLOSEST ELEMENT TO SOME VALUE
					//cin >> inp1;
					//cout << a.closest(inp1) << endl;
					break;
			case 8: cin >> k;
					a.k_largest(&cnt, k);													//K-th LARGEST ELEMENT
					break;
			case 9: cin >> x >> y;
					s = new Student("", x);
					s2 = new Student("", y);		
					cout << a.count_range(*s, *s2) << endl;									//COUNT ELEMENTS IN A GIVEN RANGE
					//cin >> x >> y;
					//cout << a.count_range(x, y) << endl;
					break;
			case 0: exit(1);
			default: cout << "Enter Values b/w 0 to 9" << endl;
					break;

		}
		a.print_avl_inorder();
		cout << endl;
	}
	return 0;
}