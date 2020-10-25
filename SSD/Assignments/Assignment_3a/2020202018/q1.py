import json
import sys

with open(sys.argv[1]) as f:
	json_dict = json.load(f)

emp1 = int(input())
emp2 = int(input())
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

x = parent(emp1)
y = parent(emp2)
res = None

if x is None or y is None:
	print('Common Parent NOT AVAILABLE')
else:
	while x != y:
		if parent(x) is None or parent(y) is None:
			res = root_val
			break
		if x == parent(y):
			y = x
		elif y == parent(x):
			x = y
		else:
			x = parent(x)
			y = parent(y)	
	res = x

	print(res)
	print(find_depth(emp1, res))
	print(find_depth(emp2, res))

	
