#include<bits/stdc++.h>
#include<fstream>
#include<sys/stat.h>
using namespace std;

#define SIZE 1000000
#define LEN 5

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
void merge_two_arrays(ll *arr1, ll *arr2, ll n1, ll n2, ll file_n) {  
    ll i = 0, j = 0, k = 0, num;  
    ofstream f_out;
    ifstream f_in;
    f_out.open("tmp");
    while (i < n1 && j < n2) {  
        if (arr1[i] < arr2[j]) { 
            f_out << arr1[i++] << endl;
            //cout << arr1[i] << " ";
        }
        else {
            f_out << arr2[j++] << endl;  
            //cout << arr2[j] << " ";
        }
        k++;
    }  
    while (i < n1) { 
        f_out << arr1[i++] << endl;  
        //cout << arr1[i] << " ";
        k++;
    }
    while (j < n2) { 
        f_out << arr2[j++] << endl;  
        //cout << arr2[j] << " ";
        k++;
    }
    //cout << endl;
    f_out.close();

    f_out.open(to_string(file_n + 1));
	f_in.open("tmp");
	while(f_in >> num) {
		f_out << num << endl;
	}
	f_in.close();
	f_out.close();
	remove("tmp");
	remove(to_string(file_n).c_str());
}
void merge_files(ll file_i) {
	ll num, num2;
	ifstream in1, in2;
	/*in1.open(to_string(file_i));
	in2.open(to_string(file_i + 1));

	cout << "File-" << file_i << ": ";
	while(in1 >> num) 
		cout << num << " ";
	cout << endl;
	cout << "File-" << file_i + 1 << ": ";
	while(in2 >> num) 
		cout << num << " ";
	cout << endl;
	in1.close();
	in2.close();*/

	in1.open(to_string(file_i));
	in2.open(to_string(file_i + 1));

	ofstream fout;
	fout.open("tmp");
	in1 >> num;
	in2 >> num2;
	while(1) {
		/*if(!(in1 >> num))
			break;
		if(!(in2 >> num2))
			break;*/
		//cout << "Num: " << num << ", Num2: " << num2 << endl;
		if(num <= num2) {
			fout << num << endl;
			if(!(in1 >> num))
				break;
			//fout << num2 << endl;
			//cout << num << " ";
		}
		else {
			fout << num2 << endl;
			if(!(in2 >> num2))
				break;
			//fout << num << endl;
			//cout << num2 << " ";
		}
	}
	while(1) {
		//cout << "Num: " << num << endl;
		fout << num << endl;	
		if(!(in1 >> num))
			break;
		//cout << num << " ";
	}
	/*while(in2 >> num2) {
		cout << "Num2: " << num2 << endl;
		fout << num2 << endl;	
		//cout << num2 << " ";
	}*/
	//cout << endl;
	fout.close();
	in1.close();
	in2.close();
	remove(to_string(file_i).c_str());
	remove(to_string(file_i + 1).c_str());

	in1.open("tmp");
	fout.open(to_string(file_i + 1));
	while(in1 >> num)
		fout << num << endl;
	in1.close();
	fout.close();
	remove("tmp");
	//remove(to_string(file_i).c_str());
}


int main(int argc, char** argv) {
	string tmp = "";
	char a;
	ll file_cnt = 0, i, *arr, *arr2, k = 0, num, flag = 0;

	arr = (ll *)malloc(sizeof(ll) * SIZE);

	ifstream f_in; 
	f_in.open(argv[1]); 
	if (!f_in) { 
		perror("Error while opening the file.\n"); 
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
			f_out.open(to_string(file_cnt));
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
	f_out.open(to_string(file_cnt));
	for(i = 0; i < k; i++) {
		f_out << arr[i] << endl;
	}
	f_out.close();
	free(arr);
	ll len_last, out_len = 0, j;
	
	/*f_out.open(argv[2]);
	f_in.open("1");
	while(f_in >> num) {
		f_out << num << endl;
	}
	f_in.close();
	f_out.close();*/
	//ifstream in1, in2;
	i = 1;
	while(i < file_cnt) {
		/*out_len += LEN;
		if(i + 1 == file_cnt)
			len_last = k;
		else
			len_last = LEN;*/
		//arr = (ll *)malloc(sizeof(ll) * out_len);
		//arr2 = (ll *)malloc(sizeof(ll) * len_last);
		
		//in1.open(to_string(i));
		//in2.open(to_string(i + 1));
		
		/*ll p = 0;
		while(in1 >> num) {
			//cout << num << ": ";
			arr[p++] = num;
		}
		//cout << endl;
		p = 0;
		while(in2 >> num) {
			arr2[p++] = num;
		}*/
		merge_files(i);		
		//in1.close();
		//in2.close();

		//merge_two_arrays(arr, arr2, out_len, len_last, i);
		//free(arr);
		//free(arr2);
		i++;
	}
	f_out.open(argv[2]);
	f_in.open(to_string(file_cnt));
	while(f_in >> num) {
		f_out << num << " ";
	}
	f_in.close();
	f_out.close();

	remove(to_string(file_cnt).c_str());
	
	/*for(i = file_cnt; i >= 1; i--) {
		f_in.open(to_string(i));
		while(f_in >> num)
			f_out << num << " ";
		f_in.close();
		char *a;
		a = new char[to_string(i).length()+1];
		strcpy(a, to_string(i).c_str());
		remove(a);
	}
	f_out.close();*/

	return 0;
}