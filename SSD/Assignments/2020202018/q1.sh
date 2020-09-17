#!/bin/bash
clear
mkdir -p Assignment1
cd Assignment1
touch lab1.txt lab2.txt lab3.txt lab4.txt lab5.txt
find . -depth -name "*.txt" -exec sh -c 'f="{}"; mv -- "$f" "${f%.txt}.c"' \;
ls -laSr
find ~ -maxdepth 2 | uniq
readlink -f Assignment1/*.txt

