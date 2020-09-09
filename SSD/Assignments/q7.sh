#!/bin/bash
clear
ps ax -o pid | cut -d$'\n' -f2- > pid.txt
count=$(cat pid.txt | wc -w)

read -p "Enter a number: " N
if(($N>$count))
then
	N=$count
fi
head -n $N pid.txt | awk '{$1=$1;print}'



