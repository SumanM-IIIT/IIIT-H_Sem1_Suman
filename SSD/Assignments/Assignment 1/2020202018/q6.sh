#!/bin/bash
clear
ans=$1
shift 1
for i in $@
do
	ans=$(echo "$ans ^ $i" | bc)
done
echo $ans
