#!/bin/sh

failed=0
passed=0;

echo "Running tests..."

for i in *.json; do
	./dump $i > stdout.out 2> stderr.out

	if ! [ -e "$i.out" ] && ! [ -e "$i.err" ]; then
		echo "*** WARNING no file for $i"
	fi

	if [ -e $i.out ]; then
		if ! diff stdout.out $i.out &> /dev/null; then
			echo "************** $i ***************"
			diff -u stdout.out $i.out
			failed=$((failed+1))
			echo "*********************************"
		else
			passed=$((passed+1))
		fi
	fi

	if [ -e $i.err ]; then
		if ! diff stderr.out $i.err &> /dev/null; then
			echo "************** $i ***************"
			diff -u stderr.out $i.err
			failed=$((failed+1))
			echo "*********************************"
		else
			passed=$((passed+1))
		fi
	fi
done

echo

rm stdout.out stderr.out

echo "Passed $passed"
echo "Failed $failed"
