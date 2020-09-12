#!/bin/bash
clear
read line < $1
declare -a arr
flag=0
IFS=' '
read -ra arr<<<"$line"

num_re="(^[0-9]$)|(^[0-5][0-9]$)|[*]"
if ! [[ "${arr[0]}" =~ $num_re ]]
then
	flag=1
fi

num_re="(^[0-9]$)|(^[0-1][0-9]$)|(^2[0-3]$)|[*]"
if ! [[ "${arr[1]}" =~ $num_re ]]
then
        flag=1
fi

num_re="(^[1-9]$)|(^0[1-9]$)|(^[1-2][0-9]$)|^3[0-1]$|[*]"
if ! [[ "${arr[2]}" =~ $num_re ]]
then
        flag=1
fi

num_re="(^[1-9]$)|(^0[1-9]$)|(^1[0-2]$)|[*]"
if ! [[ "${arr[3]}" =~ $num_re ]]
then
        flag=1
fi

num_re="(^[0-6]$)|(^0[0-6]$)|[*]"
if ! [[ "${arr[4]}" =~ $num_re ]]
then
        flag=1
fi

if [[ $flag =~ 1 ]]
then
	echo "No"
else
	echo "Yes"
fi

