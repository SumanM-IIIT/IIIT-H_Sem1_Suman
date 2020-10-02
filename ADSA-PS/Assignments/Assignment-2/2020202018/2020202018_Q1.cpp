#include<iostream>
using namespace std;

typedef struct AVL {
	int value, height, count = 0;
	struct AVL *left, *right;
} avl_node;

avl_node* insert_node(avl_node*, int);
avl_node* delete_node(avl_node*, int);
avl_node* search_node(avl_node*, int);
int count_occ(avl_node*, int);
//avl_node* height(avl_node*);
int balance(avl_node*);
avl_node* RR(avl_node*);
avl_node* RL(avl_node*);
avl_node* LL(avl_node*);
avl_node* LR(avl_node*);
avl_node* left_rotation(avl_node*);
avl_node* right_rotation(avl_node*);
int avl_height(avl_node*);
void print_inorder(avl_node*);


avl_node* insert_node(avl_node* node, int val) {		
	if(!node) {
		node = (avl_node*)malloc(sizeof(avl_node));
		node->value = val;
		node->count = 1;
		node->left = node->right = NULL;
	}
	else {
		if(val > node->value) {
			node->right = insert_node(node->right, val);
			if(balance(node) < -1) {
				if(val > node->right->value) {
					node = RR(node);
				}
				else {
					node = RL(node);
				}
			}
			node->height = avl_height(node);
		}
		else if(val < node->value) {
			node->left = insert_node(node->left, val);
			if(balance(node) > 1) {
				if(val < node->left->value)
					node = LL(node);
				else
					node = LR(node);
			}
			node->height = avl_height(node);
		}
		else {
			node->count++;
		}
	}
	return node;
}
avl_node* delete_node(avl_node* node, int val) {
	avl_node *tmp;	
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
avl_node* search_node(avl_node* node, int val) {
	if (!node || node->value == val) 
       	return node; 
    if (node->value > val) 
       	return search_node(node->left, val);
    else 
		return search_node(node->right, val); 
}
avl_node* RR(avl_node* node) {
	node = right_rotation(node);
	return node;
}
avl_node* RL(avl_node* node) {
	node->right = right_rotation(node->right);
	node = left_rotation(node);
	return node;
}
avl_node* LL(avl_node* node) {
	node = left_rotation(node);
	return node;
}
avl_node* LR(avl_node* node) {
	node->left = left_rotation(node->left);
	node = right_rotation(node);
	return node;
}
avl_node* left_rotation(avl_node* node) {
	avl_node *tmp;
	tmp = node->right;
	node->right = tmp->left;
	tmp->left = node;
	node->height = avl_height(node);
	tmp->height = avl_height(tmp);	
	return tmp;
}
avl_node* right_rotation(avl_node* node) {
	avl_node* tmp;
	tmp = node->left;
	node->left = tmp->right;
	tmp->right = node;
	node->height = avl_height(node);
	tmp->height = avl_height(tmp);
	return tmp;
}

int balance(avl_node* node) {
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
 
	return (left_h - right_h);
}
int avl_height(avl_node* node) {
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
int count_occ(avl_node* node, int val) {
	int count = 0;

	return count;
}
void print_inorder(avl_node* node) {
	if(node) {
		print_inorder(node->left);
		cout << node->value << "(Balance = " << balance(node) << "; Count = " << node->count << ")" << endl;
		print_inorder(node->right);
	}
}



int main() {
	avl_node *tmp, *root = NULL;
	int x, inp1, cnt;
	//bool srch;
	//cin >> x;
	while(1) {
		cin >> x;
		switch(x) {
			case 1: cin >> inp1; //INSERT
					root = insert_node(root, inp1);
					break;
			case 2: cin >> inp1; //DELETE
					root = delete_node(root, inp1);
					break;
			case 3: cin >> inp1; //SEARCH
					tmp = search_node(root, inp1);
					if(tmp)
						cout << tmp->value << endl;
					else
						cout << "NOT PRESENT" << endl;
					break;
			case 4: cin >> inp1; //COUNT_OCCURRENCE
					tmp = search_node(root, inp1);
					if(tmp)
						cout << tmp->count << endl;
					else
						cout << 0 << endl;
					break;
			case 5: break;
			case 6: break;
			case 7: break;
			case 8: break;
			case 9: break;
			case 0: exit(1);
			default: break;
		}
		print_inorder(root);
	}

	return 0;
}