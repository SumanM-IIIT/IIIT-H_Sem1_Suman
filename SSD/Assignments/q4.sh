\#1/bin/bash
clear
read list
output=$(echo $list | sed 's/)(/ /g' | sed 's/)//g' | sed 's/(//g' | sed 's/ \+/ /g')
echo "($output)" | sed 's/ \+)/)/g' | sed 's/( \+/(/g'
