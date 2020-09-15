#!/bin/bash
clear
read sc_word
word_sorted=$(echo $sc_word | grep -o . | sort | tr -d "\n")
all_cmds=$(compgen -c)
for i in $all_cmds 
do
	a=$(echo $i | grep -o . | sort | tr -d "\n")
	if [[ $a == $word_sorted ]]; then
		echo "Yes"
		echo "$i"
		exit 0
	fi
done
echo "No"
