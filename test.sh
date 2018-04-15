#!/bin/bash

file="./book_urls"

echo "Downloading files for testing from $file"

while read line
do
	wget $line
done < $file

echo "Finished downloading files.\nCreating a single file for test"
cat *.txt > testing
rm *.txt

for i in 1000 10000 100000 1000000
do
	echo -e "\nTesting for $i lines:"
	echo -e "\nBubble sort:"
	time  head -n $i -q testing | ./linesort -b | head -n 0
	echo -e "\nMerge sort:"
	time head -n $i -q testing | ./linesort -m | head -n 0
	echo -e "\nBinary search tree:"
	time head -n $i -q testing | ./linesort_tree | head -n 0
done
rm testing
