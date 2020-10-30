import json
import sys

with open(sys.argv[1]) as f:
	json_dict = json.load(f)

emp_no = int(input("Enter the no. of employees: "))
empl_list = []
print("Enter Employees: ")
for i in range(0, emp_no):
	tmp = int(input())
	empl_list.append(tmp)


root_val = int(json_dict['L0'][0]['name'])

def parent(emp):
	#par = emp
	if emp == root_val:
		return None
	for level in json_dict.keys():
		for emp_list in json_dict[level]:
			if int(emp_list['name']) == emp:
				if 'parent' in emp_list:
					return int(emp_list['parent'])

def find_depth(child, par):
	count = 1
	if child == root_val:
		return 0
	while par != parent(child):
		count += 1
		child = parent(child)
	return count

emp_par_tmp = []
res = None
flag = 0
for i in range(0, len(empl_list)):
	emp_par_tmp.append(parent(empl_list[i]))
	if emp_par_tmp[i] is None:
		print('Common Parent NOT AVAILABLE')
		flag = 1
		break	
if(flag == 0):
	x = emp_par_tmp[0]
	for j in range(1, len(emp_par_tmp)):
		while x != emp_par_tmp[j]:
			if parent(x) is None or parent(emp_par_tmp[j]) is None:
				res = root_val
				break
			if x == parent(emp_par_tmp[j]):
				emp_par_tmp[j] = x
			elif emp_par_tmp[j] == parent(x):
				x = emp_par_tmp[j]
			else:
				x = parent(x)
				emp_par_tmp[j] = parent(emp_par_tmp[j])	
		res = x

	print("Common Parent: ", res)
	
	for e in empl_list:
		print(find_depth(e, res))