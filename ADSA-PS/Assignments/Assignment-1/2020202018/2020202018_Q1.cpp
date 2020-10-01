#include <iostream>
#include <string>
#include <cstring>
#include <iomanip>
using namespace std;

typedef struct node_stack {
	char op;
	struct node_stack* next;
} Node_Stack;

Node_Stack *top = NULL;

int stack_empty() {
	return top == NULL;
}
char stack_top() {
	if(!stack_empty())
		return top->op;
	return -1;
}
void push(char op) {
	Node_Stack *temp = (Node_Stack*)malloc(sizeof(struct node_stack));
	temp->op = op;
	temp->next = NULL;
	if(!stack_empty()) {
		temp->next = top;
	}
	top = temp; 
}
char pop() {
	char val;
	if(top) {
		val = top->op;
		Node_Stack* temp;
		temp = top;
		top = top->next;
		temp->next = NULL;
		free(temp);
	}
	return val;
}



typedef struct node_list {
	char* operand;
	struct node_list *next;
} Node_List;

int is_operator_or_bracket(char c) {
	if(c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '(' || c == ')')
		return 1;
	return 0;
}
int is_operator(char c) {
	if(c == '+' || c == '-' || c == '*' || c == '/' || c == '%')
		return 0;
	return 1;
}

Node_List *head = NULL;
void insert_list(char* opr) {
	Node_List *temp = (Node_List*)malloc(sizeof(Node_List));
	temp->operand = (char *)malloc(strlen(opr) + 1);
	strcpy(temp->operand, opr);
	temp->next = NULL;
	if(head == NULL) 
		head = temp;
	else {
		Node_List *h2 = head;
		while(h2->next != NULL)
			h2 = h2->next;
		h2->next = temp;
	}
}

int opr_precedence(char ch) {
	if(ch == '+' || ch == '-')
		return 1;
	else if(ch == '*' || ch == '/' || ch == '%')
		return 2;
	else return 0;
}

void infix_to_postfix(char* infix) {
	Node_Stack *stack;
	int i = 0, n = strlen(infix);
	char x;
	while(i < n) {
		if(infix[i] >= '0' && infix[i] <= '9') {
			int k = 0;
			char *temp = (char *)malloc(sizeof(char) * n + 1);
			temp[k++] = infix[i++];
			while((infix[i] == '.' || (infix[i] >= '0' && infix[i] <= '9')) && infix[i] != '\0') {
				temp[k++] = infix[i++];
			}
			insert_list(temp);
		}
		else { 
			if(infix[i] == '(') {
				push(infix[i]);
			}
			else if(infix[i] == ')') {
				while(!stack_empty() && stack_top() != '(') {
					x = pop();
					insert_list(&x);
				}
				if(!stack_empty() && stack_top() != '(') return;
				else pop();
			}
			else {
				while(!stack_empty() && opr_precedence(infix[i]) <= opr_precedence(stack_top())) {
					x = pop();
					insert_list(&x);
				}
				push(infix[i]);
			}
			i++;
		}
	}
	while(!stack_empty()) {
		x = pop();
		insert_list(&x);
	}
}


typedef struct eval_stack {
	double val;
	struct eval_stack* next;
}E_Stack;

E_Stack *top2 = NULL;

int e_stack_empty() {
	return top2 == NULL;
}
char e_stack_top() {
	if(!e_stack_empty())
		return top2->val;
	return -1;
}
void e_push(double op) {
	E_Stack *temp = (E_Stack*)malloc(sizeof(struct eval_stack));
	temp->val = op;
	temp->next = NULL;
	if(!e_stack_empty()) {
		temp->next = top2;
	}
	top2 = temp; 
}
double e_pop() {
	double val = 0;
	if(top2) {
		val = top2->val;
		E_Stack* temp;
		temp = top2;
		top2 = top2->next;
		temp->next = NULL;
		free(temp);
	}
	return val;
}

int is_operand(char *str) {
	if(str[0] >= '0' && str[0] <= '9') {
		return 1;
	}
	return 0;
}

double postfix_evaluation() {
	Node_List* tmp = head;
	while(tmp != NULL) {
		if(is_operand(tmp->operand) == 1) {
			e_push(stod(tmp->operand));
		}
		else {
			double p1, p2;
			p1 = e_pop();
			p2 = e_pop();
			if(tmp->operand[0] == '+') 
				e_push(p2 + p1);
			else if(tmp->operand[0] == '-') 
				e_push(p2 - p1);
			else if(tmp->operand[0] == '*') 
				e_push(p2 * p1);
			else if(tmp->operand[0] == '/') 
				e_push(p2 / p1);
			else if(tmp->operand[0] == '%') {
				int x = (int)p1, y = (int)p2;
				e_push((double)(y % x));
			}
		}
		tmp = tmp->next;
	}
	return e_pop();
}

int main() {
	char *query = (char *)malloc(sizeof(char) * 1000);
	//cout << "Input an arithmetic expression: ";
	cin >> query;

	infix_to_postfix(query);
	double result = postfix_evaluation();
	
	cout << setprecision(6) << result << endl;
	return 0;
}