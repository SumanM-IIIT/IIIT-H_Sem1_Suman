#!/bin/bash
clear
read -p "Enter a word: " sc_word
all_cmds=$(compgen -c)
cnt=$(echo $all_cmds | wc -w)
echo | awk 'if ($sc_word in $all_cmds) {print "YES"}' 

