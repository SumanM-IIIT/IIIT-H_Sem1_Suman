#!/bin/bash
clear
mkdir -p Assignment1
cd Assignment1
find . -depth -name "*.txt" -exec sh -c 'f="{}"; mv -- "$f" "${f%.txt}.c"' \;
ls -laSr
find ~ -maxdepth 2 | uniq
readlink -f Assignment1/*.txt

