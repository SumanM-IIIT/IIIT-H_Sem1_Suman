#!/bin/bash
clear
awk 'BEGIN{FS=OFS=" "} {print $4" "$3" "$2" "$1}' $1
