#!/bin/bash

file=$1

# open .ino file and prepend #include <WProgram.h> to the beginning
# rename to file.cpp
# find a place to prepend function headers
# make
# make upload
# remove file.cpp

basename=`basename $1 .ino`

moveToCPP $basename
prependHeader $basename.cpp
make
make upload
cleanUp $basename.cpp

function moveToCPP {
	file=$1
	mv $file.ino $file.cpp
}

# http://stackoverflow.com/a/27950358
function prependHeader {
	file=$1
	prepend = '#include <WProgram.h>'
	echo -e "$prepend$(cat $1)" > $1
}

function cleanUp {
	basename = $1
	rm $basename.cpp
}

function getFunctionNames {
	file=$1
	ctags -x $file | awk '{$1=$2=$3=""; sub(/^ *\)*/,"", $0); print }'
}
