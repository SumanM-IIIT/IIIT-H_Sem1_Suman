#!/bin/bash
clear
ps a -o pid | cut -d$'\n' -f2- > pid.txt
count=$(cat pid.txt | wc -w)

read N
if [[ $N -gt $count ]]
then
	N=$count
fi
head -n $N pid.txt | sort -n | awk '{$1=$1;print}'



