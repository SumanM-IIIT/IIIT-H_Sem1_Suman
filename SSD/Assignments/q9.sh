#!/bin/bash
clear
read number
len=${#number}
k=0
sum=0
declare -a arr
if (( len >= 1 ))
then
	for ((i = 0; i < len; i++))
	do
		#echo ${number:i:1}
		tmp=${number:i:1}
		if [[ ($tmp -ge 0 && $tmp -le 9) || $tmp == *[[:space:]]* ]]
		then
			if [[ $tmp != *[[:space:]]* ]]
			then
				arr[k]=$tmp
				k=$(expr $k + 1)
			fi
		else
			echo "Invalid"
			exit 0
		fi
	done
	for ((i = k - 2; i >= 0; i = i - 2))
	do
		tmp=${arr[i]}
		tmp=$(expr $tmp \* 2)
		if (( $tmp > 9 ))
		then
			tmp=$(expr $tmp - 9)
		fi
		arr[i]=$tmp;
	done
	for((i = 0; i < k; i++))
	do
		sum=$(expr $sum + ${arr[i]})
	done
	#sum=$(expr $sum % 10)
	#echo $sum
	if (( $sum%10 == 0 ))
	then
		echo "Valid"
	else
		echo "Invalid"
	fi
else
	echo "Invalid"
fi

