typedef long long ll;  

typedef struct deque_node {
	int val;
	struct deque_node *back, *front;
} Deque_Node;

class new_deque {
public:
	Deque_Node *head, *tail; 
	new_deque() {
		head = tail = NULL;

	}
	new_deque(int n, int x) {
		Deque_Node* tmp = (Deque_Node*)malloc(sizeof(Deque_Node));
		tmp->val = x;
		tmp->front = NULL;
		tmp->back = NULL;

		head = tail = tmp;
		
		for(int i = 0; i < n - 1; i++) {
			tmp = (Deque_Node*)malloc(sizeof(Deque_Node));
			tmp->val = x;
			tmp->back = tail;
			tmp->front = NULL;
			tail->front = tmp;
			tail = tmp;
		}
	}
	void push_back(int x) {
		Deque_Node* tmp = (Deque_Node*)malloc(sizeof(Deque_Node));
		tmp->val = x;
		if(head == NULL) {
			tmp->front = tmp->back = NULL;
			head = tail = tmp;
		}
		else {
			tmp->back = tail;
			tmp->front = NULL;
			tail->front = tmp;
			tail = tmp;
		}
	}
	void pop_back() {
		Deque_Node* tmp = tail;
		if(tail == NULL) return;
		else if(tail->back == NULL) {
			head = tail = NULL;
			tmp->front = tmp->back = NULL;
			free(tmp);
			return;
		}
		tail = tail->back;
		tail->front = NULL;
		tmp->back = tmp->front = NULL;
		free(tmp);
	}
	void push_front(int x) {
		Deque_Node* tmp = (Deque_Node*)malloc(sizeof(Deque_Node));
		tmp->val = x;
		if(head == NULL) {
			tmp->front = tmp->back = NULL;
			head = tail = tmp;
		}
		else {
			tmp->back = NULL;
			tmp->front = head;
			head->back = tmp;
			head = tmp;
		}
	}
	void pop_front() {
		Deque_Node* tmp = head;
		if(head == NULL) return;
		else if(head->front == NULL) {
			head = tail = NULL;
			tmp->front = tmp->back = NULL;
			free(tmp);
			return;
		}
		head = head->front;
		head->back = NULL;
		tmp->back = tmp->front = NULL;
		free(tmp);
	}
	int front() {
		if(head)
			return head->val;
		else
			return -1;
	}
	int back() {
		if(tail)
			return tail->val;
		else
			return -1;
	}
	bool empty() {
		if(head == NULL)
			return true;
		return false;
	}
	int size() {
		int s = 0;
		Deque_Node* tmp = head;
		while(tmp != NULL) {
			s++;
			tmp = tmp->front;
		}
		return s;
	}
	void resize(int x, int d) {
		int curr_size = size(), i;
		if(curr_size > x) {
			for(i = 0; i < (curr_size - x); i++) {
				pop_back();
			}
		}
		else if(curr_size < x) {
			for(i = 0; i < (x - curr_size); i++) {
				push_back(d);
			}
		}
	}
	void clear() {
		Deque_Node *tmp = head, *tmp2;
		while(tmp != NULL) {
			tmp2 = tmp;
			tmp = tmp->front;
			tmp2->front = tmp2->back = NULL;
			free(tmp);
		}
		head = tail = NULL;
	}
	
	int operator[](int n) {
		if(n > size()) return -1;
		Deque_Node *tmp = head;
		for(int i = 0; i < n - 1; i++) {
			tmp = tmp->front;
		}
		return tmp->val;
	} 
};
