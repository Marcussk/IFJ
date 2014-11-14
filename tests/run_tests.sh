#!/bin/bash

#if [ ! $# -eq 2 ]; then
#	echo "Expected arguments <> <>"
#	exit 1
#fi

#set parameters

if [ -e runnable_tests ]; then
	TESTS=$(cat runnable_tests)
else
	echo "There are no tests to run, make sure there is file 'runnable_tests'. Exiting..."
	exit 0
fi

chmod +x ../src/ifj.run

rm -rf results
mkdir results
#cd results

#run tests
for TEST in $TESTS;do

	echo "Testing FILE $TEST"
	../src/ifj.run $TEST > ../tests/results/$TEST.output

	RETVAL=$?

	echo "Returned $RETVAL"
	
	DIFF="`diff results/$TEST.output ref/$TEST.ref`"

	if [ -n "$DIFF" ]; then
		echo -e "${TEST}: \e[0;31mFAILED\e[0m"
		cat results/$TEST.output
	else
		echo -e "${TEST}: \e[0;32mSUCCEEDED\e[0m"
	fi
done

