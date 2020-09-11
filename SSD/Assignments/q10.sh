#!/bin/bash
clear
read oprtr
read n_opr
declare -a arr
for (( i = 0; i < n_opr; i++ ))
do
	read arr[$i]
done
first=${arr[0]}
for (( i = 1; i < n_opr; i++))
do
	first=$(echo "scale=4; $first $oprtr ${arr[i]}" | bc)
done
echo $first
