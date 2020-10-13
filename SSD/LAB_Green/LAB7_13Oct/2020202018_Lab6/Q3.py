names = input().split(',')
#names = ['sally', 'Dylan', 'rebecca', 'Diana', 'Joanne', 'keith']
op = []
for i in range(0, len(names)):
	if names[i][0].isupper():
		op.append(names[i])

len_word = lambda name: len(name)

len_name = 0
for name in op:
	len_name += len_word(name)

print(len_name)
print(op) 