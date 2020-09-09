#!/bin/bash
clear
cat hamlet.txt | grep -i " to " -c				#1. count of all occurrences of <to> (case insensitive)
cat hamlet.txt | grep -n " is " 				#2. all lines containing the word <is>(used <-n> just for line number)
cat hamlet.txt | grep -n " bear " -A2 | grep -v " bear "	#3. 2 lines below <bear> (excluding it)
chmod g=,o= hamlet.txt 						#4a. (symbolic way)
chmod 000 hamlet.txt						#4b. (octal way)
chmod a+r,a+w,a+x hamlet.txt					#5. give each permission to all
groups $USER							#6. groups of current user
sudo chown root hamlet.txt					#7. changing the ownership/group to root
ls -la ~ | grep "^d[r-][w-][x-][r-][w-]x"			#8. list files from Home directory having group execute permissions


