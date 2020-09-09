#!/bin/bash
clear
awk '/^s[^a].*/{print}' $1
