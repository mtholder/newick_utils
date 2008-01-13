#! /bin/sh

echo "testing: $0"

prog=${0%.sh}	# derive tested program's name from own name
prog=${prog#*_}

echo "program: $prog"
pass=TRUE
while IFS=':' read name args ; do
	echo -n "test: $name - "
	../src/$prog $args > test_${prog}_$name.out
	if diff test_${prog}_$name.out test_${prog}_$name.exp ; then
		echo "pass"
	else
		echo "FAIL"
		pass=FALSE
	fi
done < test_${prog}_args

if test $pass = FALSE ; then
	return 1
else
	return 0
fi
