#!/bin/bash
clear
mkdir Assignment1
cd Assignment1
touch lab1.txt lab2.txt lab3.txt lab4.txt lab5.txt
rename 's/\.txt$/\.c/' *.txt
ls -laSr
find ~ -maxdepth 2 | uniq
readlink -f Assignment1/*.txt

