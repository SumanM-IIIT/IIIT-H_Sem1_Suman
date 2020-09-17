#!/bin/bash
clear
HISTFILE=~/.bash_history
set -o history
history 10| awk '{c[$2]++}END{for (x in c) print x,c[x]}' | sort -k2 -nr
