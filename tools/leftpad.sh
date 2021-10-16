#! /bin/bash

# leftpad.sh
#
# Pad a zero to the left side of single digits in file names to
# improve sorting.
#
# Ex. $ ls day[1-9].cpp | leftpad.sh

while read file; do
	newfile=$(echo $file | sed 's/[1-9]/0&/');
	mv $file $newfile && echo "moved $file -> $newfile"
done

