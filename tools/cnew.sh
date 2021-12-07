#!/bin/bash

# cnew.sh
#
# Create a C file for an Advent of Code solution
#
# cnew -y 20 -d 1 -t "Report Repair"
# - creates a file titled 2001.c in the current directory
# - file is set up with header and result output ready to go

# Sensible defaults
YEAR=$(date +%Y)
DAY=$(TZ="America/New_York" date +%d)
TITLE="UNKNOWN"

while [ $# -ne 0 ]; do
	case "$1" in
		-y|--year)
			YEAR="$2"
			shift 2;;
		-d|--day)
			DAY="$2"
			shift 2;;
		-t|--title)
			TITLE="$2"
			shift 2;;
		-h|--help)
			echo "Usage: $0 -y year -d day -t title"
			exit 0;;
		*)
			exit 1;;
	esac
done

# Left-pad single digit days
if [ ${#DAY} -eq 1 ]; then
	DAYFILE="0${DAY}"
else
	DAYFILE="${DAY}"
fi

# Strip century from year
if [ ${#YEAR} -eq 4 ]; then
	YEARFILE="${YEAR:2:2}"
else
	YEARFILE="${YEAR}"
	YEAR="20${YEAR}"
fi

# File name
FILE="${YEARFILE}${DAYFILE}.c"

if [ ${YEAR} -eq 2021 ]; then
	BANNER="aoc_banner_2021(\"${DAY}\", \"${TITLE}\");"
else
	BANNER="aoc_banner(\"${YEAR}\", \"${DAY}\", \"${TITLE}\");"
fi

# Boilerplate with placeholders
cat << _EOF_ > $FILE
#include <stdlib.h>
#include <stdio.h>

#include "aoc_banner.h"

int main()
{
	${BANNER}

	int part1 = 0;
	int part2 = 0;

	printf(TCINV "Part 1:" TCRINV " %d\n", part1);
	printf(TCINV "Part 2:" TCRINV " %d\n", part2);

	return EXIT_SUCCESS;
}

_EOF_

