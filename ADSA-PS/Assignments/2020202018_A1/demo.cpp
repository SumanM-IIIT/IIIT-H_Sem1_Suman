#include<iostream>
using namespace std;

struct list_node {
	string a;
	struct list_node *next;
};
struct list_node* head = NULL;
void insert_list(string val) {
	struct list_node *temp = (struct list_node *)malloc(sizeof(struct list_node));
	temp->a = val;
	temp->next = NULL;
	if(head == NULL) {
		head = temp;
	}
	else {
		struct list_node *dm;
		dm = head;
		while(dm->next != NULL) {
			dm = dm->next;
		}
		dm->next = temp;
	}
	//return head;
}

int main() {
	string str = "1.55";
	struct list_node *tmp, *itr;
	insert_list(str);
	insert_list("1.62");

	char c = '.';
	string stri(1, c);

	cout << "Head: " << head->a << endl;
	tmp = head;
	while(tmp != NULL) {
		cout << tmp->a << " " << stri << " > ";
		tmp = tmp->next;
	}
	
	return 0;
}