#!/bin/sh
export POSIXLY_CORRECT=yes

ERROR=$(echo "Correct use is ./dirgraph [-i FILE_ERE] [-n] [DIR]")
NORM=0
DIR=$(pwd)

MAX=1

#COUNTER
DIRECTORIES=0
FILES=0
 
#HISTOGRAM
FILE100B=0  
FILE1kiB=0
FILE10kiB=0
FILE100kiB=0
FILE1MiB=0
FILE10MiB=0
FILE100MiB=0
FILE1GB=0
FILEB1GB=0

WIDE=79



if test -t ; then
WIDE=$(tput cols)
WIDE=$(($WIDE - 12))
fi
#check for usage
case $# in
0);;
1)	if [ $1 = "-n" ]; then NORM=1
	elif [ -d $1 ]; then DIR=$1
	else    echo "Wrong argument" >&2
		echo "$ERROR" >&2
		exit 1
	fi
;;
2)	if [ $1 = "-n" ]; then NORM=1
		if [ -d $2 ]; then DIR=$2
		else echo "Wrong argument" >&2
		echo "$ERROR" >&2
		exit 1
	fi
	elif [ $1 = "-i" ]; then IGNOR=$2 
	else    echo "Wrong argument" >&2
		echo "$ERROR" >&2
		exit 1
	fi
;;
3)	if [ $1 = "-i" ]; then IGNOR=$2
		else echo "Wrong argument" >&2
		echo "$ERROR" >&2
		exit 1
	fi
	if [ $3 = "-n" ]; then NORM=1
	elif [ -d $3 ]; then DIR=$3 
	else    echo "Wrong argument" >&2
		echo "$ERROR" >&2
		exit 1
	fi
;;
4)	if [ $1 = "-i" ]; then IGNOR=$2
	else    echo "Wrong argument" >&2
			echo "$ERROR" >&2
			exit 1
	fi
	if [ $3 = "-n" ]; then NORM=1
	else    echo "Wrong argument" >&2
			echo "$ERROR" >&2
			exit 1
	fi
	if [ -d $4 ]; then DIR=$4
	else    echo "Wrong argument" >&2
	echo "$ERROR" >&2
	exit 1
	fi
;;
*) 	echo "wrong number of arguments" >&2
	echo "$ERROR" >&2
	exit 1

esac

DIRECTORIES=$(find $DIR -type d | wc -l)
FILES=$(find $DIR -type f | wc -l)
#find the largest group
MAXCOUNT ()
{
if [ "$1" ]; then 
	if [ $1 -gt $MAX ]; then 
 		MAX=$(($1))
	fi
fi
}

#IF FILE_ERE EXISTS
if [ "$IGNOR" ]; then
	if echo "$DIR" | grep -qE "$IGNOR"; then
		echo "FILE_ERE can't be cover part of root direcoty" >&2
		exit 1
	fi
	DIRECTORIES=$(find $DIR -type d | grep -v $IGNOR | wc -l)
	FILES=$(find $DIR -type f | grep -v $IGNOR | wc -l)
	FILE100B=$(find $DIR -type f -size -100c | grep -v "$IGNOR" | wc -l)
	MAXCOUNT "$FILE100B"
	FILE1kiB=$(find $DIR -type f -size +100c -size -1024c | grep -v "$IGNOR" | wc -l)
	MAXCOUNT "$FILE1kiB"
	FILE10kiB=$(find $DIR -type f -size +1024c -size -10240c  | grep -v "$IGNOR" | wc -l)
	MAXCOUNT "$FILE10kiB"	
	FILE100kiB=$(find $DIR -type f -size +10240c -size -102400c | grep -v "$IGNOR" | wc -l)
	MAXCOUNT "$FILE100kiB"
	FILE1MiB=$(find $DIR -type f -size +102400c -size -1048576c | grep -v "$IGNOR" | wc -l)
	MAXCOUNT "$FILE1MiB"
	FILE10MiB=$(find $DIR -type f -size +1048576c -size -10485760c | grep -v "$IGNOR" | wc -l)
	MAXCOUNT "$FILE10MiB"
	FILE100MiB=$(find $DIR -type f -size +10485760c -size -104857600c | grep -v "$IGNOR" | wc -l)
	MAXCOUNT "$FILE100MiB"
	FILE1GB=$(find $DIR -type f -size +104857600c -size -1073741824c | grep -v "$IGNOR" | wc -l)
	MAXCOUNT "$FILE1GB"
	FILEB1GB=$(find $DIR -type f -size +1073741824c | grep -v "$IGNOR" | wc -l)
	MAXCOUNT "$FILEB1GB"
else
	FILE100B=$(find $DIR -type f -size -100c | wc -l)
	MAXCOUNT "$FILE100B"
	FILE1kiB=$(find $DIR -type f -size +100c -size -1024c | wc -l)
	MAXCOUNT "$FILE1kiB"
	FILE10kiB=$(find $DIR -type f -size +1024c -size -10240c | wc -l)
	MAXCOUNT "$FILE10kiB"
	FILE100kiB=$(find $DIR -type f -size +10240c -size -102400c | wc -l)
	MAXCOUNT "$FILE100kiB"
	FILE1MiB=$(find $DIR -type f -size +102400c -size -1048576c | wc -l)
	MAXCOUNT "$FILE1MiB"
	FILE10MiB=$(find $DIR -type f -size +1048576c -size -10485760c | wc -l)
	MAXCOUNT "$FILE10MiB"
	FILE100MiB=$(find $DIR -type f -size +10485760c -size -104857600c | wc -l)
	MAXCOUNT "$FILE100MiB"
	FILE1GB=$(find $DIR -type f -size +104857600c -size -1073741824c | wc -l)
	MAXCOUNT "$FILE1GB"
	FILEB1GB=$(find $DIR -type f -size +1073741824c | wc -l)
	MAXCOUNT "$FILEB1GB"
fi
RATIO=$((( $MAX / $WIDE ) + 1))
#dividing while normalization (if there is some files but less than ratio it will still print 1 hash because i think for user its easier to understand it)
DIVIDE ()
{
if [ "$1" ]; then
	if [ $1 -gt "0" ] && [ $1 -lt $RATIO ]; then 
	echo "1"
	else echo "$(( $1 / $RATIO ))"
	fi
fi
}

if [ $NORM -eq "1" ] && [ $MAX -gt $WIDE ]; then
	FILE100B="$(DIVIDE "$FILE100B")"
	FILE1kiB="$(DIVIDE "$FILE1kiB")"
	FILE10kiB="$(DIVIDE "$FILE10kiB")"
	FILE100kiB="$(DIVIDE "$FILE100kiB")" 
	FILE1MiB="$(DIVIDE "$FILE1MiB")"
	FILE10MiB="$(DIVIDE "$FILE10MiB")"
	FILE100MiB="$(DIVIDE "$FILE100MiB")"
	FILE1GB="$(DIVIDE "$FILE1GB")"
	FILEB1GB="$(DIVIDE "$FILEB1GB")" 
fi		
#printing hashes
HASH ()
{
i=1
s="#"
if [ "$1" -ne "0" ]; then
	while [ "$i" -ne "$1" ]; do
		s="${s}#"
		i=$(( i+1 ))
	done
echo "$s"
fi
}
 
#final output
echo "Root directory: $DIR"
echo "Directories: $DIRECTORIES"
echo "All files: $FILES"
echo "File size histogram:"
echo "  <100 B  : "$(HASH "$FILE100B")
echo "  <1 KiB  : "$(HASH "$FILE1kiB")
echo "  <10 KiB : "$(HASH "$FILE10kiB")
echo "  <100 KiB: "$(HASH "$FILE100kiB")
echo "  <1 MiB  : "$(HASH "$FILE1MiB")
echo "  <10 MiB : "$(HASH "$FILE10MiB")
echo "  <100 MiB: "$(HASH "$FILE100MiB")
echo "  <1 GiB  : "$(HASH "$FILE1GB")
echo "  >=1 GiB : "$(HASH "$FILEB1GB")

