#/bin/bash

echo "executing test 1 out of 3"

./main.exe < input/test1.txt > output1.txt

diff output1.txt output/expectedOutput1.txt

if [ "$?" != "0" ] ; then 
	echo "test 1 failed"	
else 
	echo "test 1 succeeded"
fi

echo "executing test 2 out of 3"

./main.exe < input/test2.txt > output2.txt

diff output2.txt output/expectedOuput2.txt

if [ "$?" != "0" ] ; then 
	echo "test 2 failed"	
else 
	echo "test 2 succeeded"
fi

echo "executing test 3 out of 3"

./main.exe < input/test3.txt > output3.txt

diff output3.txt output/expectedOutput3.txt

if [ "$?" != "0" ] ; then 
	echo "test 3 failed"	
else 
	echo "test 3 succeeded"
fi
