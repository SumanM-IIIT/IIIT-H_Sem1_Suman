#include<bits/stdc++.h>

struct node_row {
	int row;
	struct node_value *next;
	struct node_row *down;
};
struct node_value {
	int col, val;
	struct node_value *next;
};

void create_value_list(int col, int val, struct node_row **row) {
	struct node_value *tmp;

	tmp = (struct node_value*)malloc(sizeof(struct node_value));
	tmp->col = col;
	tmp->val = val;
	tmp->next = NULL;

	if((*row)->next == NULL)
		(*row)->next = tmp;
	else {
		node_value *demo;
		demo = (*row)->next;
		while(demo->next)
			demo = demo->next;
		demo->next = tmp;
	}
}

void create_row_list(struct node_row **start, int rows, int cols, int s_mat[][3]) {
	struct node_row *tmp, *dem;
	int i, j;

	for(i = 0; i < rows; i++) {
		tmp = (struct node_row*)malloc(sizeof(struct node_row));
		tmp->row = i;
		tmp->next = NULL;
		tmp->down = NULL;

		if(i == 0) 
			(*start)->down = tmp;
		else {
			dem = (*start)->down;
			while(dem->down)
				dem = dem->down;
			dem->down = tmp;
		}

		for(j = 0; j < cols; j++) {
			if(s_mat[i][j] != 0) {
				create_value_list(j, s_mat[i][j], &tmp);
			}
		}
	}
}

/*void row_row_mul(int r, int cmn, int c, int A[][3], int B[][3]) {
	int i, j, sum;
	for(i = 0; i < r; i++) {
		for(j = 0; j < c; j++) {
			sum = 0;

		}
	}
}*/

int main(int argc, char const *argv[])
{
	/* code */
	int A[2][3] = {{3, 2, 0}, {1, 1, 2}};
	int B[3][3] = {{2, 1, 3}, {3, 3, 1}, {2, 2, 1}};
	
	struct node_row *Astart = NULL, *Bstart = NULL;
	create_row_list(&Astart, 2, 3, A);
	create_row_list(&Astart, 3, 3, B);

	//row_row_mul(2, 3, 3, &Astart, &Bstart);
	return 0;
}