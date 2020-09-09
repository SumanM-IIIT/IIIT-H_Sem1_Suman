#!/bin/bash
clear
read -p "Enter a value: " str
str=$(echo $str | tr '[:upper:]' '[:lower:]')
len=$(echo $str | wc -c)
len=$(expr $len - 1)
#echo $len
x=1
y=$(expr $len / 2)
while test $x -le $y
	 do
		p=$(echo $str | cut -c $x)
		q=$(echo $str | cut -c $len)
		if [ $p != $q ]
		then
			echo "No"
			exit
		fi
		x=$(expr $x + 1)
		len=$(expr $len - 1)
done
echo "Yes"

