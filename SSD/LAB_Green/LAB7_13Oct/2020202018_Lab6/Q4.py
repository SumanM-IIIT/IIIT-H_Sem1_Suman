def test_div(bin_num):
	tmp = bin_num
	p = 0
	digit = 0
	dec = 0
	while(tmp != 0):
		digit = tmp % 10
		dec += digit * pow(2, p)
		tmp = tmp // 10
		p += 1
	
	if dec % 5 == 0:
		return True
	else:
		return False

bin_string = input()
list_bin = bin_string.split(',')
set_bin = set()

for i in range(0, len(list_bin)):
	if test_div(int(list_bin[i])):
		set_bin.add(list_bin[i])

for dec in set_bin:
	print(dec, end = ", ")
print()