#!/bin/bash
index=0
totaltests=0

for file in ../samples/*.pas
do
	totaltests=$((totaltests + 1))
	name=$(echo $file | awk -F'[/]' '{print $3}' | awk -F'[.]' '{print $1}')
	echo TEST$totaltests $name
	../src/bin/ifj.run ../samples/$name.pas > results/$name.out
	
	if [ -e "output/$name.out" ]
	then
	DIFF=$(diff results/$name.out output/$name.out) 
		if [ "$DIFF" == "" ] 
		then
    	index=$((index + 1))
    	echo Passed
		else
		diff -c results/$name.out output/$name.out
		#echo $DIFF
		fi
	else
		echo output/$name.out does not exist
	fi	
done

echo Total tests : $totaltests Passed: $index
