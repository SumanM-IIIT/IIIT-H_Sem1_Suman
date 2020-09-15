#!/bin/bash
clear
read sc_word
w_len=${#sc_word}
#word_sorted=$(echo $sc_word | grep -o . | sort | tr -d "\n")
all_cmds=$(compgen -c)

for ((i = 0; i < w_len; i++)) 
do
	for j in $all_cmds
	do
		if ! [[ w_len == ${#j} && $j == *"${sc_word:$i:1}"* ]]
		then
			all_cmds=("${all_cmds[@]/$j}" )
		fi
	done
done
echo "${all_cmds[@]}"

#for i in $all_cmds 
#do
#        a=$(echo $i | grep -o . | sort | tr -d "\n")
#        if [[ $a == $word_sorted ]]; then
#                echo "Yes"
#                echo "$i"
#                exit 0
#        fi
#done
#echo "No"
