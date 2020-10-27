#include<bits/stdc++.h>
#include<fstream>
#include<sys/stat.h>
using namespace std;

#define SIZE 1000000

typedef long long ll;


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
void merge_files(ll file_i) {
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
	ll len_last, out_len = 0, j;
	i = 1;
	while(i < file_cnt) {
		merge_files(i);	
		i++;
	}
	f_out.open(argv[2]);
	f_in.open(to_string(file_cnt));
	while(f_in >> num) {
		f_out << num << " ";
	}
	f_in.close();
	f_out.close();
	remove(("data/" + to_string(file_cnt)).c_str());

	return 0;
}