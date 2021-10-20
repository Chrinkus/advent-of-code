#!/bin/bash

YEAR=$1
DAY=$2
TITLE=$3

if [ ${#DAY} -eq 1 ]; then
	DAY="0${DAY}"
fi

FILE="day${DAY}.c"

cp boilerplate.c $FILE

sed -i "s/YEAR/$YEAR/; s/DAY/$DAY/; s/TITLE/$TITLE/" $FILE

