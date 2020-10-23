import json

with open('org.json') as f:
	json_dict = json.load(f)

emp1 = int(input())
emp2 = int(input())
root_val = json_dict['L0'][0]['name']

def parent(emp):
	par = emp
	if emp == json_dict['L0'][0]['name']:
		return None
	for level in json_dict.keys():
		for emp_list in json_dict[level]:
			if emp_list['name'] == emp:
				if 'parent' in emp_list:
					par = emp_list['parent']
					return par

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
	res = root_val
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
		#print('x2:', x)
		#print('y2:', y)
	res = x

print(res)
print('Level of Employee-1 w.r.t. Leader:', find_depth(emp1, res))
print('Level of Employee-2 w.r.t. Leader:', find_depth(emp2, res))
#print('Level of', emp1, ':', count1)
#print('Level of', emp2, ':', count2)

	
		#for i in emp_list:
		#	print(i)
		#print(i.keys())
	#for i in level
#print(json_dict['L2'][1]['parent'])
