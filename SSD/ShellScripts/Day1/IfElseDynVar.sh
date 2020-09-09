#!/bin/bash
clear
if test "$1" = ""
	then
		echo "No 1st Value Passed."
		exit
fi
if test "$2" = ""
	then 
		echo "No 2nd Value Passed."
		exit
fi
clear
echo "Sum of Values: " $(($1+$2))
#echo $(expr $1+$2)
