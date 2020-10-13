import string

all_letters = set(string.ascii_lowercase)
#print(all_letters)
sentence = input()
sent_lower = sentence.lower()

if set(sent_lower) >= all_letters:
	print("YES")
else:
	print("NO")