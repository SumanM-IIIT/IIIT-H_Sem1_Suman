#!/bin/bash
clear
read sc_word
all_cmds=$(compgen -c)
for i in $all_cmds 
do
	if [[ $i == $sc_word ]]
	then
		echo "Yes"
		echo "$i"
		exit 0
	fi
done
echo "No"
