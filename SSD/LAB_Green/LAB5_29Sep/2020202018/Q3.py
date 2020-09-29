str = input();
word_list = str.replace(" ","").split(",");
word_length = map(len, word_list);
print(list(word_length));