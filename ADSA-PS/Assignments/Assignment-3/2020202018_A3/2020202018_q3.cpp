#include<bits/stdc++.h>
#include<fstream>
#include<sys/stat.h>
#include<limits.h>
using namespace std;

#define SIZE 1000000

typedef long long ll;

ll min_heap[SIZE];
ll f_index[SIZE];


void merge(ll* arr, ll left, ll mid, ll right) {
	ll i, j, sub_n1 = mid - left + 1, sub_n2 = right - mid;
 
    ll left_arr[sub_n1], right_arr[sub_n2];
 
    for(i = 0; i < sub_n1; i++)
        left_arr[i] = arr[left + i];
    for(j = 0; j < sub_n2; j++)
        right_arr[j] = arr[mid + 1 + j];
 
    i = j = 0; 
    ll l = left;
     
    while (i < sub_n1 && j < sub_n2) {
        if (left_arr[i] <= right_arr[j]) {
            arr[l] = left_arr[i];
            i++;
        }
        else {
            arr[l] = right_arr[j];
            j++;
        }
        l++;
    }
	while (i < sub_n1) {
        arr[l] = left_arr[i];
        i++;
        l++;
    }
    while (j < sub_n2) {
        arr[l] = right_arr[j];
        j++;
        l++;
    }
}
void merge_sort(ll *arr, ll left, ll right) {
	if (left < right) {
        ll mid = left + (right - left) / 2;
 		merge_sort(arr, left, mid);
        merge_sort(arr, mid + 1, right);
		merge(arr, left, mid, right);
    }
}
/*void merge_files(ll file_i) {
	ll num, num2;
	ifstream in1, in2;

	in1.open("data/" + to_string(file_i));
	in2.open("data/" + to_string(file_i + 1));

	ofstream fout;
	fout.open("data/tmp");
	in1 >> num;
	in2 >> num2;
	while(1) {
		if(num <= num2) {
			fout << num << endl;
			if(!(in1 >> num))
				break;
		}
		else {
			fout << num2 << endl;
			if(!(in2 >> num2))
				break;
		}
	}
	while(1) {
		fout << num << endl;	
		if(!(in1 >> num))
			break;
	}
	fout.close();
	in1.close();
	in2.close();
	remove(("data/" + to_string(file_i)).c_str());
	remove(("data/" + to_string(file_i + 1)).c_str());

	in1.open("data/tmp");
	fout.open("data/" + to_string(file_i + 1));
	while(in1 >> num)
		fout << num << endl;
	in1.close();
	fout.close();
	remove("data/tmp");
}*/

void min_heapify(ll in, ll len) {
    ll left = (2 * in) + 1, right = (2 * in) + 2, mini;
    if (left < len && min_heap[left] < min_heap[in])
        mini = left;
    else
        mini = in;

    if (right < len && min_heap[right] < min_heap[mini])
        mini = right;

    ll tmp, tmp2;
    if (mini != in) {
        tmp = min_heap[in];
        min_heap[in] = min_heap[mini];
        min_heap[mini] = tmp;

        tmp2 = f_index[in];
        f_index[in] = f_index[mini];
        f_index[mini] = tmp2;
        min_heapify(mini, len);
    }
    return;
}
void build_min_heap(ll len) {
	ll i;
    for(i = (len / 2) + 2; i >= 0; i--) {
        min_heapify(i, len);
    }
}
void final_merge_out(char* out_file, ll file_cnt) {
	ofstream f_out;
    f_out.open(out_file);
    ifstream f_in[file_cnt];
    ll i;
    for(i = 0; i < file_cnt; i++) {
        f_in[i].open("data/" + to_string(i + 1));
    }
    ll num;
    for(i = 0; i < file_cnt; i++) {
        if(f_in[i] >> num) {
            min_heap[i] = num;
            f_index[i] = i;
        } 
    }
    build_min_heap(file_cnt);
    
    ll k = 0, tmp;
    while(k != file_cnt) {
        f_out << min_heap[0] << " ";
        
        if(f_in[f_index[0]] >> tmp)
        	min_heap[0] = tmp;
        else {
            min_heap[0] = LLONG_MAX;
            k++;
        }
        min_heapify(0, file_cnt);
    }
    for(i = 0; i < file_cnt; i++) {
        f_in[i].close();
        remove(("data/" + to_string(i + 1)).c_str());
    }
    f_out.close();
}


int main(int argc, char** argv) {
	string tmp = "";
	char a;
	ll file_cnt = 0, i, *arr, *arr2, k = 0, num, flag = 0;

	arr = (ll *)malloc(sizeof(ll) * SIZE);

	ifstream f_in; 
	f_in.open(argv[1]); 
	if (!f_in) { 
		perror("CAN'T OPEN THE INPUT FILE.\n"); 
		exit(EXIT_FAILURE); 
	}
	
	while(f_in >> a) {
		if(k >= SIZE) {
			if(tmp != "") {
				stringstream ss(tmp);
				ss >> num;
				arr[k++] = num;
			}
			merge_sort(arr, 0, k - 1);

			file_cnt++;
			ofstream f_out;
			f_out.open("data/" + to_string(file_cnt));
			for(i = 0; i < k; i++) {
				f_out << arr[i] << endl;
			}
			f_out.close();
			free(arr);
			arr = (ll *)malloc(sizeof(ll) * SIZE);
			k = 0;
		}
		if(a == ',') {
			stringstream ss(tmp);
			ss >> num;
			arr[k++] = num; 
			tmp = "";
			continue;
		}
		else 
			tmp += a;
	}
	f_in.close();

	if(tmp != "") {
		stringstream ss(tmp);
		ss >> num;
		arr[k++] = num;
	}
	merge_sort(arr, 0, k - 1);
	file_cnt++;
	ofstream f_out;
	f_out.open("data/" + to_string(file_cnt));
	for(i = 0; i < k; i++) {
		f_out << arr[i] << endl;
	}
	f_out.close();
	free(arr);
	
	final_merge_out(argv[2], file_cnt);
/*	for(i = 0; i < file_cnt; i++)
		remove(("data/" + to_string(i + 1)).c_str());

	ll len_last, out_len = 0, j;
	i = 1;
	while(i < file_cnt) {
		merge_files(i);	
		i++;
	}
	f_out.open(argv[2]);
	f_in.open("data/" +  to_string(file_cnt));
	while(f_in >> num) {
		f_out << num << " ";
	}
	f_in.close();
	f_out.close();
	remove(("data/" + to_string(file_cnt)).c_str());
*/
	return 0;
}