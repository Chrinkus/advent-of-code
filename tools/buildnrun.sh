#! /bin/bash

while [ $# -ne 0 ]; do
	case $1 in
		-d|--day)
			DAY=$2
			shift 2;;
		-t|--test)
			TEST=$2
			shift 2;;
	esac
done

cmake --build . --target $DAY

if [ -n "$TEST" ]; then
	echo "$TEST" | ./$DAY
else
	./$DAY < "/home/${USER}/Documents/AoC_Inputs/${DAY}.txt"
fi

