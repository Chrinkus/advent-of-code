#! /bin/bash

# Renaming AoC 'day' files in batches

function usage {
	cat <<- _EOF_
		Usage: $0 [options]

		    -y, --year    Two digit year to prefix day number with.
		    -m, --move    Rename files using year prefix.
		    -h, --help    This help message.

		It is advised to do a dry run without '-m' to review changes.
	_EOF_
}

ACTION="echo"	# Default is to print new file names
PREFIX="day"	# Default prefix

while [ $# -ne 0 ]; do
	case $1 in
		-y|--year)
			YEAR=$2
			shift 2;;
		-m|--move)
			ACTION="move"
			shift ;;
		-p|--prefix)
			PREFIX=$2
			shift 2;;
		-h|--help)
			usage
			exit 0;;
		*)
			echo "Unknown argument: $1"
			exit 1;;
	esac
done

PRELEN=${#PREFIX}

for file in $(ls ${PREFIX}*); do
	EXT=$(echo "${file}" | cut -d '.' -f 2)

	if [ $ACTION = "move" ]; then
		mv "$file" "${YEAR}${file:${PRELEN}:2}.$EXT"
	else
		echo "Would move ${file} to ${YEAR}${file:${PRELEN}:2}.$EXT"
	fi
done

