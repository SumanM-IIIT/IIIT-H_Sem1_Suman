#!/bin/bash
clear
awk '/[wW][oO][rR][kK]/{print}' $1 | grep '.* .* [wW][oO][rR][kK]' | sed 's/[wW][oO][rR][kK]/good/g'

