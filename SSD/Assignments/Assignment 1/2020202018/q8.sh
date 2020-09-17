#!/bin/bash
clear
crontab $1 &> /dev/null
exit_s=$(echo $?)
if [[ $exit_s == 1 ]]
then
	echo "No"
else
	echo "Yes"
fi
